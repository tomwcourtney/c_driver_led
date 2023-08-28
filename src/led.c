#include "led.h"
#include <string.h>

write_pin write(pins_t, led_state_t);

led_t led = {-1};

static uint32_t count = 0;

void led_init(uint32_t callback_frequency)
{
    sequence_init();
    memset(&led, -1, sizeof(led_t));
    count = 0;
    return;
}

uint32_t led_get_count()
{
    return count;
}

void led_on(uint32_t id)
{
    if (count <= 0)
    {
        return;
    }
    
    if(led.enabled)
    {
        user_write_pin(led.pinout, LED_ON);
    }
}

void led_off(uint32_t id)
{
    if (count <= 0)
    {
        return;
    }
    
    user_write_pin(led.pinout, LED_OFF);
}

led_status_t led_register(led_t led_obj)
{
    if (led.id == led_obj.id)
    {
        return LED_ERR;
    }

    led = led_obj;
    count++;

    return LED_OK;
}

void led_disable(uint32_t id)
{
    led.enabled = false; 
}

void led_enable(uint32_t id)
{
    led.enabled = true; 
}