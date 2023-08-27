#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../../inc/led.h"
    #include "../spies/led_spy.h"
}


TEST_GROUP(LEDTest) 
{
    void setup()
    {
        led_init(10);
        led_spy_init();
    }

    void teardown()
    {
    }

    #define ARE_N_LEDS_REGISTERED(num)\
        LONGS_EQUAL(num, led_get_count());
    
    #define ARE_N_SEQUENCES_REGISTERED(num)\
        LONGS_EQUAL(num, sequence_get_count());

    led_status_t define_and_register_led(uint32_t id)
    {
        // Make a new LED
        led_t new_led = {
            .id = id,
            .enabled = true,
            .pinout = {.pin = 0}
        };
    
        // Register the LED with the module
        return led_register(new_led);
    }

};


/********/
/* ZERO */
/********/

TEST(LEDTest, init_led_driver_has_no_registered_led)
{
    ARE_N_LEDS_REGISTERED(0);
}

// turning on an LED that isn't registered does not turn on that LED
TEST(LEDTest, turn_on_led_when_nothing_registered_led_undefined)
{
    uint32_t id = 0;

    led_on(id);

    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(id));
}

// Check that after the sequence is initialized that there are no registered sequences.
TEST(LEDTest, no_sequences_initialized_when_led_initialized)
{
    ARE_N_SEQUENCES_REGISTERED(0);
}

/*******/
/* ONE */
/*******/

// register an LED, and then LED is registered..
TEST(LEDTest, register_led_led_registered)
{
    uint32_t led_id = 0;
    
    define_and_register_led(led_id);
    
    ARE_N_LEDS_REGISTERED(1);
    
}

TEST(LEDTest, cannot_register_registered_led)
{
    uint32_t old_led_id = 0;
    uint32_t new_led_id = 0;

    define_and_register_led(old_led_id);
    

    // Register the LED with the module, checking it returns error
    LONGS_EQUAL(LED_ERR, define_and_register_led(new_led_id));
    
    // make sure another led wasn't added to list
    ARE_N_LEDS_REGISTERED(1);
}

TEST(LEDTest, turn_on_led_led_turns_on)
{
    uint32_t led_id = 0;

    define_and_register_led(led_id);

    led_on(led_id);

    IS_LED_ON(led_id);
}

TEST(LEDTest, turn_off_led)
{
    uint32_t led_id = 0;
    
    define_and_register_led(led_id);

    led_on(led_id);

    led_off(led_id);

    IS_LED_OFF(led_id);
}


// Disabled led's state cannot be chagned 
TEST(LEDTest, disabled_led_state_cannot_be_changed)
{
    uint32_t led_id = 0;

    define_and_register_led(led_id);

    led_disable(led_id);
    
    led_on(led_id);

    IS_LED_UNDEFINED(led_id);
}

// Disabled led can be renabled 
TEST(LEDTest, disabled_led_state_can_be_reenabled)
{
    uint32_t led_id = 0;

    define_and_register_led(led_id);

    led_disable(led_id);
    
    led_enable(led_id);
    
    led_on(led_id);

    IS_LED_ON(led_id);
}

// Sequence can be defined 
TEST(LEDTest, sequence_can_be_defined)
{
    // define a sequence 
    // uint32_t length = 2;
    // uint32_t sequence_array[] = {0, 1};
    // uint32_t sequence_period = 10;
    // led_register_sequence(sequence_period, sequence_array, length);


    // Check that the sequence is defined 
}
/* MANY

*/

