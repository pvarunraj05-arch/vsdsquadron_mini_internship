#ifndef GPIO_H
#define GPIO_H
#include <ch32v00x.h>

#define TEST_PIN 4 // Matches physical label PD6 on the board 

void gpio_init(void);
void gpio_toggle(void);
#endif