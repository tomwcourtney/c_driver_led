#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../spies/led_spy.h"
    #include "../../inc/sequence.h"
    #include "../../inc/led.h"
    #include "../../inc/rgb_led.h"
    #include <string.h>
}


TEST_GROUP(LEDRGBTest)
{
    void setup()
    {
        rgb_led_init();
        led_spy_init();
        led_init(1);
        sequence_init();
    }

    void teardown()
    {
    }
    #define ARE_N_SEQUENCES_REGISTERED(num)\
        LONGS_EQUAL(num, sequence_get_count());

    #define ARE_N_LEDS_REGISTERED(num)\
        LONGS_EQUAL(num, led_get_count());
        
    #define ARE_N_RGB_LEDS_REGISTERED(num)\
        LONGS_EQUAL(num, rgb_led_get_count());


    int32_t define_and_register_led_super(bool enabled, pins_t pinout)
    {
        // Make a new LED
        led_t new_led = {
            .enabled = enabled,
            .pinout = pinout,
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
            .sequence_initialized = false
        };
        
        // Register the LED with the module
        return led_register(new_led);
    }

    /**
     * @brief Defines and registers a sequence.
     * 
     * @param length    - the length of the sequence elements
     * @param period    - the duration of the sequence period in ms
     * @param seq       - the array specifying the sequence on/off.
     * @return int32_t - The id of the registered sequence or error if failed.
     */
    int32_t define_and_register_sequence_super(uint8_t length, uint32_t period, uint8_t * seq)
    {
        sequence_t sequence = {
            .length = length,
            .period = period
        };

        memcpy(sequence.sequence, seq, sequence.length);

        return sequence_register(sequence);
    }

    void step_n_times(int n)
    {
        for (int i = 0; i < n; i++)
        {
            led_update_state();
        }
    }
};

/********/
/* ZERO */
/********/

// Sequence can be initalised with an 0-255 values    
TEST(LEDRGBTest, sequence_can_be_assigned_0_to_255_values)
{
    
    sequence_t sequence = {
            .length = 2,
            .period = 10
        };


    uint8_t arr[] = {0, 255};

    memcpy(sequence.sequence, arr, sequence.length);

    uint32_t sequence_id = sequence_register(sequence);

    ARE_N_SEQUENCES_REGISTERED(1);
    CHECK(sequence_exists(sequence_id));
}

// Led Can be assigned Sequence with 0-255 values
TEST(LEDRGBTest, led_can_be_assinged_sequence_with_0_to_255_values)
{
    // Create an RGB sequence and register it 
    uint8_t sequence[5] = {50, 255, 10, 33,66};
 

    int32_t seq_id = define_and_register_sequence_super(5, 100, &sequence[0]);
    // Register an Led and assign it a Sequence 
    int32_t led_id = define_and_register_led_super(true, {.pin = 0});
    led_assign_sequence(led_id, seq_id);

    // Retrieve LED object
    led_t * led = led_get_from_id(led_id);
    
    // Check sequenceid is equal to registed sequence
    CHECK(led->sequence_id==seq_id);
}

// Led state matches the Sequence pattern 
TEST(LEDRGBTest, led_follows_rgb_sequence_assigned)
{
    // Create an RGB sequence and register it 
    uint8_t sequence[5] = {50, 255, 10, 33};
 

    int32_t seq_id = define_and_register_sequence_super(4, 8, &sequence[0]);
    // Register an Led and assign it a Sequence 
    int32_t led_id = define_and_register_led_super(true, {.pin = 0});
    led_assign_sequence(led_id, seq_id);

    // Check for 50
    step_n_times(1);
    LONGS_EQUAL(50, led_spy_get_state(led_id));

    // Check for 255
    step_n_times(2);
    LONGS_EQUAL(255, led_spy_get_state(led_id));

    // Check for 10
    step_n_times(2);
    LONGS_EQUAL(10, led_spy_get_state(led_id));

    // Check for 35
    step_n_times(2);
    LONGS_EQUAL(33, led_spy_get_state(led_id));
}

// After init the RGB driver has no Leds initalised 
TEST(LEDRGBTest, after_init_rgb_driver_has_no_leds_inited)
{
    // Initting the RGB led driver 
    rgb_led_init();

    // Check no Leds initted 
    ARE_N_LEDS_REGISTERED(0);
    ARE_N_RGB_LEDS_REGISTERED(0);
}

// Turning on an rgb LED that isn't registered does not turn on that LED
TEST(LEDRGBTest, turning_on_an_rgb_led_that_isnt_registered_doesnt_turn_on_that_led)
{
    // Turn led on white 
    int32_t id = 0;
    rgb_led_on(id, WHITE);
    // Check Turned off still
    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(id));
    
}
/* TODO 
*/ 


/*******/
/* ONE */
/*******/

// Registering an RGB led then 3 leds are registered and 1 rgb led
TEST(LEDRGBTest, registering_an_rgb_led_registers_3_leds_and_1_rgb_led)
{
    // Registering the RGB led 
    led_t new_led = {
            .enabled = true,
            .pinout = {.pin = 0},
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
            .sequence_initialized = false
        };
    rgb_led_register({.pin = 0}, {.pin = 1}, {.pin = 2}, new_led);
    // Checking that 3 Leds where registered 
    ARE_N_LEDS_REGISTERED(3);
    // Checking that 1 RGB led was registered 
    ARE_N_RGB_LEDS_REGISTERED(1);
}


/* TODO
* -
* - Turn an RGB LED on WHITE turns all leds on to full 255
* - Turn an RGB LED on BLUE turns only the blue led on to 255
* - Turn an RGB LED on RED turns only the red led on to 255
* - Turn an RGB LED on GREEN turns only the green led on to 255
* - Turing RGB LED off turns all values to 0 
* - 
*/

/********/
/* MANY */
/********/


/* TODO
* - Resitering an RGB led when ther is MAX_LEDS -2 already registed returns error and does not register led's
*
*/