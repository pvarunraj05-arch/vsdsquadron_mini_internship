# Changelog

All notable changes to the LED Pattern Sequencer firmware are documented here.

---

## v1.0.0 — Initial Release

### Added
- `SystemClock_Config()` in `main.c` — configures CH32V003 to run at 48 MHz using HSI oscillator and PLL multiplier (x6). Required for correct UART baud rate generation and timer tick accuracy.
- GPIO driver (`lib/gpio.c`) — initializes PD0 as push-pull output at 50 MHz slew rate. Exposes `My_GPIO_Write()` for LED state control.
- UART driver (`lib/uart.c`) — full USART1 initialization on PD5 (TX) and PD6 (RX). Implements blocking `UART_SendString()` and non-blocking line-accumulating `UART_ReceiveString()`.
- Timer driver (`lib/timer.c`) — TIM2 configured for 500ms periodic interrupt using prescaler=499 and period=47999 at 48 MHz. ISR marked with `__attribute__((interrupt("machine")))` for correct RISC-V interrupt entry/exit.
- Sequencer application (`app/sequencer.c`) — two-state machine (IDLE / PLAYING), pattern buffer up to 63 characters, UART command parser for `load`, `play`, `stop`. Pattern loops continuously until stopped.
- `__enable_irq()` call in `main.c` — explicitly enables global machine-mode interrupts, required on RISC-V targets where interrupts are disabled by default after reset.
- Startup delay in `main.c` — 2,000,000-cycle busy wait after peripheral init to allow WCH-Link USB-Serial bridge to enumerate before the first UART transmission.
- Error handling — `play` with no loaded pattern returns a usage error; unknown commands return a hint message; empty UART lines are silently ignored.

### Fixed
- Resolved circular include between `timer.c` and `sequencer.h` by replacing `#include "../app/sequencer.h"` in `timer.c` with `extern void Sequencer_PlayTick(void)` forward declaration.
- Fixed `UART_ReceiveString` buffer corruption caused by a static index `i` being shared across calls with different buffer pointers. Replaced with a persistent internal static buffer with explicit copy-out on line completion.
- Fixed missing `Sequencer_Init()` and `My_GPIO_Init()` calls in `main.c` that caused undefined sequencer state and non-functional LED output on first boot.
- Fixed `platformio.ini` `include_dir` pointing only to `lib/`, which prevented `sequencer.h` from resolving `timer.h` and `gpio.h` includes. Changed to `task4/submission` root.
- Fixed Windows Defender / RAV Endpoint Protection blocking `riscv-none-embed-ar` during library indexing by adding `.platformio` folder to antivirus exclusions.

### Known Limitations
- Pattern input is not validated — characters other than '0' and '1' are accepted and treated silently as '0' (LED off).
- Only a single LED on PD0 is supported. Multi-LED parallel output is not implemented.
- Playback speed is fixed at 500ms per step. Runtime speed adjustment via UART is not supported in this version.
