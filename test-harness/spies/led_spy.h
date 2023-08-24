#ifndef LED_SPY_H
#define LED_SPY_H

// led set on
// led set off
// led get state
#include "../../src/led.h"
#include <stdint.h>

#define MAX_LEDS 64

void led_spy_init(void);

led_state_t led_spy_get_state(uint32_t id);

led_state_t led_spy_set_state(uint32_t id, led_state_t);


#endif
