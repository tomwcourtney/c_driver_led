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


led_status_t led_assign_sequence(uint32_t led_id, uint32_t sequence_id)
{
    // Check if LED exists
    if(!led_exists(led_id))
    {
        return LED_ERR;
    }

    // Check if sequence exists
    if(!sequence_exists(sequence_id))
    {
        return LED_ERR;
    }

    // Assign sequence to LED
    if(led_id == led.id)
    {
        led.sequence_id = sequence_id;
    }

    return LED_OK;
}

uint32_t led_get_sequence(uint32_t led_id)
{
    if(led_id == led.id)
    {
        return led.sequence_id;
    }
    return -1; 
}

led_status_t led_start_sequence(uint32_t led_id)
{
    return LED_OK;
}

bool led_exists(uint32_t led_id)
{
    if(led.id == led_id)
    {
        return true;
    }
    return false; 
}