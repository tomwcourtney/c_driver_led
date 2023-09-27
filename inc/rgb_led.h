/**
 * @file rgb_led.h
 * @brief A wrapper around the led.h that allows the definition and use of RGB leds
 */


#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>



/**
 * @brief The inialisation for the rgb led wrapper. Initialization the state of all of the LEDs in the LED array and creates
 * some special sequences like white, red, blue and green ect...
 * @note The led_init from led.h will need to be called also before registering any rgb leds
*/
void rgb_led_init();

/**
 * @brief Return the number of registered RGB LEDs in the rgb_led module.
 * 
 * @returns uint16 - the number of registered LEDs.
 * 
 */
uint32_t rgb_led_get_count();


/**
 * Functions will be ported as nessessary from sequence and led
 * 
 * Interface rgb_led  
 *  rgb_led_on(uint32 hexi_colour_code)
 *  rgb_led_off(void)
 *  int32_t rgb_led_id = rgb_led_register(pins_t red_pin, pins_t green_pin, pins_t blue_pin, led_t rgb_led_settings)
 
 *  typedef struct {
 *      int32_t led_id_red
 *      int32_t led_id_blue
 *      int32_t led_id_green
 *  } rgb_led_t
 * 
 * Interface rgb_sequence 
 * uint32_t rgb_sequence_get_count()
 * rgb_sequence_register(sequence_t _sequence)
 * bool rgb_sequence_exists(uint32_t rgb_sequence_id)
 * 
 * uint32_t colourSequence[] = {WHITE, GREEN, 0x002B9B64, 0x009B2B2B}; // Uses hexi colour codes but since we have 32 bits the first byte is 00, we can use the spare byte for brightness scaling in future 
 * typedef struct{
 *     uint32_t colourSequence[MAX_SEQUENCE];
 *     uint8_t length;
 *     uint32_t period;
 * }rgb_sequence_t;
 * 
*/

#endif