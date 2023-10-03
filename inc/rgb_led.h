/**
 * @file rgb_led.h
 * @brief A wrapper around the led.h and sequence.h that allows the definition and use of RGB leds as single units
 */


#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>
#include "led.h"

/**
 * @brief Holds state information for an rgb led's configuration.
 */
 typedef struct {
     int32_t led_id_red;   /**Led ID for the red led. */
     int32_t led_id_green; /**Led ID for the green led. */
     int32_t led_id_blue;  /**Led ID for the blue led. */
 } rgb_led_t;

 /**
 * @brief Holds state information for an rgb sequence 
 */
 typedef struct {
     int32_t seq_id_red;   /**Sequence ID for the red led. */
     int32_t seq_id_green; /**Sequence ID for the green led. */
     int32_t seq_id_blue;  /**Sequence ID for the blue led. */
 } rgb_sequence_t;

/**
 * @brief Predefined hexidecimal colour codes to be used for sequence creation 
 */
typedef enum {
    C_WHITE  = 0xFFFFFF,
    C_RED    = 0xFF0000,
    C_GRN    = 0x00FF00,
    C_BLUE   = 0x0000FF,
} rgb_colour_t;

/** @brief On init defined RGB patterns to be used to turn LED solid colours  
 * 
*/
typedef enum {
    RGB_WHITE,
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE, 
    RGB_OFF, 
} rgb_state_t;


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
 * @brief Return the number of registered RGB sequences
 * 
 * @returns uint16 - the number of registered LEDs.
 * 
 */
uint32_t rgb_sequence_get_count();

/**
 * @brief Changes the RGB led to the colour specified 
 * @param [in] id - the rgb led id 
 * @param [in] colourCode - Hexidecimal colour code or a predefined code defined in rgb_colour_t
*/
void rgb_led_on(int32_t id, int32_t colourCode);

/**
 * @brief Registeres a new RGB led inside of rgb_led.h, this in turn registeres 3 LEDS in led.h, 
 * @param [in] red_pin - the pwm pin connected to the red led 
 * @param [in] grren_pin - the pwm pin connected to the green led 
 * @param [in] blue_pin - the pwm pin connected to the blue led 
 * @param [in] led_obj  - a structure of type led_t that has the configurations that will be written to all leds, the pinout field can be ignored
 *
 * @return Error if failed to register, rgb led ID otherwise
*/
int32_t rgb_led_register(pins_t red_pin, pins_t green_pin, pins_t blue_pin, led_t led_obj);

/**
 * @brief Creates and registers an RGB sequence that can be assinged to RGB led's
 * @param [in] length - Amount of elements in rgbSequence  
 * @param [in] period - Length of time [ms] between the start and end of sequence 
 * @param [in] rgbSequence - Array of bit hexidecimal colour codes
 *
 * @return int32_t - If successfully registered returns the ID of the sequence. If error returns -1
*/
int32_t rgb_sequence_register(uint8_t length,uint16_t period,uint32_t * rgbSequence);

/**
 * @brief Returns the id's for all associated sequence ids for a defined RGB sequence 
 * @param [in] rgbSequenceId - RGB sequence id to fetch associated sequence id's for 
 * @param [in] redSequenceId - red channel sequence id
 * @param [in] greenSequenceId - green channel sequnce id
 * @param [in] blueSequenceId - blue channel sequence id 
 *
*/
void rgb_sequence_get_ids_from_id(int32_t rgbSequenceId, int32_t * redSequenceId, int32_t * greenSequenceId, int32_t * blueSequenceId);

/**
 * @brief Assigns an RGB sequence to an RGB led 
 *
 * @param [in] rgb_led_id - the id of the RGB led to be assigned to 
 * @param [in] rgb_sequence_id - the id of the rgb sequence to be assinged to the led 
 * 
 * @return led_status_t - err if the led or sequence doesn't exist.
*/
led_status_t rgb_assign_sequence(int32_t rgb_led_id,int32_t rgb_sequence_id);

/**
 * @brief Returns the id's for all associated led ids for a defined RGB led 
 * @param [in] rgbLedId - RGB led id to fetch associated led id's for 
 * @param [in] redLedId - red channel led id
 * @param [in] greenLedId - green channel sequnce id
 * @param [in] blueLedId - blue channel led id 
 *
*/
void rgb_led_get_ids_from_id(int32_t rgbLedId, int32_t * redLedId, int32_t * greenLedId, int32_t * blueLedId);

/**
 * Functions will be ported as nessessary from sequence and led
 * 
 * Interface rgb_led  
 *  
 *  rgb_led_off(void)
 


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