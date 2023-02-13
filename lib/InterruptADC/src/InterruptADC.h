#ifndef INTERRUPT_ADC_H
#define INTERRUPT_ADC_H

#include "Arduino.h"

#define digitalToggle(PIN) digitalWrite(PIN, !digitalRead((PIN)))

#define ISR_ENTER(ID) digitalWrite((ID), HIGH)
#define ISR_EXIT(ID) digitalWrite((ID), LOW)

#define TASK_ENTER(ID) digitalWrite((ID), HIGH)
#define TASK_EXIT(ID) digitalWrite((ID), LOW)

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
    uint8_t _map[16];
    volatile uint16_t _result[16];      // result of ADC conversion
    volatile uint8_t _channel = 0u; // current ADC channel
    uint8_t _channelCount = 0u;
};

extern InterruptADC_ InterruptADC;

#endif /* INTERRUPT_ADC_H */