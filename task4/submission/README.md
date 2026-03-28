# Project-6: LED Pattern Sequencer (Capstone)

## Project Overview
This project implements a scriptable LED pattern sequencer for the **VSDSquadron Mini** (CH32V003). Users can send custom bit-patterns (e.g., `101100`) via a UART terminal, which are then stored in a buffer and played back on the onboard LED (PD0). The timing of the sequence is managed by hardware interrupts to ensure precise playback without blocking the main application loop.

---

## Hardware & Drivers Used
* **GPIO**: Controls the onboard LED on pin **PD0**.
* **UART**: Provides a serial interface (TX: PD5, RX: PD6) for user commands at **115200 baud**.
* **Timer (TIM2)**: Generates hardware interrupts every **500ms** to drive the sequencer "tick."
* **System Clock**: Configured for high-speed operation (PLL).

---

## API Summary
### Application Layer (`app/sequencer.h`)
* `void Sequencer_Init()`: Resets the buffer and state machine.
* `void Sequencer_ProcessCommand(char* cmd)`: Parses UART strings for `load`, `play`, and `stop`.
* `void Sequencer_PlayTick()`: The "heartbeat" function called by the Timer ISR to update the LED.

### Driver Layer (`lib/`)
* `void My_GPIO_Write(uint8_t pin, uint8_t state)`: Low-level pin control.
* `void UART_SendString(char *s)`: Transmits feedback to the user terminal.
* `void Timer_Start()` / `Timer_Stop()`: Controls the sequence timing engine.

---

## How to Demo
1.  Connect to the board via PuTTY/Serial Monitor at **115200 baud**.
2.  Press the physical **RESET** button on the VSDSquadron Mini.
3.  Type `load 10101100` and press **Enter**.
4.  Type `play` and press **Enter** to start the sequence.
5.  Type `stop` to end the playback and turn off the LED.