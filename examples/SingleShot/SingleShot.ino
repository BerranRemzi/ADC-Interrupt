#include <Arduino.h>
#include <InterruptADC.h>

#define TIMER_PERIOD_MS 1000

void setup(void)
{
  Serial1.begin(9600);

  InterruptADC.pinMode(A0, INPUT);
  InterruptADC.pinMode(A1, INPUT);
  InterruptADC.pinMode(A4, INPUT);
  InterruptADC.pinMode(A5, INPUT);
  InterruptADC.initialize();
}

void loop(void)
{
  digitalToggle(LED_BUILTIN);

  Serial1.print(InterruptADC.analogRead(A0));
  Serial1.print(",");
  Serial1.print(InterruptADC.analogRead(A1));
  Serial1.print(",");
  Serial1.print(InterruptADC.analogRead(A4));
  Serial1.print(",");
  Serial1.println(InterruptADC.analogRead(A5));

  InterruptADC.start();
  delay(100);
}
