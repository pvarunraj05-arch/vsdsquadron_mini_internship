#include <stdio.h>
#include "gpio.h"
#include "debug.h"
int main(void) {
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200); // Initialize UART 
    gpio_init();

    // Mandatory Startup Messages 
    printf("Board: VSDSquadron Mini\n");
    printf("Firmware: Task-2 Validation v1.0\n");

    int counter = 0;
    while(1) {
        printf("Counter: %d\n", counter++); // Counter for UART log 
        gpio_toggle(); // Call the API layer [cite: 75]
        Delay_Ms(1000);
    }
}