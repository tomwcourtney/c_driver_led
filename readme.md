# c_driver_led
C driver for controlling led's using sequences.

## Table of Contents
- [Setup](#setup)
- [Useage](#useage)

## Setup 
You must define a pin_t type which describes a pin for your platform. This is to be defined in user_led.h Example for STM32 + HAL:
```C
typedef struct
{
  GPIO_TypeDef * port;
  uint16_t pin;
} pin_t;

```
You must also implement a 'write function' in user_led.h that takes two inputs, your defined pins_t type and also led_state_t. Example for STM32 + HAL:
```C
void write(pin_t pin, gpio_state_t state)
{
  GPIO_PinState hal_state = state == GPIO_STATE_HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(pin.port, pin.pin, hal_state);
}
```
If you are using RGB leds then your write function may take the form of somthing like this:
```C
void write(pins_t pins, led_state_t state)
{
	if(pins.pin == 0) // red
	{
		TIM2->CCR3 = state;
	}
	else if (pins.pin == 1) // green
	{
		TIM22->CCR2 = state;
	}
	else if (pins.pin == 2) // blue
	{
		TIM22->CCR1 = state;
	}
}

```

## Useage
### Normal Led Usage 
Once the user code has been completed the driver can be initalised by calling the led_init function,
with the callback period of the led_update function (see doxygen documentation for more detial).

After init has been called sequences and leds can be defined can be defined and assigned to leds. For example the initalisation of the driver where the led_update is called every 250ms with a simple on off pattern running on a single led :
```C
// Init Driver 
led_init(250);
// Define an led 
led_t new_led = {
            .enabled = true,
            .pinout = {.port = LED_PORT, .pin = LED_PIN},
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
            .sequence_initialized = false
        };
int32_t led_id =  led_register(new_led);       
// Define on off pattern 
sequence_t sequence = {
            .sequence[5] = {LED_OFF, LED_ON, LED_ON, LED_OFF};
            .length      = 5;
            .period      = 10;
        };
int32_t seq_id = sequence_register(sequence);
// assign Sequence to led 
led_assign_sequence(led_id, seq_id);
// Main loop 
while(true)
{
 led_update_state();
 HAL_Delay(250);
}
```
### RGB Led Usage
How the driver functions overall does not change that much when using RGB led's, the following
is a comperable example that is an RGB led flashing through colours. 
```C
// Init Driver 
led_init(250);
rgb_led_init();
// Define an led 
led_t new_led = {
        .enabled = enabled,
        .pinout = {.pin = 0}, // It does not matter what this is set as
        .sequence_id = -1,
        .sequence_idx = 0,
        .timer_count = 0,
        .sequence_initialized = false};
pins_t redPin = {{.port = R_LED_PORT, .pin = R_LED_PIN}}
pins_t greenPin = {{.port = G_LED_PORT, .pin = G_LED_PIN}}
pins_t bluePin = {{.port = B_LED_PORT, .pin = B_LED_PIN}}
uint32_t rgb_led_id = rgb_led_register(redPin, greenPin, bluePin, new_led);   
// Define Colour pattern
uint32_t seq[4] = {C_WHITE, C_GRN, C_BLUE, C_OFF};
int32_t rgb_seq_id = rgb_sequence_register(4, 8, seq_1);
// assign Sequence to led 
rgb_assign_sequence(rgb_led_id, rgb_seq_id);
// Main loop 
while(true)
{
 led_update_state();
 HAL_Delay(250);
}
```
