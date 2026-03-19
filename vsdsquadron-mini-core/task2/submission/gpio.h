#ifndef GPIO_H
#define GPIO_H

#include <ch32v00x.h>

// Define your physical pin based on the datasheet mapping
// Example: Silkscreen PD6 corresponds to Pin 6
#define TEST_PIN 6 

// GPIO API Layer
void gpio_init(void);
void gpio_set(void);
void gpio_clear(void);
void gpio_toggle(void);

#endif
