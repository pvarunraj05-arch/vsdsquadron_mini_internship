#ifndef UART_H
#define UART_H

#include <ch32v00x.h>

void UART_Init(uint32_t baudrate);
void UART_SendString(char *s);
uint8_t UART_ReceiveString(char *buffer);

#endif