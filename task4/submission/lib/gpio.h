#ifndef GPIO_H
#define GPIO_H

#include <ch32v00x.h>

#define LED_PIN   0
#define GPIO_PORT GPIOD

void My_GPIO_Init(void);
void My_GPIO_Write(uint8_t pin, uint8_t state);

#endif