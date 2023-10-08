#include "CppUTest/TestHarness.h"
#include <iostream>

extern "C"
{
#include "../spies/led_spy.h"
#include "../../inc/sequence.h"
#include "../../inc/led.h"
#include "../../inc/rgb_led.h"
#include <string.h>
}

TEST_GROUP(LEDRGBTest){
    void setup(){
        sequence_init();
led_spy_init();
led_init(1);
rgb_led_init();
}

void teardown()
{
}
#define ARE_N_SEQUENCES_REGISTERED(num) \
    LONGS_EQUAL(num, sequence_get_count());

#define ARE_N_LEDS_REGISTERED(num) \
    LONGS_EQUAL(num, led_get_count());

#define ARE_N_RGB_LEDS_REGISTERED(num) \
    LONGS_EQUAL(num, rgb_led_get_count());

#define ARE_N_SEQUENCES_REGISTERED(num) \
    LONGS_EQUAL(num, sequence_get_count());
#define ARE_N_RGB_SEQUENCES_REGISTERED(num) \
    LONGS_EQUAL(num, rgb_sequence_get_count());
#define DOES_LED_HAVE_SEQUENCE(led_id, sequence_id) \
    LONGS_EQUAL(led_get_sequence_id(led_id), sequence_id)

int32_t define_and_register_led_super(bool enabled, pins_t pinout)
{
    // Make a new LED
    led_t new_led = {
        .enabled = enabled,
        .pinout = pinout,
        .sequence_id = -1,
        .sequence_idx = 0,
        .timer_count = 0,
        .sequence_initialized = false};

    // Register the LED with the module
    return led_register(new_led);
}

int32_t define_and_register_sequence_super(uint8_t length, uint32_t period, uint8_t *seq)
{
    sequence_t sequence = {
        .length = length,
        .period = period};

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

int32_t register_rgb_led(pins_t redPin, pins_t greenPin, pins_t bluePin, bool enabled)
{
    led_t new_led = {
        .enabled = enabled,
        .pinout = {.pin = 0},
        .sequence_id = -1,
        .sequence_idx = 0,
        .timer_count = 0,
        .sequence_initialized = false};
    return rgb_led_register(redPin, greenPin, bluePin, new_led);
}

void check_sequence_matches_colour(int32_t seqId, uint32_t colour)
{
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId, &seqRedId, &seqGreenId, &seqBlueId);
    sequence_t *seq_obj_red = sequence_get_from_id(seqRedId);
    sequence_t *seq_obj_green = sequence_get_from_id(seqGreenId);
    sequence_t *seq_obj_blue = sequence_get_from_id(seqBlueId);
    CHECK_TEXT(((colour >> 16) & 0xFF) == seq_obj_red->sequence[0], "RED CHANNEL MISMATCH");
    CHECK_TEXT(((colour >> 8) & 0xFF) == seq_obj_green->sequence[0], "GREEN CHANNEL MISMATCH");
    CHECK_TEXT((colour & 0xFF) == seq_obj_blue->sequence[0], "BLUE CHANNEL MISMATCH");
}

void check_led_ids(int32_t rgbLedId, int32_t redId, int32_t greenId, int32_t blueId, int16_t redVal, int16_t greenVal, int16_t blueVal, int8_t steps)
{
    int32_t ledRedId = {0}, ledGreenId = {0}, ledBlueId = {0};
    rgb_led_get_ids_from_id(rgbLedId, &ledRedId, &ledGreenId, &ledBlueId);
    CHECK_TEXT(ledRedId == redId, "red Id Wrong");
    CHECK_TEXT(ledGreenId == greenId, "grn Id Wrong");
    CHECK_TEXT(ledBlueId == blueId, "blu Id Wrong");
    step_n_times(steps);
    LONGS_EQUAL_TEXT(redVal, led_spy_get_state(ledRedId), "GREEN LED ERR");
    LONGS_EQUAL_TEXT(greenVal, led_spy_get_state(ledGreenId), "BLUE LED ERR");
    LONGS_EQUAL_TEXT(blueVal, led_spy_get_state(ledBlueId), "RED LED ERR");
}
}
;

/********/
/* ZERO */
/********/

// Sequence can be initalised with an 0-255 values
TEST(LEDRGBTest, sequence_can_be_assigned_0_to_255_values)
{
    uint32_t preSeqCount = sequence_get_count();
    sequence_t sequence = {
        .length = 2,
        .period = 10};
    uint8_t arr[] = {0, 255};

    memcpy(sequence.sequence, arr, sequence.length);
    uint32_t sequence_id = sequence_register(sequence);

    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 1);
    CHECK(sequence_exists(sequence_id));
}

// Led Can be assigned Sequence with 0-255 values
TEST(LEDRGBTest, led_can_be_assinged_sequence_with_0_to_255_values)
{
    // Create an RGB sequence and register it
    uint8_t sequence[5] = {50, 255, 10, 33, 66};

    int32_t seq_id = define_and_register_sequence_super(5, 100, &sequence[0]);
    // Register an Led and assign it a Sequence
    int32_t led_id = define_and_register_led_super(true, {.pin = 0});
    led_assign_sequence(led_id, seq_id);

    // Retrieve LED object
    led_t *led = led_get_from_id(led_id);

    // Check sequenceid is equal to registed sequence
    CHECK(led->sequence_id == seq_id);
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
    rgb_led_on(id, RGB_WHITE);
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
    register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    // Checking that 3 Leds where registered
    ARE_N_LEDS_REGISTERED(3);
    // Checking that 1 RGB led was registered
    ARE_N_RGB_LEDS_REGISTERED(1);
}

// Registering an RGB sequence registers 3 sequences
TEST(LEDRGBTest, Registering_1_rgb_sequences_registers_3_sequences)
{
    // Stuff
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence
    uint32_t seq[1] = {C_WHITE};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Check that 1 extra RGB sequences exits
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount + 1);
    // Check that 3 extra LED Sequnces exits
    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 3);
    // Check that the 3 LED Sequences contain only 255
    check_sequence_matches_colour(seqId, C_WHITE);
}

TEST(LEDRGBTest, Registering_red_rgb_sequences_registers_3_sequences)
{
    // Stuff
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence
    uint32_t seq[1] = {C_RED};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Check that 1 extra RGB sequences exits
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount + 1);
    // Check that 3 extra LED Sequnces exits
    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 3);
    // Check that the 3 LED Sequences contain only 255
    check_sequence_matches_colour(seqId, C_RED);
}

TEST(LEDRGBTest, Registering_green_rgb_sequences_registers_3_sequences)
{
    // Stuff
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence
    uint32_t seq[1] = {C_GRN};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Check that 1 extra RGB sequences exits
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount + 1);
    // Check that 3 extra LED Sequnces exits
    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 3);
    // Check that the 3 LED Sequences contain only 255
    check_sequence_matches_colour(seqId, C_GRN);
}

TEST(LEDRGBTest, Registering_blue_rgb_sequences_registers_3_sequences)
{
    // Stuff
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence
    uint32_t seq[1] = {C_BLUE};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Check that 1 extra RGB sequences exits
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount + 1);
    // Check that 3 extra LED Sequnces exits
    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 3);
    // Check that the 3 LED Sequences contain only 255
    check_sequence_matches_colour(seqId, C_BLUE);
}

// Sequence can be assigned to rgb led
TEST(LEDRGBTest, sequencs_can_be_assigned_to_rgb_led)
{
    // Definine a sequence
    uint32_t seq[1] = {C_WHITE};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    // Assign Sequence
    led_status_t assignStatus = rgb_assign_sequence(ledId, seqId);
    LONGS_EQUAL(LED_OK, assignStatus);
    // Check each led has a sequence assigned
    int32_t ledRedId = {0}, ledGreenId = {0}, ledBlueId = {0};
    rgb_led_get_ids_from_id(ledId, &ledRedId, &ledGreenId, &ledBlueId);
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId, &seqRedId, &seqGreenId, &seqBlueId);
    DOES_LED_HAVE_SEQUENCE(ledRedId, seqRedId);
    DOES_LED_HAVE_SEQUENCE(ledGreenId, seqGreenId);
    DOES_LED_HAVE_SEQUENCE(ledBlueId, seqBlueId);
}

// Once assigned sequence is assigned RGB led follows pattern
TEST(LEDRGBTest, sequencs_are_followed_by_rgb_led)
{
    // Definine a sequence
    uint32_t seq[1] = {C_RED};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    // Assign Sequence
    led_status_t assignStatus = rgb_assign_sequence(ledId, seqId);
    LONGS_EQUAL(LED_OK, assignStatus);
    // Check each led has a sequence assigned
    int32_t ledRedId = {0}, ledGreenId = {0}, ledBlueId = {0};
    rgb_led_get_ids_from_id(ledId, &ledRedId, &ledGreenId, &ledBlueId);
    step_n_times(1);
    LONGS_EQUAL(255, led_spy_get_state(ledRedId));
    LONGS_EQUAL(0, led_spy_get_state(ledGreenId));
    LONGS_EQUAL(0, led_spy_get_state(ledBlueId));
}

/********/
/* MANY */
/********/

// Can Register many Sequences
TEST(LEDRGBTest, multiple_rgb_sequences_can_be_registered)
{
    // Stuff
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence
    uint32_t seq_1[1] = {C_WHITE};
    int32_t seqId_1 = rgb_sequence_register(1, 1, seq_1);
    uint32_t seq_2[1] = {C_RED};
    int32_t seqId_2 = rgb_sequence_register(1, 1, seq_2);
    // Check that 1 extra RGB sequences exits
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount + 2);
    // Check that 3 extra LED Sequnces exits
    ARE_N_SEQUENCES_REGISTERED(preSeqCount + 6);
    // Checking white pattern
    check_sequence_matches_colour(seqId_1, C_WHITE);
    // Checking red pattern
    check_sequence_matches_colour(seqId_2, C_RED);
}

// Turn an RGB LED on WHITE turns all leds on to full 255
TEST(LEDRGBTest, led_on_function_can_turn_led_white)
{
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);

    // Turn LED White
    led_status_t onStatus = rgb_led_on(ledId, RGB_WHITE);
    LONGS_EQUAL_TEXT(LED_OK, onStatus, "COMMANND FAiILED");
    // Check state of LED
    check_led_ids(ledId, 0, 1, 2, 255, 255, 255, 1);
}

TEST(LEDRGBTest, led_on_function_can_turn_led_red)
{
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);

    // Turn LED White
    led_status_t onStatus = rgb_led_on(ledId, RGB_RED);
    LONGS_EQUAL_TEXT(LED_OK, onStatus, "COMMANND FAiILED");
    // Check state of LED
    check_led_ids(ledId, 0, 1, 2, 255, 0, 0, 1);
}

TEST(LEDRGBTest, led_on_function_can_turn_led_green)
{
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);

    // Turn LED White
    led_status_t onStatus = rgb_led_on(ledId, RGB_GREEN);
    LONGS_EQUAL_TEXT(LED_OK, onStatus, "COMMANND FAiILED");
    // Check state of LED
    check_led_ids(ledId, 0, 1, 2, 0, 255, 0, 1);
}

TEST(LEDRGBTest, led_on_function_can_turn_led_blue)
{
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);

    // Turn LED White
    led_status_t onStatus = rgb_led_on(ledId, RGB_BLUE);
    LONGS_EQUAL_TEXT(LED_OK, onStatus, "COMMANND FAiILED");
    // Check state of LED
    check_led_ids(ledId, 0, 1, 2, 0, 0, 255, 1);
}

TEST(LEDRGBTest, led_on_function_can_turn_led_off)
{
    // Define an RGB led
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);

    // Turn LED White
    led_status_t onStatus = rgb_led_on(ledId, RGB_OFF);
    LONGS_EQUAL_TEXT(LED_OK, onStatus, "COMMANND FAiILED");
    // Check state of LED
    check_led_ids(ledId, 0, 1, 2, 0, 0, 0, 1);
}

// More then one RGB led can be registered
TEST(LEDRGBTest, more_then_one_rgb_led_can_be_registered)
{
    // Define an RGB led
    int32_t ledId_1 = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    int32_t ledId_2 = register_rgb_led({.pin = 3}, {.pin = 4}, {.pin = 5}, true);
    // Check number of leds registered
    ARE_N_LEDS_REGISTERED(6);
    // Check number of RGB leds registered
    ARE_N_RGB_LEDS_REGISTERED(2);

    // Check refistered led pins
    check_led_ids(ledId_1, 0, 1, 2, 0, 0, 0, 1);
    check_led_ids(ledId_2, 3, 4, 5, 0, 0, 0, 1);
}

// Trying to assign RGB sequence that does not exist returns error
TEST(LEDRGBTest, assiginging_an_rgb_led_a_sequence_that_does_not_exisist_returns_error)
{
    // Define an RGB led
    int32_t ledId_1 = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    // Check number of leds registered
    led_status_t assignStatus = rgb_assign_sequence(ledId_1, 20);
    LONGS_EQUAL(LED_ERR, assignStatus);
}

// Trying to assign RGB sequence to an led that does not exist returns error
TEST(LEDRGBTest, assiginging_an_unexistant_rgb_led_a_sequencereturns_error)
{
    // Assign sequence and check fails
    led_status_t assignStatus = rgb_assign_sequence(5, RGB_BLUE);
    LONGS_EQUAL(LED_ERR, assignStatus);
}

// Resitering an RGB led when ther is MAX_LEDS -2 already registed returns error and does not register led's
TEST(LEDRGBTest, cannot_register_rgb_led_whhen_there_is_already_max_led_m2_registed)
{
    // Register max led's -2
    for (int i = 0; i < (LEDS_MAX - 2); i++)
    {
        define_and_register_led_super(true, {.pin = 0});
    }
    // Try to register RGB led
    int32_t ledId_1 = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    LONGS_EQUAL(-1, ledId_1);
    ARE_N_LEDS_REGISTERED((LEDS_MAX - 2));
}

// RGB sequce Follows pattern
TEST(LEDRGBTest, rgb_sequence_can_follow_long_pattern)
{
    // Register RGB sequence
    uint32_t seq_1[4] = {C_WHITE, C_GRN, C_BLUE, C_OFF};
    int32_t seqId_1 = rgb_sequence_register(4, 8, seq_1);
    // Define an RGB led
    int32_t ledId_1 = register_rgb_led({.pin = 0}, {.pin = 1}, {.pin = 2}, true);
    // Assgin sequence to led
    rgb_assign_sequence(ledId_1, seqId_1);
    // Step and check colour
    check_led_ids(ledId_1, 0, 1, 2, 255, 255, 255, 1);
    check_led_ids(ledId_1, 0, 1, 2, 0, 255, 0, 2);
    check_led_ids(ledId_1, 0, 1, 2, 0, 0, 255, 2);
    check_led_ids(ledId_1, 0, 1, 2, 0, 0, 0, 2);
}

//  Registering an RGB sequence when there is MAX_SEQUENclearCE - 2 returns error
TEST(LEDRGBTest, cannot_register_rgb_seq_when_there_is_already_max_seq_m2_registed)
{
    // Register max led's -2
    uint8_t sequence[5] = {50, 255, 10, 33};
    int preSeq = sequence_get_count();
    for (int i = 0; i < ((MAX_SEQUENCES - preSeq) - 2); i++)
    {
        define_and_register_sequence_super(4, 8, &sequence[0]);
    }
    ARE_N_SEQUENCES_REGISTERED(MAX_SEQUENCES - 2);
    // Try to register RGB seq
    uint32_t seq[1] = {C_RED};
    int32_t seqId = rgb_sequence_register(1, 1, seq);
    LONGS_EQUAL(-1, seqId);
    ARE_N_SEQUENCES_REGISTERED(MAX_SEQUENCES - 2);
}
