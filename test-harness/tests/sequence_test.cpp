#include "CppUTest/TestHarness.h"
  

extern "C" 
{
    #include "../../inc/sequence.h"
    #include "../../inc/led.h"
}


TEST_GROUP(SEQTest) 
{
    void setup()
    {
        sequence_init();
    }

    void teardown()
    {
    }

    #define ARE_N_SEQUENCES_REGISTERED(num)\
        LONGS_EQUAL(num, sequence_get_count());


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
};


/********/
/* ZERO */
/********/

// Check that after the sequence is initialized that there are no registered sequences.
TEST(SEQTest, no_sequences_initialized_when_led_initialized)
{
    ARE_N_SEQUENCES_REGISTERED(0);
}

// make sure you can't get a sequence that doesn't exist
TEST(SEQTest, get_sequence_for_nonexistent_sequence_returns_null)
{
    sequence_t * test_seq = sequence_get_from_id(2);

    POINTERS_EQUAL(NULL, test_seq);
}

// sequence exists is false when sequence doesnt exist
TEST(SEQTest, sequence_doesnt_exist_when_not_registered)
{
    CHECK_FALSE(sequence_exists(0));
}

/*******/
/* ONE */
/*******/

// Sequence: Register one sequence
TEST(SEQTest, sequence_can_be_defined)
{
    uint32_t sequence_id = define_and_register_sequence(123);
    
    ARE_N_SEQUENCES_REGISTERED(1);
    CHECK(sequence_exists(sequence_id));
}

TEST(SEQTest, check_that_registered_sequence_matches_what_we_defined)
{
    // Define and register sequence
    uint8_t length = 3;
    uint16_t period = 1000;
    uint8_t arr[] = {LED_OFF, LED_ON, LED_OFF};
    uint32_t id = define_and_register_sequence_super(length, period, &arr[0]);

    // Get the registered sequence 
    sequence_t * seq_obj = sequence_get_from_id(id);
    CHECK(length == seq_obj->length);
    CHECK(period == seq_obj->period);
    CHECK(length == seq_obj->length);


    // Check sequece elements are equal 
    CHECK(arr[0] == seq_obj->sequence[0]);
    CHECK(arr[1] == seq_obj->sequence[1]);
    CHECK(arr[2] == seq_obj->sequence[2]);

   
}


/********/
/* MANY */
/********/

TEST(SEQTest, register_two_sequences)
{
    // Define and register sequence 0
    uint8_t length_0 = 3;
    uint16_t period_0 = 1000;
    uint8_t arr_0[] = {LED_OFF, LED_ON, LED_OFF};
    uint32_t id_0 = define_and_register_sequence_super(length_0, period_0, &arr_0[0]);

    // Define and register sequence 1
    uint8_t length_1 = 4;
    uint16_t period_1 = 2000;
    uint8_t arr_1[] = {LED_ON, LED_OFF, LED_ON};
    uint32_t id_1 = define_and_register_sequence_super(length_1, period_1, &arr_1[0]);

    // Get sequence 0
    sequence_t * seq_obj_0 = sequence_get_from_id(id_0);
    CHECK(length_0 == seq_obj_0->length);
    CHECK(period_0 == seq_obj_0->period);
    // Check sequece elements are equal 
    CHECK(arr_0[0] == seq_obj_0->sequence[0]);
    CHECK(arr_0[1] == seq_obj_0->sequence[1]);
    CHECK(arr_0[2] == seq_obj_0->sequence[2]);

    // Get sequence 1
    sequence_t * seq_obj_1 = sequence_get_from_id(id_1);
    CHECK(length_1 == seq_obj_1->length);
    CHECK(period_1 == seq_obj_1->period);
    // Check sequece elements are equal 
    CHECK(arr_1[0] == seq_obj_1->sequence[0]);
    CHECK(arr_1[1] == seq_obj_1->sequence[1]);
    CHECK(arr_1[2] == seq_obj_1->sequence[2]);
    CHECK(arr_1[3] == seq_obj_1->sequence[3]);

    ARE_N_SEQUENCES_REGISTERED(2);
}

// make sure you can't register a sequence when sequence array is full
TEST(SEQTest, register_max_sequences_plus_one_gives_error)
{
    uint8_t length = 3;
    uint16_t period = 100;
    uint8_t arr[] = {LED_OFF, LED_ON, LED_OFF};

    for (int i = 0; i < MAX_SEQUENCES; i++)
    {
        define_and_register_sequence_super(length, period, arr);
    }

    CHECK(-1 == define_and_register_sequence_super(length, period, arr));
}