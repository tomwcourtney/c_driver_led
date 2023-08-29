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
        led_init(1);
        led_spy_init();
    }

    void teardown()
    {
    }

    #define ARE_N_LEDS_REGISTERED(num)\
        LONGS_EQUAL(num, led_get_count());
    
    #define ARE_N_SEQUENCES_REGISTERED(num)\
        LONGS_EQUAL(num, sequence_get_count());

    #define DOES_LED_HAVE_SEQUENCE(led_id, sequence_id)\
        LONGS_EQUAL(led_get_sequence(led_id), sequence_id)

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

    sequence_status_t define_and_register_sequence(uint32_t id)
    {
        sequence_t sequence = {
            .id = id,
            .length = 2,
            .period = 10
        };

        uint8_t arr[] = {0,1};

        memcpy(sequence.sequence, arr, sequence.length);

        return sequence_register(sequence);
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

// Sequence: Register one sequence
TEST(LEDTest, sequence_can_be_defined)
{
    uint32_t sequence_id = 0;

    define_and_register_sequence(sequence_id);
    
    ARE_N_SEQUENCES_REGISTERED(1);
}


// Sequences can be assigned to leds 
TEST(LEDTest, sequences_can_be_assigned_to_leds)
{
    uint32_t sequence_id = 0;
    uint32_t led_id = 0;

    // Define LED and sequence
    define_and_register_sequence(sequence_id);
    define_and_register_led(led_id);
    
    // Assign sequence to LED
    led_assign_sequence(led_id,sequence_id);

    // Check if the sequence has been assigned correctly 
    DOES_LED_HAVE_SEQUENCE(led_id, sequence_id);
}

// Cannot assign a non-existant sequence to an led 
TEST(LEDTest, cannot_assign_nonexistant_sequence_to_led)
{
    uint32_t sequence_id = 0;
    uint32_t led_id = 0;

    // Define LED
    define_and_register_led(led_id);
    
    // Assign sequence to LED and check if sequence was assigned
    LONGS_EQUAL(LED_ERR,led_assign_sequence(led_id,sequence_id));
}

// cannot assign a sequence to a non existant led 
TEST(LEDTest, cannot_assign_sequence_to_non_existant_led)
{
    uint32_t sequence_id = 0;
    uint32_t led_id = 0;

    // Define sequence
    define_and_register_sequence(sequence_id);
    
    // Assign sequence to LED and check if sequence was assigned
    LONGS_EQUAL(LED_ERR,led_assign_sequence(led_id,sequence_id));
}

// When Sequence start is called the first bit of sequence is not written to LED
TEST(LEDTest, sequence_start_doesnt_change_led_state)
{
    uint32_t sequence_id = 0;
    uint32_t led_id = 0;

    define_and_register_sequence(sequence_id);
    define_and_register_led(led_id);

    led_assign_sequence(led_id, sequence_id);
    
    led_enable(led_id);

    IS_LED_UNDEFINED(led_id);
}

// sequence is assigned to LED, LED state is as it should be according to the sequence for every time step induced by the timer interrupt
TEST(LEDTest, led_state_changes_according_to_registered_sequence_when_led_enabled_and_updated)
{
    uint32_t sequence_id = 0;
    uint32_t led_id = 0;

    define_and_register_sequence(sequence_id);
    define_and_register_led(led_id);

    led_assign_sequence(led_id, sequence_id);
    
    led_enable(led_id);

    led_timer_step();
    
    IS_LED_OFF(led_id);

    led_timer_step();

    IS_LED_ON(led_id);
}
/********/
/* MANY */
/********/


