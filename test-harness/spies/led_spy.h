#ifndef LED_SPY_H
#define LED_SPY_H

// led set on
// led set off
// led get state 
#include "../../inc/led.h"
#include <stdint.h>

#define DRIVER_TEST

#define IS_LED_ON(id)\
        LONGS_EQUAL(LED_ON, led_spy_get_state(id));

#define IS_LED_OFF(id)\
        LONGS_EQUAL(LED_OFF, led_spy_get_state(id));

#define IS_LED_UNDEFINED(id)\
        LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(id));



void led_spy_init(void);

led_state_t led_spy_get_state(int32_t id);
led_state_t led_spy_set_state(int32_t id, led_state_t);


#endif
