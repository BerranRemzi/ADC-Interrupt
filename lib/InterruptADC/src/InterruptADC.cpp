#include "InterruptADC.h"
#include <avr/power.h>

InterruptADC_ InterruptADC;

void InterruptADC_::initialize(void)
{
    ADMUX = (1 << REFS0) | _map[_channel];                                     // select reference voltage and ADC channel
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC and interrupt, set prescaler
}

void InterruptADC_::startSingleConversion(uint8_t channel)
{
    ISR_ENTER(4);
    ADMUX = (1 << REFS0) | channel; // select reference voltage and ADC channel
    ADCSRA |= (1 << ADSC);          // start ADC conversion
    ISR_EXIT(4);
}

void InterruptADC_::start(void)
{
    ISR_ENTER(2);
    if (0 == InterruptADC._channel)
    {
        InterruptADC.startSingleConversion(InterruptADC._map[InterruptADC._channel]);
    }
    ISR_EXIT(2);
}

void InterruptADC_::interrupt(void)
{
    ISR_ENTER(3);
    _result[_channel] = ADC; // read ADC value
    _channel++;

    if (_channel < _channelCount)
    {
        startSingleConversion(_map[_channel]);
    }
    else
    {
        _channel = 0;
    }
    ISR_EXIT(3);
}

void InterruptADC_::pinMode(uint8_t pin, uint8_t mode)
{
    _channelCount++;
}
uint16_t InterruptADC_::analogRead(uint8_t pin)
{
    return _result[pin];
}

ISR(ADC_vect) // ADC conversion complete interrupt service routine
{
    InterruptADC.interrupt();
}