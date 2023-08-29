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

/**
 * @brief Struct that stores sequences 
 * 
 */
typedef struct{
    uint32_t id;
    uint8_t sequence[50];
    uint8_t length;
    uint8_t period;
}sequence_t;

/**
 * @brief Sequence status variables 
 * 
 */
typedef enum{
    SEQUENCE_OK,
    SEQUENCE_ERROR
}sequence_status_t;

/**
 * @brief Initalises the sequece's giving you the ability to register and assign sequences 
 */
void sequence_init();

/**
 * @brief Registers a sequence to the module's state; an array of sequences.
 *
 * @param sequence - A sequence object to store in the module state.
 * @return sequence_status_t - error if object could not be registered.
 */
sequence_status_t sequence_register(sequence_t sequence);

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


#endif