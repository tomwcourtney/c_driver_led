
#include "../test-harness/spies/led_spy.h"


#include "led.h"
#include <stdint.h>
#include "main.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim21;
extern TIM_HandleTypeDef htim22;
void write(pins_t pins, led_state_t state)
{
//    led_spy_set_state(pins.pin, state);
	if(pins.pin == 0) // red
	{
		TIM2->CCR3 = state;
	}
	else if (pins.pin == 1) // green
	{
		TIM22->CCR2 = state;
	}
	else if (pins.pin == 2) // blue
	{
		TIM22->CCR1 = state;
	}
}
