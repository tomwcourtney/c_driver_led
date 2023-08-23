#include "led_spy.h"

#include <stdbool.h>

led_state_t led_state = LED_STATE_UNDEFINED;

void led_spy_init(void)
{
    led_state = LED_STATE_UNDEFINED;
}

led_state_t led_spy_get_state(uint32_t id)
{
    return led_state;
}

led_state_t led_spy_set_state(uint32_t id, led_state_t state)
{
    led_state = state;
    return led_state;
}