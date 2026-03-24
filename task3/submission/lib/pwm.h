#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/**
 * @brief Initializes the PWM peripheral on PD4 (TIM2_CH1).
 * @param frequency: Desired frequency in Hz (e.g., 1000 for 1kHz). 
 */
void pwm_init(uint32_t frequency);

/**
 * @brief Updates the PWM duty cycle. 
 * @param duty_cycle: Percentage value (0 to 100).
 */
void pwm_set_duty(uint8_t duty_cycle);

/**
 * @brief Starts the PWM output. 
 */
void pwm_start(void);

/**
 * @brief Stops the PWM output. 
 */
void pwm_stop(void);

#endif