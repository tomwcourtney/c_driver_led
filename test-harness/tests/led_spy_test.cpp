#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../spies/led_spy.h"
}


TEST_GROUP(LEDSpyTest)
{
    void setup()
    {
        led_spy_init();
    }

    void teardown()
    {
    }
};


/* ZERO
*/

// After spy initialisation, the spy LED state is undefined
TEST(LEDSpyTest, led_undefined_after_initialisation)
{
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(0));
}

// // Calling led_spy_set_state(0xfff,ON) creates an error that can be read from led_spy_get_error(), (gives us a way to check if an id is passed in incorrectly)
// TEST(LEDSpyTest, writing_to_undefied_led_returns_error)
// {
//     // Write to led that does not exist
//     led_spy_set_state(0xfffff, ON);
//     // Check that the UNDEFINED_LED_ERROR is returned 
//     LONGS_EQUAL(UNDEFINED_LED_ERROR, led_spy_get_error());
    
// }




/* ONE
*/

// Calling led_spy_set_state(ON) causes the LED to be turned on
TEST(LEDSpyTest, led_turn_on)
{
    led_spy_set_state(0,LED_ON);
    LONGS_EQUAL(LED_ON, led_spy_get_state(0));

}

// Turn off an LED, LED state is OFF
TEST(LEDSpyTest, led_turned_off)
{
    led_spy_set_state(0, LED_OFF);
    LONGS_EQUAL(LED_OFF, led_spy_get_state(0));
}

// Turn on LED, then turn it off, LED state is OFF
TEST(LEDSpyTest, led_turned_on_then_off)
{
    led_spy_set_state(0, LED_ON);
    led_spy_set_state(0, LED_OFF);
    LONGS_EQUAL(LED_OFF, led_spy_get_state(0));
}

// Turn OFF LED, then turn it on, LED state is ON
TEST(LEDSpyTest, led_turned_off_then_on)
{
    led_spy_set_state(0, LED_OFF);
    led_spy_set_state(0, LED_ON);
    LONGS_EQUAL(LED_ON, led_spy_get_state(0));
}

/* 
MANY
*/

// LED spy initialised, two LEDs are undefined
TEST(LEDSpyTest, init_and_two_leds_undefined)
{
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(0));
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(1));
}
 
// turn on LED 0, LED 0 is on, led 1 is undefined
TEST(LEDSpyTest, turn_on_led_0_led_0_on_led_1_undefined)
{
    led_spy_set_state(0, LED_ON);
    LONGS_EQUAL(LED_ON, led_spy_get_state(0));
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(1));
}

// turn on LED 1, turn off LED 2, LED 1 is ON led 2 is off 
TEST(LEDSpyTest, turn_on_led_0_turn_off_led_1_led_0_on_led_1_off)
{
    led_spy_set_state(0, LED_ON);
    led_spy_set_state(1, LED_OFF);
    LONGS_EQUAL(LED_ON, led_spy_get_state(0));
    LONGS_EQUAL(LED_OFF, led_spy_get_state(1));
}

// don't allow setting state of LEDs outside of LED_MAX. if the state of a LED outside the range is queried, it should reutrn undefined
TEST(LEDSpyTest, dont_allow_setting_invalid_leds)
{
    led_spy_set_state(LEDS_MAX, LED_ON);
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(LEDS_MAX));
}

