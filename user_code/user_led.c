
#include "../test-harness/spies/led_spy.h"


#include "led.h"
#include <stdint.h>

void write(pins_t pins, led_state_t state)
{
    led_spy_set_state(pins.pin, state);
}
