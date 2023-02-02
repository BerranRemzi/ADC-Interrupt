#include <Arduino.h>

#define PRESCALER 1024                           // timer prescaler
#define COMPARE_VALUE (F_CPU / PRESCALER / 1000) // compare value for 1ms period
#define digitalToggle(PIN) digitalWrite(PIN, !digitalRead((PIN)))

#define ISR_ENTER(ID) digitalWrite((ID), HIGH)
#define ISR_EXIT(ID) digitalWrite((ID), LOW)

#define TASK_ENTER(ID) digitalWrite((ID), HIGH)
#define TASK_EXIT(ID) digitalWrite((ID), LOW)

volatile uint16_t adc_value[2];   // result of ADC conversion
volatile uint8_t adc_channel = 0; // current ADC channel

const uint8_t adc_map[2] = {0, 1};

ISR(TIMER1_COMPA_vect) // timer interrupt service routine
{
  ISR_ENTER(2);
  if (0 == adc_channel)
  {
    ADMUX = (1 << REFS0) | adc_map[adc_channel]; // select reference voltage and ADC channel
    ADCSRA |= (1 << ADSC);                       // start ADC conversion
    
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
    ADMUX = (1 << REFS0) | adc_map[adc_channel]; // select reference voltage and ADC channel
    ADCSRA |= (1 << ADSC);              // start ADC conversion
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
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  OCR1A = COMPARE_VALUE;
  TIMSK1 = (1 << OCIE1A);
}

void ADC_Init(void)
{
  ADMUX = (1 << REFS0) | adc_map[adc_channel];                                              // select reference voltage and ADC channel
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC and interrupt, set prescaler
}

void setup(void)
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  cli(); // disable global interrupts

  Timer_Init();
  ADC_Init();

  sei(); // enable global interrupts
}

void loop(void)
{
  digitalToggle(LED_BUILTIN);
  delay(10);
  // use adc_value[0] and adc_value[1] for your purposes
}
