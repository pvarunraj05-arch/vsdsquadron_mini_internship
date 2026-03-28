#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "../lib/gpio.h"
#include "../lib/timer.h"
#include "../lib/uart.h"

#define MAX_PATTERN_LEN 64

typedef enum {
    IDLE,
    PLAYING
} SequencerState;

void Sequencer_Init(void);
void Sequencer_ProcessCommand(char* cmd);
void Sequencer_PlayTick(void);

#endif