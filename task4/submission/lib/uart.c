#include "uart.h"

void UART_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOD, ENABLE);

    // PD5 as TX
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // PD6 as RX
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = baudrate;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void UART_SendString(char *s) {
    while (*s) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s++);
    }
}

uint8_t UART_ReceiveString(char *buffer) {
    static int i = 0;
    static char internal_buf[64];

    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
        char c = USART_ReceiveData(USART1);
        if (c == '\n' || c == '\r') {
            if (i > 0) {
                internal_buf[i] = '\0';
                i = 0;
                int j = 0;
                while (internal_buf[j]) { buffer[j] = internal_buf[j]; j++; }
                buffer[j] = '\0';
                return 1;
            }
        } else if (i < 63) {
            internal_buf[i++] = c;
        }
    }
    return 0;
}