#include "gpio.h"

void gpio_init(void) {
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD; // Enable Clock
    GPIOD->CFGLR &= ~(0xf << (TEST_PIN * 4)); // Reset mode
    GPIOD->CFGLR |= (0x1 << (TEST_PIN * 4));  // Set as Output 
}

void gpio_toggle(void) {
    if(GPIOD->OUTDR & (1 << TEST_PIN)) GPIOD->BCR = (1 << TEST_PIN);
    else GPIOD->BSHR = (1 << TEST_PIN); // Toggle pin state 
}