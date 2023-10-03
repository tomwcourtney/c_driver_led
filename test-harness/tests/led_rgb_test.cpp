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

    #define ARE_N_SEQUENCES_REGISTERED(num)\
            LONGS_EQUAL(num, sequence_get_count());
    #define ARE_N_RGB_SEQUENCES_REGISTERED(num)\
        LONGS_EQUAL(num, rgb_sequence_get_count());
    #define DOES_LED_HAVE_SEQUENCE(led_id, sequence_id)\
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
    int32_t register_rgb_led(pins_t redPin, pins_t greenPin , pins_t bluePin, bool enabled)
    {
        led_t new_led = {
            .enabled = enabled,
            .pinout = {.pin = 0},
            .sequence_id = -1,
            .sequence_idx = 0,
            .timer_count = 0,
            .sequence_initialized = false
        };
        return rgb_led_register(redPin, greenPin, bluePin, new_led);
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
    register_rgb_led({.pin = 0}, {.pin = 1} ,  {.pin = 2}, true);
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
    uint8_t length = 1;
    uint16_t period = 1000;
    uint32_t seq[1] = {C_WHITE};
    int32_t seqId = rgb_sequence_register(length, period, seq);
    // Check that 1 extra RGB sequences exits 
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount+1);
    // Check that 3 extra LED Sequnces exits 
    ARE_N_SEQUENCES_REGISTERED(preSeqCount+3);
    // Check that the 3 LED Sequences contain only 255
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId,&seqRedId,&seqGreenId,&seqBlueId);
    sequence_t * seq_obj_red   = sequence_get_from_id(seqRedId);
    sequence_t * seq_obj_green = sequence_get_from_id(seqGreenId);
    sequence_t * seq_obj_blue  = sequence_get_from_id(seqBlueId);
    CHECK(255 == seq_obj_red->sequence[0]);
    CHECK(255 == seq_obj_green->sequence[0]);
    CHECK(255 == seq_obj_blue->sequence[0]);
}

TEST(LEDRGBTest, Registering_red_rgb_sequences_registers_3_sequences)
{
    // Stuff 
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence 
    uint8_t length = 1;
    uint16_t period = 1000;
    uint32_t seq[1] = {C_RED};
    int32_t seqId = rgb_sequence_register(length, period, seq);
    // Check that 1 extra RGB sequences exits 
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount+1);
    // Check that 3 extra LED Sequnces exits 
    ARE_N_SEQUENCES_REGISTERED(preSeqCount+3);
    // Check that the 3 LED Sequences contain only 255
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId,&seqRedId,&seqGreenId,&seqBlueId);
    sequence_t * seq_obj_red   = sequence_get_from_id(seqRedId);
    sequence_t * seq_obj_green = sequence_get_from_id(seqGreenId);
    sequence_t * seq_obj_blue  = sequence_get_from_id(seqBlueId);
    CHECK(255 == seq_obj_red->sequence[0]);
    CHECK(0 == seq_obj_green->sequence[0]);
    CHECK(0 == seq_obj_blue->sequence[0]);
}

TEST(LEDRGBTest, Registering_green_rgb_sequences_registers_3_sequences)
{
    // Stuff 
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence 
    uint8_t length = 1;
    uint16_t period = 1;
    uint32_t seq[1] = {C_GRN};
    int32_t seqId = rgb_sequence_register(length, period, seq);
    // Check that 1 extra RGB sequences exits 
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount+1);
    // Check that 3 extra LED Sequnces exits 
    ARE_N_SEQUENCES_REGISTERED(preSeqCount+3);
    // Check that the 3 LED Sequences contain only 255
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId,&seqRedId,&seqGreenId,&seqBlueId);
    sequence_t * seq_obj_red   = sequence_get_from_id(seqRedId);
    sequence_t * seq_obj_green = sequence_get_from_id(seqGreenId);
    sequence_t * seq_obj_blue  = sequence_get_from_id(seqBlueId);
    CHECK(0 == seq_obj_red->sequence[0]);
    CHECK(255 == seq_obj_green->sequence[0]);
    CHECK(0 == seq_obj_blue->sequence[0]);
}

TEST(LEDRGBTest, Registering_blue_rgb_sequences_registers_3_sequences)
{
    // Stuff 
    uint32_t preRgbSeqCount = rgb_sequence_get_count();
    uint32_t preSeqCount = sequence_get_count();
    // Register RGB sequence 
    uint8_t length = 1;
    uint16_t period = 1;
    uint32_t seq[1] = {C_BLUE};
    int32_t seqId = rgb_sequence_register(length, period, seq);
    // Check that 1 extra RGB sequences exits 
    ARE_N_RGB_SEQUENCES_REGISTERED(preRgbSeqCount+1);
    // Check that 3 extra LED Sequnces exits 
    ARE_N_SEQUENCES_REGISTERED(preSeqCount+3);
    // Check that the 3 LED Sequences contain only 255
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId,&seqRedId,&seqGreenId,&seqBlueId);
    sequence_t * seq_obj_red   = sequence_get_from_id(seqRedId);
    sequence_t * seq_obj_green = sequence_get_from_id(seqGreenId);
    sequence_t * seq_obj_blue  = sequence_get_from_id(seqBlueId);
    CHECK(0 == seq_obj_red->sequence[0]);
    CHECK(0 == seq_obj_green->sequence[0]);
    CHECK(255 == seq_obj_blue->sequence[0]);
}

// Sequence can be assigned to rgb led
TEST(LEDRGBTest, sequencs_can_be_assigned_to_rgb_led)
{
    // Definine a sequence 
    uint8_t length = 1;
    uint16_t period = 1000;
    uint32_t seq[1] = {C_WHITE};
    int32_t seqId = rgb_sequence_register(length, period, seq);
    // Define an RGB led 
    int32_t ledId = register_rgb_led({.pin = 0}, {.pin = 1} ,  {.pin = 2}, true);
    // Assign Sequence
    led_status_t assignStatus = rgb_assign_sequence(ledId,seqId);
    LONGS_EQUAL(LED_OK, assignStatus);
    // Check each led has a sequence assigned 
    int32_t ledRedId = {0}, ledGreenId = {0}, ledBlueId = {0};
    rgb_led_get_ids_from_id(ledId, &ledRedId, &ledGreenId, &ledBlueId);
    int32_t seqRedId = {0}, seqGreenId = {0}, seqBlueId = {0};
    rgb_sequence_get_ids_from_id(seqId,&seqRedId,&seqGreenId,&seqBlueId);
    DOES_LED_HAVE_SEQUENCE(ledRedId , seqRedId);
    DOES_LED_HAVE_SEQUENCE(ledGreenId,seqGreenId);
    DOES_LED_HAVE_SEQUENCE(ledBlueId,seqBlueId);
}


/* TODO
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