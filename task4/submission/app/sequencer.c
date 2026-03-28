#include "sequencer.h"
#include <string.h>

static char pattern_buffer[MAX_PATTERN_LEN];
static int current_index = 0;
static int pattern_len   = 0;
static SequencerState state = IDLE;

void Sequencer_Init(void) {
    state         = IDLE;
    current_index = 0;
    pattern_len   = 0;
    memset(pattern_buffer, 0, MAX_PATTERN_LEN);
}

void Sequencer_ProcessCommand(char* cmd) {
    if (strncmp(cmd, "load ", 5) == 0) {
        strncpy(pattern_buffer, cmd + 5, MAX_PATTERN_LEN - 1);
        pattern_buffer[MAX_PATTERN_LEN - 1] = '\0';
        pattern_len   = strlen(pattern_buffer);
        current_index = 0;
        UART_SendString("Pattern Loaded: ");
        UART_SendString(pattern_buffer);
        UART_SendString("\r\n");
    }
    else if (strncmp(cmd, "play", 4) == 0) {
        if (pattern_len > 0) {
            current_index = 0;
            state = PLAYING;
            Timer_Start();
            UART_SendString("Sequence Started...\r\n");
        } else {
            UART_SendString("No pattern loaded! Use 'load 1010' first.\r\n");
        }
    }
    else if (strncmp(cmd, "stop", 4) == 0) {
        state = IDLE;
        Timer_Stop();
        My_GPIO_Write(LED_PIN, 0);
        UART_SendString("Sequence Stopped.\r\n");
    }
    else {
        UART_SendString("Unknown command. Use: load <pattern>, play, stop\r\n");
    }
}

void Sequencer_PlayTick(void) {
    if (state == PLAYING) {
        if (pattern_buffer[current_index] == '1') {
            My_GPIO_Write(LED_PIN, 1);
        } else {
            My_GPIO_Write(LED_PIN, 0);
        }
        current_index++;
        if (current_index >= pattern_len) {
            current_index = 0;
        }
    }
}