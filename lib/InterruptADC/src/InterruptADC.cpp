#include "InterruptADC.h"
#include "Arduino.h"
#include "wiring_private.h"
#include <avr/power.h>

InterruptADC_ InterruptADC;

extern uint8_t analog_reference;

void InterruptADC_::initialize(void)
{
    // ADMUX = (1 << REFS0) | _map[_channel].;                                          // select reference voltage and ADC channel
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // enable ADC and interrupt
}

void InterruptADC_::startSingleConversion(uint8_t pos)
{
    ISR_ENTER(4);
    ADCSRB = _map[pos].ADCSRB_value;
    ADMUX = _map[pos].ADMUX_value; // select reference voltage and ADC channel
    ADCSRA |= (1 << ADSC);         // start ADC conversion
    ISR_EXIT(4);
}

void InterruptADC_::start(void)
{
    ISR_ENTER(2);
    if (0 == InterruptADC._channel)
    {
        InterruptADC.startSingleConversion(InterruptADC._channel);
    }
    ISR_EXIT(2);
}

void InterruptADC_::interrupt(void)
{
    ISR_ENTER(3);
    _map[_channel].result = ADC; // read ADC value
    _channel++;

    if (_channel < _channelCount)
    {
        startSingleConversion(_channel);
    }
    else
    {
        _channel = 0;
    }
    ISR_EXIT(3);
}

void InterruptADC_::pinMode(uint8_t pin, uint8_t mode)
{
    _map[_channelCount].analogPin = pin;
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
    if (pin >= 18)
        pin -= 18; // allow for channel or pin numbers
#endif
    pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    if (pin >= 54)
        pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
    if (pin >= 18)
        pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
    if (pin >= 24)
        pin -= 24; // allow for channel or pin numbers
#else
    if (pin >= 14)
        pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    _map[_channelCount].ADCSRB_value = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif

    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
    _map[_channelCount].ADMUX_value = (analog_reference << 6) | (pin & 0x07);
#endif
#endif

    _channelCount++;
}

uint16_t InterruptADC_::analogRead(uint8_t pin)
{
    uint16_t result = 0u;
    for (uint8_t i = 0u; i < _channelCount; i++)
    {
        if (pin == _map[i].analogPin)
        {
            result = _map[i].result;
            break;
        }
    }
    return result;
}

ISR(ADC_vect) // ADC conversion complete interrupt service routine
{
    InterruptADC.interrupt();
}
