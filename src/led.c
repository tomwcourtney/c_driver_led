#include "led.h"
#include <string.h>
#include <stdio.h>

led_t led = {-1};

static uint32_t count = 0;
static uint32_t timer_period = 0;



void led_init(uint32_t _timer_period)
{
    sequence_init();
    memset(&led, -1, sizeof(led_t));
    count = 0;
    timer_period = _timer_period;
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
        write(led.pinout, LED_ON);
    }
}

void led_off(uint32_t id)
{
    if (count <= 0)
    {
        return;
    }
    
    write(led.pinout, LED_OFF);
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

    led.sequence_idx = 0;
    led.timer_count = 0;

    return LED_OK;
}

uint32_t led_get_sequence_id(uint32_t led_id)
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

void led_timer_step()
{
    // uint32_t sequence_id = led_get_sequence_id(led.id);
    uint32_t sequence_id = led.sequence_id;
    
    if (!sequence_exists(sequence_id))
    {
        return;
    }

    sequence_t * sequence = sequence_get_from_id(sequence_id);
    
    float thresh = sequence->period/sequence->length;
    
    led.timer_count += timer_period;
    
    if (led.timer_count >= thresh)
    {
        led.sequence_idx += 1; 

        if(led.sequence_idx > (sequence->length-1))
        {
            led.sequence_idx = 0;
        }
                   
        led.timer_count = led.timer_count - thresh;
    }

    
    if(led.enabled)
    {
        write(led.pinout, sequence->sequence[led.sequence_idx]);
        return;
    }

    return;
}

void led_turn_on(uint32_t led_id)
{
    // Set the sequence to "on sequence"
    sequence_t sequence =
    {
        .length = 1,
        .period = 1,
        .sequence = {LED_ON}
    };

    led_assign_sequence(led_id, sequence_register(sequence)); 
}

void led_turn_off(uint32_t led_id)
{
    // Set the sequence to "on sequence"
    sequence_t sequence =
    {
        .length = 1,
        .period = 1,
        .sequence = {LED_OFF}
    };
    led_assign_sequence(led_id, sequence_register(sequence)); 
}
