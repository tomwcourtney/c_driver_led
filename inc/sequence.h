/**
 * @file sequence.h
 * @brief Module for storing sequence information for a flashing LED.
 * @version 0.1
 * @date 2023-08-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SEQUENCES 64

/**
 * @brief Struct that stores sequences 
 * @param
 * @param
 * @param
 * @param
 */

#define MAX_SEQUENCE 100

typedef struct{
    uint8_t sequence[MAX_SEQUENCE];
    uint8_t length;
    uint32_t period;
}sequence_t;

/**
 * @brief Sequence status variables 
 * 
 */
typedef enum{
    SEQUENCE_OK,
    SEQUENCE_ERROR = -1
}sequence_status_t;

/**
 * @brief Initalises the sequece's giving you the ability to register and assign sequences 
 */
void sequence_init();

/**
 * @brief Registers a sequence to the module's state; an array of sequences.
 *
 * @param sequence - A sequence object to store in the module state.
 * @return int32_t - If successfully registered returns the ID of the sequence. If error
 * returns -1 (SEQUENCE_ERROR).
 */
int32_t sequence_register(sequence_t sequence);

/**
 * @brief Returns the current number of registered sequences 
 * 
 * @return uint32_t - Number of registered sequences.
 */
uint32_t sequence_get_count();

/**
 * @brief Checks if a sequence is registered.
 * 
 * @return bool - Returns true if sequence exists.
 */
bool sequence_exists(uint32_t sequence_id);

/**
 * @brief Returns the sequence if registed based off the inputted id
 * 
 * @param sequence_id - index of the sequence in the list of sequences.
 * 
 * @return sequence_t * - Returns pointer to object found. Else return NULL.
 */
 sequence_t * sequence_get_from_id(uint32_t sequence_id);

#endif