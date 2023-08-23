#ifndef LED_SPY_H
#define LED_SPY_H

// led set on
// led set off
// led get state

#include <stdint.h>

typedef enum 
{
    LED_STATE_UNDEFINED = -1,
    LED_STATE_ON,
    LED_STATE_OFF
} led_state_t;



void led_spy_init(void);

led_state_t led_spy_get_state(uint32_t id);

led_state_t led_spy_set_state(uint32_t id, led_state_t);


#endif
