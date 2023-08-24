/**
 * @file A driver for LEDs. They can be RGB or not and there can be 1 or many
 * LEDs. 
 */

#ifndef LED_H
#define LED_H


#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Holds state information for an led's configuration.
 * 
 * @param id        - A unique identifier for an led. 
 * @param enabled   - True if led is in use, false otherwise.
 * @param sequence  - A pointer to a sequence object that defines the flash pattern of the led.
 * @param pinout    - A pointer to a pinout object that defines connection of the led to the microcontroller.
 */
typedef struct {
    uint32_t id;
    bool enabled;
} led_t;

/** 
 * @brief Definitions of enums for the on and off states of an LED.
 */
typedef enum{
    LED_UNDEFINED,
    LED_ON,
    LED_OFF
}led_state_t;

/**
 * @brief user defined structure which contains hardware specific pin information needed to toggle the pin
 * e.g. in STM32 uising HAL, pins_t would be defined as 
 * typdef struct {
    GPIO_TypeDef * port;
    uint16_t pin;
 } pins_t;
*/
typedef struct pins_t pins_t;

typedef void (*write_pin)(pins_t, led_state_t);

/**
 * @brief The inialisation for the led driver, sets the 'write_fcn' that the driver uses to contorl the leds and the callback 
 * frequency that led_update will be called 
 * @param [IN] write_fcn - Function pointer to the hardwear specific implementation of the write is located 
 * @param [IN] callback_frequency - How often the led_update function will be called 
*/
void led_init(write_pin write, uint32_t callback_frequency);

/**
 * @brief Return the number of registered LEDs in the led module.
 * 
 * @returns uint16 - the number of registered LEDs.
 * 
 */
uint16_t led_get_led_count();

/**
 * @brief turn on selected LED
 *
 * @param [IN] id - ID of LED to be turned on
*/
void led_on(uint32_t id);

/* INTERFACE 
   led_register_led(led_id)
   led_register_sequence(sequence_array, sequence_period, length)
   led_set_sequence(led_id,sequence_id)

   led_start_sequence() //Starts the current sequence that the leds are configured for 
   led_stop_sequence()  //Stops the current sequence that the leds are configured for 
   
   led_on(led_id)
   led_off(led_id)
   
   led_all_on()
   led_all_off()
*/

#endif