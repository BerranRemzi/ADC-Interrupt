#ifndef INTERRUPT_ADC_H
#define INTERRUPT_ADC_H

#include "Arduino.h"

#ifdef INTERRUPT_ADC_DEBUG
#define digitalToggle(PIN) digitalWrite(PIN, !digitalRead((PIN)))

#define ISR_ENTER(ID) digitalWrite((ID), HIGH)
#define ISR_EXIT(ID) digitalWrite((ID), LOW)
#define TASK_ENTER(ID) digitalWrite((ID), HIGH)
#define TASK_EXIT(ID) digitalWrite((ID), LOW)
#else
#define digitalToggle(PIN)

#define ISR_ENTER(ID)
#define ISR_EXIT(ID)
#define TASK_ENTER(ID)
#define TASK_EXIT(ID)
#endif /* INTERRUPT_ADC_DEBUG */

typedef struct
{
    uint8_t analogPin;
    uint8_t adc;
    uint8_t ADCSRB_value;
    uint8_t ADMUX_value;
    uint16_t result;
} InterruptADC_Channel_t;

class InterruptADC_
{
public:
    void initialize(void);
    void startSingleConversion(uint8_t channel);
    static void start(void);
    void interrupt(void);
    void pinMode(uint8_t pin, uint8_t mode);
    uint16_t analogRead(uint8_t pin);

private:
    volatile InterruptADC_Channel_t _map[16];
    volatile uint8_t _channel = 0u; // current ADC channel
    uint8_t _channelCount = 0u;
};

extern InterruptADC_ InterruptADC;

#endif /* INTERRUPT_ADC_H */