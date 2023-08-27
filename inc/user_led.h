#ifndef USER_LED_H
#define USER_LED_H

#include <stdint.h>

/**
 * @brief user defined structure which contains hardware specific pin information needed to toggle the pin
 * e.g. in STM32 uising HAL, pins_t would be defined as 
 * typdef struct {
    GPIO_TypeDef * port;
    uint16_t pin;
 } pins_t;
*/
// Need this forward declaration because there are function definitions in this file that use the structure but it will not be defined yet.
typedef struct{
    uint32_t pin;
}pins_t;

#endif