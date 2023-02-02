
# ADC-Timer Interrupt Example

This code demonstrates the use of ADC and Timer interrupts in an Arduino environment. The code sets up timer 1 to generate a compare match interrupt every 1 ms and starts ADC conversions for 2 channels in a cyclic manner. The ADC conversion complete interrupt updates the ADC values for each channel and selects the next channel.

The ISR_ENTER and ISR_EXIT macros are used to mark the entry and exit of interrupt service routines (ISRs).
The TASK_ENTER and TASK_EXIT macros are used to mark the entry and exit of tasks. The digitalToggle function toggles the state of a digital output pin.

## Hardware Requirements

-   Arduino board with ATmega328P or similar

## Code Overview

-   `PRESCALER` is defined as 1024 and is used as the timer prescaler value.
-   `COMPARE_VALUE` is defined as the compare value for a 1ms period.
-   `digitalToggle(PIN)` is a macro that toggles the state of a digital output pin.
-   `ISR_ENTER(ID)` and `ISR_EXIT(ID)` are macros that mark the entry and exit of ISRs, respectively.
-   `TASK_ENTER(ID)` and `TASK_EXIT(ID)` are macros that mark the entry and exit of tasks, respectively.
-   `adc_value[2]` is an array to store the ADC values for 2 channels.
-   `adc_channel` is a variable to store the current ADC channel.
-   `adc_map[2]` is an array to map the ADC channels to their corresponding ADC multiplexer input.
-   `TIMER1_COMPA_vect` is the timer compare match interrupt service routine (ISR). It selects the first ADC channel and starts an ADC conversion.
-   `ADC_vect` is the ADC conversion complete interrupt service routine (ISR). It reads the ADC value, increments the ADC channel, and selects the next channel for conversion.
-   `Timer_Init()` initializes timer 1 in CTC mode and sets the compare match value.
-   `ADC_Init()` initializes the ADC and enables the ADC interrupt.
-   The `setup()` function sets the modes of digital output pins 2, 3, 4, and the built-in LED, initializes the timer and ADC, and enables global interrupts.
-   The `loop()` function toggles the state of the built-in LED and performs any necessary tasks.

## How to use

1.  Connect the required hardware to your Arduino board.
2.  Open the code in the Arduino IDE.
3.  Upload the code to your Arduino board.
4.  Observe the behavior of the built-in LED and the output pins.

## Note

This code is provided as an example and may need to be modified for your specific use case.
