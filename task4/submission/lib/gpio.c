#include "gpio.h"

void My_GPIO_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void My_GPIO_Write(uint8_t pin, uint8_t state) {
    if (state) GPIO_SetBits(GPIO_PORT, (1 << pin));
    else       GPIO_ResetBits(GPIO_PORT, (1 << pin));
}