#include "rgb_led.h"
#include <string.h>
rgb_led_t rgbLed = {0};
rgb_sequence_t rgbSeq = {0};

static uint32_t rgb_led_count = 0; /** Number of registered RGB Leds*/
static uint32_t rgb_seq_count = 0; /** Number of registered RGB sequences*/


void rgb_led_init()
{
    rgb_led_count = 0;
    rgb_seq_count = 0;

    // Create RGB_WHITE Sequence 
    uint32_t seq[1] = {C_WHITE};
    rgb_sequence_register(1, 1, seq);
}


uint32_t rgb_led_get_count()
{
    return rgb_led_count;
    
}


void rgb_led_on(int32_t id, int32_t colourCode)
{
    led_assign_sequence(rgbLed.led_id_red, rgbSeq.seq_id_red);
    led_assign_sequence(rgbLed.led_id_green, rgbSeq.seq_id_green);
    led_assign_sequence(rgbLed.led_id_blue, rgbSeq.seq_id_blue);
}

int32_t rgb_led_register(pins_t red_pin, pins_t green_pin, pins_t blue_pin, led_t led_obj)
{
    // Register LEDS with led Module 
    // R
    led_obj.pinout = red_pin;
    rgbLed.led_id_red = led_register(led_obj);
    // G
    led_obj.pinout = green_pin;
    rgbLed.led_id_green = led_register(led_obj);
    // B
    led_obj.pinout = blue_pin;
    rgbLed.led_id_blue = led_register(led_obj);

    return rgb_led_count++;
}

void rgb_sequence_get_ids_from_id(uint32_t rgbSequenceId, uint32_t * redSequenceId, uint32_t * greenSequenceId, uint32_t * blueSequenceId)
{

    *redSequenceId   = rgbSeq.seq_id_red;
    *greenSequenceId = rgbSeq.seq_id_green;
    *blueSequenceId  = rgbSeq.seq_id_blue;

}

int32_t rgb_sequence_register(uint8_t length, uint16_t period, uint32_t * rgbSequence)
{
    // Turn RGB sequence into 3 seperate channels through byte manipulation 
    sequence_t sequenceTemp = {
        .length = length,
        .period = period,
        };

    // Seperating Sequences 
    uint8_t redSequence[MAX_SEQUENCE] = {0};
    uint8_t grnSequence[MAX_SEQUENCE] = {0};
    uint8_t bluSequence[MAX_SEQUENCE] = {0};
    // Adding in Colours 
    redSequence[0] = (rgbSequence[0] >> 16) & 0xFF;
    grnSequence[0] = (rgbSequence[0] >> 8) & 0xFF;
    bluSequence[0] = rgbSequence[0] & 0xFF;
    // Register 3 Patterns 
    memcpy(sequenceTemp.sequence, redSequence, length);
    rgbSeq.seq_id_red   = sequence_register(sequenceTemp);
    memcpy(sequenceTemp.sequence, grnSequence, length);
    rgbSeq.seq_id_green = sequence_register(sequenceTemp);
    memcpy(sequenceTemp.sequence, bluSequence, length);
    rgbSeq.seq_id_blue  = sequence_register(sequenceTemp);
    // Return the rgb sequence ID 
    return rgb_seq_count ++;
}

uint32_t rgb_sequence_get_count()
{
    return rgb_seq_count;
}