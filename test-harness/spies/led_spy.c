#include "led_spy.h"
#include <stdbool.h>
#include <stdio.h>

led_state_t led_states[MAX_LEDS] = {0};


void led_spy_init(void)
{
    for (int i = 0; i < MAX_LEDS; i++)
    {
        led_states[i] = LED_UNDEFINED;
    }
}

led_state_t led_spy_get_state(uint32_t id)
{
    if (id >= MAX_LEDS)
    {
        return LED_UNDEFINED;
    }
    return led_states[id];
}

led_state_t led_spy_set_state(uint32_t id, led_state_t state)
{
    
    if (id >= MAX_LEDS)
    {
        return LED_UNDEFINED;
    }

    led_states[id] = state;
    return led_states[id];
}