#include <stdio.h>
#include "gpio.h"

int main(void) {
    // Standard initialization for VSDSquadron Mini
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200); 

    gpio_init(); // Uses the API layer [cite: 54]

    // Mandatory Startup Message [cite: 29]
    printf("--- Board Bring-Up Successful ---\n");
    printf("Board: VSDSquadron Mini\n"); // [cite: 30]
    printf("Firmware Version: 1.0.1\n"); // [cite: 31]

    int counter = 0;
    while(1) {
        // Continuous counter [cite: 32]
        printf("Task 2 Counter: %d\n", counter++); 
        
        gpio_toggle(); // Toggles the physical pin [cite: 45]
        Delay_Ms(1000); 
    }
}
