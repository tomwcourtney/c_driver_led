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

/**
 * @brief 
 * 
 */
typedef struct{
    uint32_t id;
    uint8_t sequence[50];
    uint8_t length;
    uint8_t period;
}sequence_t;

/**
 * @brief 
 * 
 */
typedef enum{
    SEQUENCE_OK,
    SEQUENCE_ERROR
}sequence_status_t;

void sequence_init();
sequence_status_t sequence_register(sequence_t sequence);
uint32_t sequence_get_count();


#endif