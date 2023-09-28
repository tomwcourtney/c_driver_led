#include "rgb_led.h"

rgb_led_t rgbLed = {0};
// This is the number of LEDs in the array.
static uint32_t count = 0;

void rgb_led_init()
{
    count = 0;
}


uint32_t rgb_led_get_count()
{
    return count;
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

    return count++;
}