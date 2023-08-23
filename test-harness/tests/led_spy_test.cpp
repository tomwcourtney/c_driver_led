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



// After spy initialisation, the spy LED state is undefined
TEST(LEDSpyTest, led_undefined_after_initialisation)
{
    LONGS_EQUAL(LED_STATE_UNDEFINED, led_spy_get_state(0));
}

// // Calling led_spy_set_state(0xfff,ON) creates an error that can be read from led_spy_get_error(), (gives us a way to check if an id is passed in incorrectly)
// TEST(LEDSpyTest, writing_to_undefied_led_returns_error)
// {
//     // Write to led that does not exist
//     led_spy_set_state(0xfffff, ON);
//     // Check that the UNDEFINED_LED_ERROR is returned 
//     LONGS_EQUAL(UNDEFINED_LED_ERROR, led_spy_get_error());
    
// }




// Calling led_spy_set_state(ON) causes the LED to be turned on
TEST(LEDSpyTest, led_turn_on)
{
    led_spy_set_state(0,LED_STATE_ON);
    LONGS_EQUAL(LED_STATE_ON, led_spy_get_state(0));

}

// Turn off an LED
TEST(LEDSpyTest, led_turned_off)
{
    led_spy_set_state(0, LED_STATE_OFF);
    LONGS_EQUAL(LED_STATE_OFF, led_spy_get_state(0));
}


/* ONE
    
*/

/* MANY

*/

