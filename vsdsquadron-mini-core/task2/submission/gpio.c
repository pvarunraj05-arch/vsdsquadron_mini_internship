#include "gpio.h"

void gpio_init(void) {
    // Enable Clock for Port D
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
    
    // Configure Pin as Output (Push-Pull)
    // No direct register access should happen in main.c [cite: 49]
    GPIOD->CFGLR &= ~(0xf << (TEST_PIN * 4));
    GPIOD->CFGLR |= (0x1 << (TEST_PIN * 4)); 
}

void gpio_set(void) {
    GPIOD->BSHR = (1 << TEST_PIN); 
}

void gpio_clear(void) {
    GPIOD->BCR = (1 << TEST_PIN); 
}

void gpio_toggle(void) {
    if(GPIOD->OUTDR & (1 << TEST_PIN)) gpio_clear();
    else gpio_set();
}
