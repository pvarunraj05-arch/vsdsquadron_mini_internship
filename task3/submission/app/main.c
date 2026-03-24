#include "pwm.h"
#include "debug.h" // For UART printf

int main(void) {
    // System Initialization
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200); // 

    printf("Task-3: PWM Driver Demo Started\r\n");

    // Initialize PWM at 1kHz using Library API 
    pwm_init(1000); 
    pwm_start();

    uint8_t brightness = 0;
    int8_t fade_amount = 5;

    while(1) {
        // Update brightness via Library API 
        pwm_set_duty(brightness);

        // UART Logging [cite: 50, 75]
        printf("LED Brightness: %d%%\r\n", brightness);

        // Update fade logic
        brightness += fade_amount;
        if (brightness <= 0 || brightness >= 100) {
            fade_amount = -fade_amount; // Reverse direction
        }

        Delay_Ms(100); // 100ms for smooth breathing effect
    }
}