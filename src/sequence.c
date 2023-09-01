#include "sequence.h"
#include <string.h>
#include <stdio.h>

sequence_t sequence = {0};

static uint32_t count = 0;

void sequence_init()
{
    memset(&sequence, 0, sizeof(sequence_t));
    count = 0;
    return;
}

uint32_t sequence_get_count()
{
    return count;
}

int32_t sequence_register(sequence_t _sequence)
{
    sequence = _sequence;
    count++;

    return count;
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
    return &sequence;
 }