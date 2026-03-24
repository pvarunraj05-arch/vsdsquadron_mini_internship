#include "pwm.h"
#include "ch32v00x.h"

void pwm_init(uint32_t frequency) {
    // 1. Enable clocks for GPIOD and TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. Configure PD4 as Alternate Function Push-Pull (TIM2_CH1)
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 3. Timer Base Configuration
    uint16_t period = 1000; // Resolution of 1000 steps 
    uint32_t prescaler = (SystemCoreClock / (frequency * period)) - 1;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. PWM Mode Configuration for Channel 1
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // Start at 0%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
}

void pwm_set_duty(uint8_t duty_cycle) {
    if (duty_cycle > 100) duty_cycle = 100;
    // Map 0-100% to our 0-1000 period
    uint16_t pulse = duty_cycle * 10;
    TIM_SetCompare1(TIM2, pulse); // 
}

void pwm_start(void) {
    TIM_Cmd(TIM2, ENABLE); // 
}

void pwm_stop(void) {
    TIM_Cmd(TIM2, DISABLE); // 
}