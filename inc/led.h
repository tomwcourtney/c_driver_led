/**
 * @file led.h
 * @brief A driver for LEDs. They can be RGB or not and there can be 1 or many
 * LEDs. 
 */

#ifndef LED_H
#define LED_H

#include "user_led.h"
#include "sequence.h"

#include <stdint.h>
#include <stdbool.h>

#define LEDS_MAX 64

/**
 * @brief Holds state information for an led's configuration.
 * 
 * @param enabled       - True if led is in use, false otherwise.
 * @param pinout        - A pinout object that defines connection of the led to the microcontroller.
 * @param sequence_id   - A pointer to a sequence object that defines the flash pattern of the led.
 * @param sequence_idx  - Maintains the position in the assigned sequence that the LED is up to
 * @param timer_count   - Maintains the acrued time since the last sequence index increment
 * @param sequence_initialized - if true then the led has been set to the first state in the sequence.
 */

typedef struct {
    bool enabled;
    pins_t pinout;
    int32_t sequence_id;
    uint8_t sequence_idx;
    uint32_t timer_count; 
    bool sequence_initialized;
}led_t;

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

/* User defined hardware layer function that changes the LED state on the target device */
void write(pins_t, led_state_t);

/**
 * @brief The inialisation for the led driver. Initialization the state of all of the LEDs in the LED array and creates
 * some special sequences like on and off.
 * 
 * @param [in] callback_frequency - How often the led_update function will be called in milliseconds.
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
 * @param [in] id - ID of LED to be turned on
*/
void led_on(int32_t id);

/**
 * @brief turn off selected LED
 *
 * @param [in] id - ID of LED to be turned off
*/
void led_off(int32_t id);

/**
 * @brief Register an LED and its configurations with the LED module.
 * 
 * @param [in] led_obj - a structure of type led_t that has the configurations for the LED
 * 
 * @return Error if failed to register, led ID otherwise
 */
int32_t led_register(led_t led_obj);

/**
 * @brief User defined function that turns led's on and off 
 * 
 * @param [in] pins - The pin to be written to 
 * @param [in] state - The state to be written to the input pin 
 */
void user_write_pin(pins_t pins, led_state_t state);

/**
 * @brief disable selected LED
 *
 * @param [in] id - ID of LED to be disabled 
*/
void led_disable(int32_t id);

/**
 * @brief enable selected LED
 *
 * @param [in] id - ID of LED to be disabled 
*/
void led_enable(int32_t id);

/**
 * @brief Assigns a sequence to an LED
 *
 * @param [in] led_id - the id of the led to be assigned to 
 * @param [in] sequence_id - the id of the sequence to be assinged to the led 
 * 
 * @return led_status_t - err if the led or sequence doesn't exist.
*/
led_status_t led_assign_sequence(int32_t led_id, int32_t sequence_id);

/**
 * @brief Returns the current sequence assinged to that led, returns -1 if there is no sequence assigned 
 *
 * @param [in] led_id - the ID of the led to check 
 * 
 * @return uint32_t - the ID of the assigned sequence. -1 if no sequence assigned.
*/
int32_t led_get_sequence_id(int32_t led_id);

/**
 * @brief Checks if a led is registered.
 * 
 * @return bool - Returns true if led exists.
 */
bool led_exists(int32_t led_id);

/**
 * @brief Turns on the sequence enable boolean for the selected LED.
 * 
 * @param led_id - The led to start running the sequence of.
 * @return led_status_t - Err if the LED isn't registered.
 */
led_status_t led_start_sequence(int32_t led_id);

/**
 * @brief Is called whenever you want to update the state of your LEDs
 * according to their sequence. Will update all LEDs.
 */
void led_update_state();

/**
 * @brief Turns on the specified LED
 * 
 * @param led_id - unique identifier of the target led.
 */
void led_turn_on(int32_t led_id);

/**
 * @brief Turns off the specified LED
 * 
 * @param led_id - unique identifier of the target led.
 */
void led_turn_off(int32_t led_id);

/**
 * @brief Returns the led if registed based off the inputted id
 * 
 * @param sequence_id - index of the led in the list of sequences.
 * 
 * @return led_t * - Returns pointer to object found. Else return NULL.
 */
 led_t * led_get_from_id(uint32_t led_id);

/**
 * @brief Allows the user offset patterns on the fly, works by chaing the current sequence index
 * at the inputed value 
 * @param led_id - unique identifier of the target led.
 * @param seq_offset - amout to offset the sequence_idx in the led's structure
 */
void led_offset_sequence(uint32_t led_id, uint8_t seq_offset);


#endif