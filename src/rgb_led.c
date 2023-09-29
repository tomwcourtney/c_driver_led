#include "rgb_led.h"

rgb_led_t rgbLed = {0};

static uint32_t rgb_led_count = 0; /** Number of registered RGB Leds*/
static uint32_t rgb_seq_count = 0; /** Number of registered RGB sequences*/


void rgb_led_init()
{
    rgb_led_count = 0;
    rgb_seq_count = 0;

}


uint32_t rgb_led_get_count()
{
    return rgb_led_count;
    
}


void rgb_led_on(int32_t id, int32_t colourCode)
{

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

}

int32_t rgb_sequence_register(uint8_t length, uint16_t period, uint32_t * rgbSequence)
{
    // Turn RGB sequence into 3 seperate channels through byte manipulation 

    // Register 3 Patterns 

    // Return the rgb sequence ID 
    return 0;
}

uint32_t rgb_sequence_get_count()
{
    return rgb_seq_count;
}