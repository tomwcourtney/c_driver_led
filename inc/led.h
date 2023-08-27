/**
 * @file A driver for LEDs. They can be RGB or not and there can be 1 or many
 * LEDs. 
 */

#ifndef LED_H
#define LED_H

#include "user_led.h"
#include "sequence.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Holds state information for an led's configuration.
 * 
 * @param id        - A unique identifier for an led. 
 * @param enabled   - True if led is in use, false otherwise.
 * @param sequence  - A pointer to a sequence object that defines the flash pattern of the led.
 * @param pinout    - A pinout object that defines connection of the led to the microcontroller.
 */
typedef struct {
    uint32_t id;
    bool enabled;
    pins_t pinout;
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
 * @brief For describing execution status of function.
 */
typedef enum{
    LED_OK,
    LED_ERR
}led_status_t;


typedef void (*write_pin)(pins_t, led_state_t);

/**
 * @brief The inialisation for the led driver, sets the 'write_fcn' that the driver uses to contorl the leds and the callback 
 * frequency that led_update will be called 
 * @param [IN] callback_frequency - How often the led_update function will be called 
*/
void led_init(uint32_t callback_frequency);

/**
 * @brief Return the number of registered LEDs in the led module.
 * 
 * @returns uint16 - the number of registered LEDs.
 * 
 */
uint32_t led_get_count();

/**
 * @brief turn on selected LED
 *
 * @param [IN] id - ID of LED to be turned on
*/
void led_on(uint32_t id);

/**
 * @brief turn off selected LED
 *
 * @param [IN] id - ID of LED to be turned off
*/
void led_off(uint32_t id);

/**
 * @brief Register an LED and its configurations with the LED module.
 * 
 * @param [IN] led_obj - a structure of type led_t that has the configurations for the LED
 * 
 * @return Error if failed to register.
 */
led_status_t led_register(led_t led_obj);

/**
 * @brief User defined function that turns led's on and off 
 * 
 * @param [IN] pins - The pin to be written to 
 * @param [IN] state - The state to be written to the input pin 
 */
void user_write_pin(pins_t pins, led_state_t state);

/**
 * @brief disable selected LED
 *
 * @param [IN] id - ID of LED to be disabled 
*/
void led_disable(uint32_t id);

/**
 * @brief enable selected LED
 *
 * @param [IN] id - ID of LED to be disabled 
*/
void led_enable(uint32_t id);

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