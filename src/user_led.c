
#include "../test-harness/spies/led_spy.h"


// #include "user_led.h"
#include "led.h"
#include <stdint.h>

void user_write_pin(pins_t pins, led_state_t state)
{
    led_spy_set_state(pins.pin, state);
}