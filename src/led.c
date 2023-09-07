#include "led.h"
#include <string.h>
#include <stdio.h>

led_t led = {-1};

led_t leds[LEDS_MAX] = {{-1}};

static uint32_t count = 0;
static uint32_t timer_period = 0;

void led_init(uint32_t _timer_period)
{
    sequence_init();

    for(int i = 0; i < LEDS_MAX; i++)
    {
        memset(&(leds[i]), -1, sizeof(led_t));
    }

    count = 0;
    timer_period = _timer_period;


    // Create the "off sequence"
    sequence_t sequence_off =
    {
        .length = 1,
        .period = 1,
        .sequence = {LED_OFF}
    };
    sequence_register(sequence_off);
    // Create the "on sequence"
    sequence_t sequence_on =
    {
        .length = 1,
        .period = 1,
        .sequence = {LED_ON}
    };
    sequence_register(sequence_on);


    return;
}

uint32_t led_get_count()
{
    return count;
}

void led_on(int32_t id)
{
    if (count <= 0)
    {
        return;
    }

    if(leds[id].enabled)
    {
        write(leds[id].pinout, LED_ON);
    }
}

void led_off(int32_t id)
{
    if (count <= 0)
    {
        return;
    }
    
    write(leds[id].pinout, LED_OFF);
}

int32_t led_register(led_t led_obj)
{
    leds[count] = led_obj;

    return count++;
}

void led_disable(int32_t id)
{
    leds[id].enabled = false; 
}

void led_enable(int32_t id)
{
    leds[id].enabled = true; 
}


led_status_t led_assign_sequence(int32_t led_id, int32_t sequence_id)
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

    leds[led_id].sequence_id = sequence_id;

    leds[led_id].sequence_idx = 0;
    leds[led_id].timer_count = 0;

    return LED_OK;
}

int32_t led_get_sequence_id(int32_t led_id)
{
    // if(led_id == led.id)
    // {
    //     return led.sequence_id;
    // }
    return leds[led_id].sequence_id;
    //return -1; 
}

bool led_exists(int32_t led_id)
{
    return led_id < count;
}

void led_timer_step()
{
    // uint32_t sequence_id = led_get_sequence_id(led.id);
    for (int i = 0; i < count; i++)
    {
        uint32_t sequence_id = leds[i].sequence_id;

        if (!sequence_exists(sequence_id))
        {
            continue;
        }

        sequence_t * sequence = sequence_get_from_id(sequence_id);
        
        float thresh = sequence->period/sequence->length;
        
        leds[i].timer_count += timer_period;
        
        if (leds[i].timer_count >= thresh)
        {
            leds[i].sequence_idx += 1; 

            if(leds[i].sequence_idx > (sequence->length-1))
            {
                leds[i].sequence_idx = 0;
            }
                    
            leds[i].timer_count = leds[i].timer_count - thresh;
        }

        
        if(leds[i].enabled)
        {
            write(leds[i].pinout, sequence->sequence[leds[i].sequence_idx]);
            continue;
        }
    }
}


void led_turn_on(int32_t led_id)
{
    led_assign_sequence(led_id, 1); 
}

void led_turn_off(int32_t led_id)
{
    led_assign_sequence(led_id, 0); 
}

void led_print(int32_t id)
{
    printf( "id: %d\n"
            "enabled: %d\n"
            "pinout: ..\n"
            "sequence_id: %d\n"
            "sequence_idx: %d\n"
            "timer_count: %d\n", id, leds[id].enabled, leds[id].sequence_id, leds[id].sequence_idx, leds[id].timer_count);
}

led_t * led_get_from_id(uint32_t led_id)
{
    if (!led_exists(led_id))
    {
        return NULL;
    }
    return &(leds[led_id]);
}