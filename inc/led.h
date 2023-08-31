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
 * @param id            - A unique identifier for an led. 
 * @param enabled       - True if led is in use, false otherwise.
 * @param pinout        - A pinout object that defines connection of the led to the microcontroller.
 * @param sequence_id   - A pointer to a sequence object that defines the flash pattern of the led.
 * @param sequence_idx  - Maintains the position in the assigned sequence that the LED is up to
 * @param timer_count   - Maintains the acrued time since the last sequence index increment
 */
typedef struct {
    uint32_t id;
    bool enabled;
    pins_t pinout;
    uint32_t sequence_id;
    uint8_t sequence_idx;
    uint32_t timer_count; 

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


// typedef void (*write_pin)(pins_t, led_state_t);

void write(pins_t, led_state_t);

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

/**
 * @brief Assigns a sequence to an LED
 *
 * @param [IN] led_id - the id of the led to be assigned to 
 * @param [IN] sequence_id - the id of the sequence to be assinged to the led 
 * 
 * @return led_status_t - err if the led or sequence doesn't exist.
*/
led_status_t led_assign_sequence(uint32_t led_id, uint32_t sequence_id);

/**
 * @brief Returns the current sequence assinged to that led, returns -1 if there is no sequence assigned 
 *
 * @param [IN] led_id - the ID of the led to check 
 * 
 * @return uint32_t - the ID of the assigned sequence. -1 if no sequence assigned.
*/
uint32_t led_get_sequence(uint32_t led_id);

/**
 * @brief Checks if a led is registered.
 * 
 * @return bool - Returns true if led exists.
 */
bool led_exists(uint32_t led_id);

/**
 * @brief Turns on the sequence enable boolean for the selected LED.
 * 
 * @param led_id - The led to start running the sequence of.
 * @return led_status_t - Err if the LED isn't registered.
 */
led_status_t led_start_sequence(uint32_t led_id);

/**
 * @brief Turns on the sequence enable boolean for the selected LED.
 * 
 * @param led_id - The led to start running the sequence of.
 * @return bool - returns the led 
 */
bool led_is_enabled(uint32_t led_id);

/**
 * @brief Is called whenever you want to update the state of your LEDs
 * according to their sequence. Will update all LEDs.
 */
void led_timer_step();


/* INTERFACE 

   led_start_sequence() //Starts the current sequence that the leds are configured for 
   led_stop_sequence()  //Stops the current sequence that the leds are configured for 
   
   led_on(led_id)
   led_off(led_id)
   
   led_all_on()
   led_all_off()
*/

#endif