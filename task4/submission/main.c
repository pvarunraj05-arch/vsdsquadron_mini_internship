#include "lib/uart.h"
#include "lib/gpio.h"
#include "lib/timer.h"
#include "app/sequencer.h"

void SystemClock_Config(void) {
    RCC->CTLR |= (1 << 0);
    while (!(RCC->CTLR & (1 << 1)));

    RCC->CFGR0 &= ~(0xF << 18);
    RCC->CFGR0 |=  (0x4 << 18);
    RCC->CFGR0 &= ~(1 << 16);

    RCC->CTLR |= (1 << 24);
    while (!(RCC->CTLR & (1 << 25)));

    RCC->CFGR0 &= ~(0x3 << 0);
    RCC->CFGR0 |=  (0x2 << 0);
    while (((RCC->CFGR0 >> 2) & 0x3) != 0x2);
}

int main() {
    SystemClock_Config();
    UART_Init(115200);
    My_GPIO_Init();
    Sequencer_Init();

    for(volatile int i = 0; i < 2000000; i++);

    Timer_Init();
    __enable_irq();

    UART_SendString("READY - Type 'load', 'play', or 'stop'\r\n");

    while(1) {
        char buf[64];
        if(UART_ReceiveString(buf)) {
            Sequencer_ProcessCommand(buf);
        }
    }
}