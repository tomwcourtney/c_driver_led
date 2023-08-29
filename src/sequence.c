#include "sequence.h"
#include <string.h>

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

sequence_status_t sequence_register(sequence_t sequence)
{
    // can you do this? how does compiler differentiate between the identifiers
    sequence = sequence;
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