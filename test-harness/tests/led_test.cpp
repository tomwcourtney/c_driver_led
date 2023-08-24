#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../../src/led.h"
    #include "../spies/led_spy.h"
}


TEST_GROUP(LEDTest)
{
    void setup()
    {
        led_init(NULL, 10);
    }

    void teardown()
    {
    }
};



/* ZERO
    - After init there is no LED registed
*/
TEST(LEDTest, init_led_driver_has_no_registered_led)
{
    CHECK(led_get_led_count()==0);
}

// turning on an LED that isn't registered does not turn on that LED
TEST(LEDTest, turn_on_led_nothing_registered_led_doesnt_turn_on)
{
    uint32_t id = 0;
    led_on(id);

    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(id));
}


/* ONE

*/

/* MANY

*/

