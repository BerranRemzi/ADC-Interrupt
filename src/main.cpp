#include <Arduino.h>
#include <avr/power.h>

#define PRESCALER 64                                 // timer prescaler
#define TIMER_PERIOD_MS 1000
#define COMPARE_VALUE (F_CPU / PRESCALER / TIMER_PERIOD_MS) - 1 // compare value for 1ms period

#define digitalToggle(PIN) digitalWrite(PIN, !digitalRead((PIN)))

#define ISR_ENTER(ID) digitalWrite((ID), HIGH)
#define ISR_EXIT(ID) digitalWrite((ID), LOW)

#define TASK_ENTER(ID) digitalWrite((ID), HIGH)
#define TASK_EXIT(ID) digitalWrite((ID), LOW)

const uint8_t adc_map[] = {0, 1};

volatile uint16_t adc_value[sizeof(adc_map)]; // result of ADC conversion
volatile uint8_t adc_channel = 0;             // current ADC channel

void Adc_StartConversion(uint8_t channel)
{
  ISR_ENTER(4);
  ADMUX = (1 << REFS0) | channel; // select reference voltage and ADC channel
  ADCSRA |= (1 << ADSC);          // start ADC conversion
  ISR_EXIT(4);
}

ISR(TIMER1_COMPA_vect) // timer interrupt service routine
{
  ISR_ENTER(2);
  if (0 == adc_channel)
  {
    Adc_StartConversion(adc_map[adc_channel]);
  }
  ISR_EXIT(2);
}

ISR(ADC_vect) // ADC conversion complete interrupt service routine
{
  ISR_ENTER(3);
  adc_value[adc_channel] = ADC; // read ADC value
  adc_channel++;

  if (adc_channel < sizeof(adc_map))
  {
    Adc_StartConversion(adc_map[adc_channel]);
  }
  else
  {
    adc_channel = 0;
  }
  ISR_EXIT(3);
}

void Timer_Init(void)
{
  // set up timer 1 in CTC mode
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
  OCR1A = COMPARE_VALUE;
  TIMSK1 = (1 << OCIE1A);
}

void ADC_Init(void)
{
  ADMUX = (1 << REFS0) | adc_map[adc_channel];                                     // select reference voltage and ADC channel
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC and interrupt, set prescaler
}

void setup(void)
{
  cli(); // disable global interrupts
  clock_prescale_set(clock_div_2);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  Timer_Init();
  ADC_Init();

  sei(); // enable global interrupts
}

void loop(void)
{
  digitalToggle(LED_BUILTIN);
  static uint8_t counter = 0;
  Serial.println(counter);
  counter++;

  delay(10);
  // use adc_value[0] and adc_value[1] for your purposes
}
