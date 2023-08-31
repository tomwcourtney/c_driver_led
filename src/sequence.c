#include "sequence.h"
#include <string.h>
#include <stdio.h>

sequence_t sequence = {-1};

static uint32_t count = 0;

void sequence_init()
{
    memset(&sequence, -1, sizeof(sequence_t));
    count = 0;
    return;
}

uint32_t sequence_get_count()
{
    return count;
}

sequence_status_t sequence_register(sequence_t _sequence)
{
    sequence = _sequence;
    count++;

    return SEQUENCE_OK;
}

bool sequence_exists(uint32_t sequence_id)
{
    if(sequence.id == sequence_id)
    {
        return true;
    }
    return false; 
}

 sequence_t * sequence_get_from_id(uint32_t sequence_id)
 {
    return &sequence;
 }