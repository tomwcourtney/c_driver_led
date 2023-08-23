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
 * @param on        - True if led should be on, false otherwise.
 * @param sequence  - A pointer to a sequence object that defines the flash pattern of the led.
 * @param pinout    - A pointer to a pinout object that defines connection of the led to the microcontroller.
 */
typedef struct {
    uint32_t id;
    bool on;
} led_t;




/* INTERFACE 
   led_init(write_fcn, callback_frequency) 
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