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
        LONGS_EQUAL(led_get_sequence_id(led_id), sequence_id)

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

    led_status_t define_and_register_led_super(uint32_t id, bool enabled, pins_t pinout)
    {
        // Make a new LED
        led_t new_led = {
            .id = id,
            .enabled = enabled,
            .pinout = pinout
        };
    
        // Register the LED with the module
        return led_register(new_led);
    }

    int32_t define_and_register_sequence(uint32_t id)
    {
        sequence_t sequence = {
            .length = 2,
            .period = 10
        };

        uint8_t arr[] = {LED_OFF, LED_ON};

        memcpy(sequence.sequence, arr, sequence.length);

        return sequence_register(sequence);
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
            led_timer_step();
        }
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

    //define_and_register_sequence(sequence_id);
    sequence_t sequence = {
        .length = 2,
        .period = 100
    };
    uint8_t arr[] = {LED_OFF, LED_ON};
    memcpy(sequence.sequence, arr, sequence.length);
    sequence_register(sequence);

    define_and_register_led(led_id);

    led_assign_sequence(led_id, sequence_id);
    
    uint32_t thresh = 700 / 2;

    led_enable(led_id);

    step_n_times(1);
    
    IS_LED_OFF(led_id);

    step_n_times(thresh + 1);

    IS_LED_ON(led_id);
}

// test to make sure turning off an unregistered LED does not change its state
TEST(LEDTest, turn_off_led_with_no_registered_leds_doesnt_change_state)
{
    led_off(0);

    IS_LED_UNDEFINED(0);
}

// make sure getting the sequence of an unregistered LED returns error code
TEST(LEDTest, get_led_sequence_of_nonregistered_led_returns_error)
{
    // NOTE led_get_sequence prototype returns uint32_t, but the "error" conditions returns -1 (signed). this is sus
    LONGS_EQUAL((uint32_t)(-1), led_get_sequence_id(0));
}

// make sure calling the timer callback doesn't change the state of LEDs with an unregistered sequence
TEST(LEDTest, timer_step_on_led_with_non_existent_sequence_doesnt_touch_led_state)
{
    uint32_t led_id = 0;
    define_and_register_led(led_id);
    led_enable(led_id);

    step_n_times(1000);

    IS_LED_UNDEFINED(led_id);
}

// make sure disabled LED states aren't touched by the timer callback
TEST(LEDTest, timer_step_on_disabled_led_doesnt_touch_led_state)
{
    uint32_t led_id = 0;
    uint32_t sequence_id = 0;
    define_and_register_led(led_id);
    define_and_register_sequence(sequence_id);
    led_assign_sequence(led_id, sequence_id);
    led_disable(led_id);

    step_n_times(1000);

    IS_LED_UNDEFINED(led_id);
}

TEST(LEDTest, turn_on_led_and_led_turns_on_when_time_steps){

    led_init(10);

    uint32_t led_id = 0;

    define_and_register_led(led_id);
    led_turn_on(led_id);

    led_timer_step();
    
    IS_LED_ON(led_id);
}

TEST(LEDTest, user_turn_off_led_interface_and_led_turns_off){
    uint32_t led_id = 0;

    define_and_register_led(led_id);
    led_turn_off(led_id);

    led_timer_step();

    IS_LED_OFF(led_id);
}

TEST(LEDTest, run_long_sequence_for_half_a_period_then_turn_on_led_immediately)
{
    led_init(10);

    define_and_register_led_super(0, true, {.pin = 0});

    // Build the sequence
    uint8_t sequence[100] = {0};
    sequence[0] = LED_ON;
    for(uint8_t i = 1; i < 99; i++){
        sequence[i] = LED_OFF;    
    }

    uint32_t seq_id = define_and_register_sequence_super(100, 1000, &sequence[0]);

    led_assign_sequence(0, seq_id);

    // step til we're deep into the long sequence
    step_n_times(50);
    
    // check LED off
    IS_LED_OFF(0);
    
    // turn on led
    led_turn_on(0);

    // step once
    step_n_times(1);
    
    // make sure LED turns on instantly
    IS_LED_ON(0);
}

/********/
/* MANY */
/********/







// TEST(LEDTest, check_led_starts_from_the_beginning_of_newly_assigned_sequence)
// {
//     led_init(1);
//     uint32_t led_id = 0;

//     define_and_register_led_super(led_id, true, {.pin = 0});

//     uint8_t sequence_0[] = {LED_OFF, LED_ON};
//     uint32_t seq_id_0 = define_and_register_sequence_super(2, 4, &sequence_0[0]);
//     led_assign_sequence(led_id, seq_id_0);

//     step_n_times(2);
//     IS_LED_ON(led_id);
    
//     // step_n_times(2);
//     // IS_LED_OFF(led_id);

//     uint8_t sequence_1[] = {LED_OFF, LED_ON, LED_ON, LED_ON, LED_ON};
//     uint8_t period = 10;
//     uint8_t length = 5;
//     uint32_t seq_id_1 = define_and_register_sequence_super(length, period, &sequence_1[0]);

//     // CHECK(seq_id_1 > 1);
    
//     led_assign_sequence(led_id, seq_id_1);
    
//     step_n_times(1); 
//     IS_LED_OFF(led_id);
// }