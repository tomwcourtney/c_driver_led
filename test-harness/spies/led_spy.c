#include "led_spy.h"
#include <stdbool.h>
#include <stdio.h>

led_state_t led_states[LEDS_MAX] = {0};

void led_spy_init(void)
{
    for (int i = 0; i < LEDS_MAX; i++)
    {
        led_states[i] = LED_UNDEFINED;
    }
}

led_state_t led_spy_get_state(int32_t id)
{
    if (id >= LEDS_MAX)
    {
        return LED_UNDEFINED;
    }
    
    return led_states[id];
}

led_state_t led_spy_set_state(int32_t id, led_state_t state)
{
    if (id >= LEDS_MAX)
    {
        return LED_UNDEFINED;
    }

    led_states[id] = state;
    return led_states[id];
}