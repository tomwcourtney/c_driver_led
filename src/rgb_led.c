#include "rgb_led.h"
#include <string.h>
#include <stdio.h>

rgb_led_t rgbLeds[LEDS_MAX] = {0};
rgb_sequence_t rgbSequences[MAX_SEQUENCES] = {0};

static int32_t rgb_led_count = 0; /** Number of registered RGB Leds*/
static int32_t rgb_seq_count = 0; /** Number of registered RGB sequences*/

/**
 * @brief Predefined hexidecimal colour codes to be used for sequence creation
 */
typedef enum
{
    C_WHITE = 0xFFFFFF,
    C_RED = 0xFF0000,
    C_GREEN = 0x00FF00,
    C_BLUE = 0x0000FF,
    C_OFF = 0x000000,
} rgb_colour_t;

void rgb_led_init()
{
    rgb_led_count = 0;
    rgb_seq_count = 0;
    // Registering base Sequences 
    uint32_t seq[1] = {C_WHITE};
    rgb_sequence_register(1, 1, seq);
    seq[0] = C_RED;
    rgb_sequence_register(1, 1, seq);
    seq[0] = C_GREEN;
    rgb_sequence_register(1, 1, seq);
    seq[0] = C_BLUE;
    rgb_sequence_register(1, 1, seq);
    seq[0] = C_OFF;
    rgb_sequence_register(1, 1, seq);
}


uint32_t rgb_led_get_count()
{
    return rgb_led_count;  
}

led_status_t rgb_led_on(int32_t id, rgb_state_t existing_sequence)
{
    return rgb_assign_sequence(id,existing_sequence);
}


int32_t rgb_led_register(pins_t red_pin, pins_t green_pin, pins_t blue_pin, led_t led_obj)
{
    // Check there is enough led space to register rgb led
    if((LEDS_MAX-led_get_count()) < 3)
    {
        return -1;
    }
    // Register LEDS with led Module 
    // R
    led_obj.pinout = red_pin;
    rgbLeds[rgb_led_count].led_id_red = led_register(led_obj);
    // G
    led_obj.pinout = green_pin;
    rgbLeds[rgb_led_count].led_id_green = led_register(led_obj);
    // B
    led_obj.pinout = blue_pin;
    rgbLeds[rgb_led_count].led_id_blue = led_register(led_obj);

    return rgb_led_count++;
}

void rgb_sequence_get_ids_from_id(int32_t rgbSequenceId, int32_t * redSequenceId, int32_t * greenSequenceId, int32_t * blueSequenceId)
{
    *redSequenceId   = rgbSequences[rgbSequenceId].seq_id_red;
    *greenSequenceId = rgbSequences[rgbSequenceId].seq_id_green;
    *blueSequenceId  = rgbSequences[rgbSequenceId].seq_id_blue;
}

int32_t rgb_sequence_register(uint8_t length, uint16_t period, uint32_t * rgbSequence)
{
    // Check there is enough space for RGB sequence 
    if((MAX_SEQUENCES-sequence_get_count()) < 3)
    {
        return -1;
    }
    
    // Turn RGB sequence into 3 seperate channels through byte manipulation 
    sequence_t sequenceTemp = {
        .length = length,
        .period = period,
        };

    // Creating colour channels 
    uint8_t redSequence[MAX_SEQUENCE] = {0};
    uint8_t grnSequence[MAX_SEQUENCE] = {0};
    uint8_t bluSequence[MAX_SEQUENCE] = {0};
    // Adding in Colours 
    for(int _iter = 0; _iter <length; _iter ++)
    {
        redSequence[_iter] = (rgbSequence[_iter] >> 16) & 0xFF;
        grnSequence[_iter] = (rgbSequence[_iter] >> 8) & 0xFF;
        bluSequence[_iter] = rgbSequence[_iter] & 0xFF;
    }
    
    // Register 3 Patterns 
    memcpy(sequenceTemp.sequence, redSequence, length);
    rgbSequences[rgb_seq_count].seq_id_red   = sequence_register(sequenceTemp);
    memcpy(sequenceTemp.sequence, grnSequence, length);
    rgbSequences[rgb_seq_count].seq_id_green = sequence_register(sequenceTemp);
    memcpy(sequenceTemp.sequence, bluSequence, length);
    rgbSequences[rgb_seq_count].seq_id_blue  = sequence_register(sequenceTemp);
    // Return the rgb sequence ID 
    return rgb_seq_count ++;
}

uint32_t rgb_sequence_get_count()
{
    return rgb_seq_count;
}


led_status_t rgb_assign_sequence(int32_t rgb_led_id, int32_t rgb_sequence_id)
{
    // Check sequence exists 
    if(!rgb_sequence_exists(rgb_sequence_id))
    {
        return LED_ERR; 
    }

    // Check led exists 
    if(!rgb_led_exists(rgb_led_id))
    {
        return LED_ERR; 
    }

    led_status_t redStatus   = led_assign_sequence(rgbLeds[rgb_led_id].led_id_red, rgbSequences[rgb_sequence_id].seq_id_red);
    led_status_t blueStatus  = led_assign_sequence(rgbLeds[rgb_led_id].led_id_green, rgbSequences[rgb_sequence_id].seq_id_green);
    led_status_t greenStatus = led_assign_sequence(rgbLeds[rgb_led_id].led_id_blue, rgbSequences[rgb_sequence_id].seq_id_blue);

    // Check status variables 
    if(!redStatus && !blueStatus && !greenStatus)
    {
        return LED_OK;
    }
    return LED_ERR;
}

void rgb_led_get_ids_from_id(int32_t rgbLedId, int32_t * redLedId, int32_t * greenLedId, int32_t * blueLedId)
{
    *redLedId   = rgbLeds[rgbLedId].led_id_red;
    *greenLedId = rgbLeds[rgbLedId].led_id_green;
    *blueLedId  = rgbLeds[rgbLedId].led_id_blue;
}

bool rgb_sequence_exists(int32_t rgb_sequence_id)
{
    return rgb_sequence_id < rgb_seq_count;
}

bool rgb_led_exists(int32_t rgb_led_id)
{
    return rgb_led_id < rgb_led_count;
}
