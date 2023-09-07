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

    int32_t define_and_register_led()
    {
        // Make a new LED
        led_t new_led = {
            .enabled = true,
            .pinout = {.pin = 0},
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
        };
    
        // Register the LED with the module
        return led_register(new_led);
    }

    int32_t define_and_register_led_super(bool enabled, pins_t pinout)
    {
        // Make a new LED
        led_t new_led = {
            .enabled = enabled,
            .pinout = pinout,
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
        };
        
        // Register the LED with the module
        return led_register(new_led);
    }

    int32_t define_and_register_sequence()
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
    int32_t id = 0;

    led_on(id);

    LONGS_EQUAL(LED_UNDEFINED, led_spy_get_state(id));
}

/*******/
/* ONE */
/*******/

// register an LED, and then LED is registered..
TEST(LEDTest, register_led_led_registered)
{
    define_and_register_led();
    
    ARE_N_LEDS_REGISTERED(1);
}

TEST(LEDTest, turn_on_led_led_turns_on)
{
    int32_t led_id = define_and_register_led();

    led_on(led_id);

    IS_LED_ON(led_id);
}

TEST(LEDTest, turn_off_led)
{
    int32_t led_id = define_and_register_led();

    led_on(led_id);

    led_off(led_id);

    IS_LED_OFF(led_id);
}

// Disabled led's state cannot be chagned 
TEST(LEDTest, disabled_led_state_cannot_be_changed)
{
    int32_t led_id = define_and_register_led();

    led_disable(led_id);
    
    led_on(led_id);

    IS_LED_UNDEFINED(led_id);
}

// Disabled led can be renabled 
TEST(LEDTest, disabled_led_state_can_be_reenabled)
{
    int32_t led_id = define_and_register_led();

    led_disable(led_id);
    
    led_enable(led_id);
    
    led_on(led_id);

    IS_LED_ON(led_id);
}

// Sequences can be assigned to leds 
TEST(LEDTest, sequences_can_be_assigned_to_leds)
{
    // Define LED and sequence
    int32_t sequence_id = define_and_register_sequence();
    int32_t led_id = define_and_register_led();
    
    // Assign sequence to LED
    led_assign_sequence(led_id, sequence_id);

    // Check if the sequence has been assigned correctly 
    DOES_LED_HAVE_SEQUENCE(led_id, sequence_id);
}

// Cannot assign a non-existant sequence to an led 
TEST(LEDTest, cannot_assign_nonexistant_sequence_to_led)
{
    int32_t sequence_id = sequence_get_count() + 1;
    int32_t led_id = define_and_register_led();
    
    // Assign sequence to LED and check if sequence was assigned
    LONGS_EQUAL(LED_ERR, led_assign_sequence(led_id,sequence_id));
}

// cannot assign a sequence to a non existant led 
TEST(LEDTest, cannot_assign_sequence_to_non_existant_led)
{
    int32_t sequence_id = define_and_register_sequence();
    int32_t led_id = 0;
    
    // Assign sequence to LED and check if sequence was assigned
    LONGS_EQUAL(LED_ERR,led_assign_sequence(led_id,sequence_id));
}

// When Sequence start is called the first bit of sequence is not written to LED
TEST(LEDTest, sequence_start_doesnt_change_led_state)
{
    int32_t sequence_id = define_and_register_sequence();
    int32_t led_id = define_and_register_led();

    led_assign_sequence(led_id, sequence_id);
    
    led_enable(led_id);

    IS_LED_UNDEFINED(led_id);
}

// sequence is assigned to LED, LED state is as it should be according to the sequence for every time step induced by the timer interrupt
TEST(LEDTest, led_state_changes_according_to_registered_sequence_when_led_enabled_and_updated)
{
    //define_and_register_sequence(sequence_id);
    sequence_t sequence = {
        .length = 2,
        .period = 100
    };
    uint8_t arr[] = {LED_OFF, LED_ON};
    memcpy(sequence.sequence, arr, sequence.length);
    int32_t sequence_id = sequence_register(sequence);

    int32_t led_id = define_and_register_led();

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
    LONGS_EQUAL(-1, led_get_sequence_id(0));
}

// make sure calling the timer callback doesn't change the state of LEDs with an unregistered sequence
TEST(LEDTest, timer_step_on_led_with_non_existent_sequence_doesnt_touch_led_state)
{
    int32_t led_id = define_and_register_led_super(true, {.pin = 0});

    step_n_times(1000);

    IS_LED_UNDEFINED(led_id);
}

// make sure disabled LED states aren't touched by the timer callback
TEST(LEDTest, timer_step_on_disabled_led_doesnt_touch_led_state)
{
    int32_t sequence_id = define_and_register_sequence();
    int32_t led_id = define_and_register_led();
    led_assign_sequence(led_id, sequence_id);
    led_disable(led_id);

    step_n_times(1000);

    IS_LED_UNDEFINED(led_id);
}

// make sure led_turn_on turns on led
TEST(LEDTest, turn_on_led_and_led_turns_on_when_time_steps){

    led_init(10);

    int32_t led_id = define_and_register_led();
    led_turn_on(led_id);

    led_timer_step();
    
    IS_LED_ON(led_id);
}

// make sure led_turn_off turns off led
TEST(LEDTest, user_turn_off_led_interface_and_led_turns_off){
    int32_t led_id = define_and_register_led();
    led_turn_off(led_id);

    led_timer_step();

    IS_LED_OFF(led_id);
}

// test that led_turn_on overrides any current sequence
TEST(LEDTest, run_long_sequence_for_half_a_period_then_turn_on_led_immediately)
{
    led_init(10);

    int32_t led_id = define_and_register_led_super(true, {.pin = 0});

    // Build the sequence
    uint8_t sequence[100] = {0};
    sequence[0] = LED_ON;
    for(uint8_t i = 1; i < 99; i++){
        sequence[i] = LED_OFF;    
    }

    int32_t seq_id = define_and_register_sequence_super(100, 1000, &sequence[0]);

    led_assign_sequence(led_id, seq_id);

    // step til we're deep into the long sequence
    step_n_times(50);
    
    // check LED off
    IS_LED_OFF(led_id);
    
    // turn on led
    led_turn_on(led_id);

    // step once
    step_n_times(1);
    
    // make sure LED turns on instantly
    IS_LED_ON(led_id);
}


// make sure getting sequence id for an led that hasn't been assigned a sequence returns -1

// range check/led_exists check on led_disable/enable

// range check/led_exists check on led_turn_on/off

// make sure passing negative IDs to functions that take an ID fails properly (led and sequence)

/********/
/* MANY */
/********/

TEST(LEDTest, check_led_starts_from_the_beginning_of_newly_assigned_sequence)
{
    led_init(1);
    int32_t led_id = define_and_register_led_super(true, {.pin = 0});

    uint8_t sequence_0[] = {LED_OFF, LED_ON};
    int32_t seq_id_0 = define_and_register_sequence_super(2, 4, sequence_0);
    led_assign_sequence(led_id, seq_id_0);

    step_n_times(2);
    IS_LED_ON(led_id);
    
    // step_n_times(2);
    // IS_LED_OFF(led_id);

    uint8_t sequence_1[] = {LED_OFF, LED_ON, LED_ON, LED_ON, LED_ON};
    uint8_t period = 10;
    uint8_t length = 5;
    int32_t seq_id_1 = define_and_register_sequence_super(length, period, sequence_1);

    // CHECK(seq_id_1 > 1);
    
    led_assign_sequence(led_id, seq_id_1);
    
    step_n_times(1); 
    IS_LED_OFF(led_id);
}

TEST(LEDTest, register_two_leds_led_count_increases)
{
    // Register LED
    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});
    
    // Check they've registered 
    ARE_N_LEDS_REGISTERED(2);

    // check they're undefined
    IS_LED_UNDEFINED(led_0_id);
    IS_LED_UNDEFINED(led_1_id);
}

TEST(LEDTest, two_led_can_be_turned_on)
{
    // Registers LED's
    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});
    
    // Turn them both on 
    led_turn_on(led_0_id);
    led_turn_on(led_1_id);

    // Step to turn on
    led_timer_step();

    // Check they are both on 
    IS_LED_ON(led_0_id);
    IS_LED_ON(led_1_id);
    
}


TEST(LEDTest, two_led_can_be_turned_on_then_off)
{
    // Registers LED's
    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});
    
    // Turn them both on 
    led_turn_on(led_0_id);
    led_turn_on(led_1_id);

    // Step to turn on
    led_timer_step();

    // Turn them both off 
    led_turn_off(led_0_id);
    led_turn_off(led_1_id);

    // Step to turn on
    led_timer_step();

    // Check they are both on 
    IS_LED_OFF(led_0_id);
    IS_LED_OFF(led_1_id);
    
}

// make sure getting LED that doesnt exist returns null
TEST(LEDTest, get_led_that_doesnt_exist_returns_null)
{
    led_t * led = led_get_from_id(0);    

    POINTERS_EQUAL(NULL, led);
}


TEST(LEDTest, check_registered_led_values_are_expected)
{
    uint32_t led_id = define_and_register_led_super(true, {.pin = 0});

    led_t * led_obj = led_get_from_id(led_id);

    CHECK(led_obj->enabled);
    LONGS_EQUAL(-1, led_obj->sequence_id);
    LONGS_EQUAL(0,led_obj->sequence_idx);
    LONGS_EQUAL(0,led_obj->timer_count);
}


// Define an LED
// Step
// Define a second LED and check values match expected

TEST(LEDTest, register_one_led_then_another)
{
    led_init(1);

    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});

    led_turn_on(led_0_id);

    led_timer_step();
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});

    // Check Sequnce is expected
    LONGS_EQUAL(1, led_get_sequence_id(led_0_id));
    LONGS_EQUAL(-1, led_get_sequence_id(led_1_id));

    // Check led is on 
    IS_LED_ON(led_0_id);
    IS_LED_UNDEFINED(led_1_id);

}

TEST(LEDTest, register_two_leds_turn_one_on)
{
    led_init(1);

    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});

    led_turn_on(led_0_id);

    led_timer_step();

    LONGS_EQUAL(1, led_get_sequence_id(led_0_id));
    LONGS_EQUAL(-1, led_get_sequence_id(led_1_id));


    IS_LED_ON(led_0_id);
    IS_LED_UNDEFINED(led_1_id);
}

TEST(LEDTest, sequnce_on_is_the_same_for_multiple_leds)
{
    // Register two Leds
    led_init(1);
    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});
    
    // Assign on sequence to both new LEDs
    led_turn_on(led_0_id);
    led_turn_on(led_1_id);
    
    // Check that the LEDs both have the same sequence assigned
    LONGS_EQUAL(1, led_get_sequence_id(led_0_id));
    LONGS_EQUAL(1, led_get_sequence_id(led_1_id));
}

TEST(LEDTest, sequnce_off_is_the_same_for_multiple_leds)
{
    // Register two Leds
    led_init(1);
    int32_t led_0_id = define_and_register_led_super(true, {.pin = 0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = 1});
    
    // Assign on sequence to both new LEDs
    led_turn_off(led_0_id);
    led_turn_off(led_1_id);
    
    // Check that the LEDs both have the same sequence assigned
    LONGS_EQUAL(0, led_get_sequence_id(led_0_id));
    LONGS_EQUAL(0, led_get_sequence_id(led_1_id));
}

TEST(LEDTest, two_leds_with_different_sequences_follow_their_respective_sequences)
{
    led_init(1);

    uint32_t user_pin_0 = 1;
    uint32_t user_pin_1 = 0;

    // define two leds
    int32_t led_0_id = define_and_register_led_super(true, {.pin = user_pin_0});
    int32_t led_1_id = define_and_register_led_super(true, {.pin = user_pin_1});

    // define two different sequences and assign each to a different led
    uint8_t sequence_0[] = {LED_OFF, LED_ON, LED_ON, LED_OFF};
    int32_t seq_0_id = define_and_register_sequence_super(4, 8, sequence_0);
    led_assign_sequence(led_0_id, seq_0_id);

    uint8_t sequence_1[] = {LED_ON, LED_OFF, LED_OFF};
    int32_t seq_1_id = define_and_register_sequence_super(3, 8, sequence_1);
    led_assign_sequence(led_1_id, seq_1_id);

    // step through sequence verifying each step
    IS_LED_UNDEFINED(user_pin_0);
    IS_LED_UNDEFINED(user_pin_1);
    
    step_n_times(1);

    IS_LED_OFF(user_pin_0);
    IS_LED_ON(user_pin_1);
    
    step_n_times(2);

    IS_LED_ON(user_pin_0);
    IS_LED_OFF(user_pin_1);

    step_n_times(2);

    IS_LED_ON(user_pin_0);
    IS_LED_OFF(user_pin_1);

    step_n_times(2);

    IS_LED_OFF(user_pin_0);
    IS_LED_ON(user_pin_1);

    step_n_times(2);

    IS_LED_OFF(user_pin_0);
    IS_LED_OFF(user_pin_1);
}

/********/
/* TODO */
/********/

// TODO make it so that after an LED timer step, the state is in the first position

// check pin independence
// e.g. we currently check led ID with IS_LED_ON etc. but what is actually turned on
// is pin. try changing pin assignments in above test and see fails

// make sure you can't register > LED_MAX leds

// TODO add offset field to sequence (esp. when registering it) so that you can use the same
// sequence array but have it start at a different point
// Add tests into led_turn_on and off, currently every time you turn an led on it creates
// a new sequence and assignes it. This will cause issues i think 