#include <Arduino.h>
#include <avr/power.h>
#include <TimerOne.h>
#include <InterruptADC.h>

#define TIMER_PERIOD_MS 1000

void setup(void)
{
  cli(); // disable global interrupts
  clock_prescale_set(clock_div_2);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  InterruptADC.initialize();
  InterruptADC.pinMode(A0, INPUT);
  InterruptADC.pinMode(A1, INPUT);

  Timer1.initialize(TIMER_PERIOD_MS);
  Timer1.attachInterrupt(InterruptADC.start); // blinkLED to run every 0.15 seconds
  
  sei(); // enable global interrupts
}

void loop(void)
{
  digitalToggle(LED_BUILTIN);

  Serial.print(InterruptADC.analogRead(A0));
  Serial.print(",");
  Serial.println(InterruptADC.analogRead(A1));

  delay(10);
  // use adc_value[0] and adc_value[1] for your purposes
}
