#include "sequence.h"
#include <string.h>
#include <stdio.h>

//sequence_t sequence = {0};
sequence_t sequences[MAX_SEQUENCES] = {0};

static uint32_t count = 0;

void sequence_init()
{
    for (int i = 0; i < MAX_SEQUENCES; i++)
    {
        memset(&(sequences[i]), 0, sizeof(sequence_t));
    }

    count = 0;
    return;
}

uint32_t sequence_get_count()
{
    return count;
}

int32_t sequence_register(sequence_t _sequence)
{
    if (count >= MAX_SEQUENCES)
    {
        return -1;
    }

    sequences[count] = _sequence;

    // we increment in the return statement because we want to return the value of count BEFORE incrementing
    // as this is the actual id of the sequence
    return count++;
}

bool sequence_exists(uint32_t sequence_id)
{
    if(sequence_id < count)
    {
        return true;
    }
    return false; 
}

 sequence_t * sequence_get_from_id(uint32_t sequence_id)
 {
    if (!sequence_exists(sequence_id))
    {
        return NULL;
    }
    
    return &(sequences[sequence_id]);
 }