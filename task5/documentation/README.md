# LED Pattern Sequencer — Scriptable over UART

## Project Summary

The LED Pattern Sequencer is an embedded firmware application for the VSDSquadron Mini (CH32V003F4U6) that allows a user to load arbitrary binary LED patterns over a UART serial interface and play them back at a fixed 500ms tick rate using a hardware timer interrupt. The system accepts human-readable commands (`load`, `play`, `stop`) from any serial terminal, making it scriptable and easy to demonstrate without specialized tooling.

## Target Hardware

| Field | Value |
|---|---|
| Board | VSDSquadron Mini |
| MCU | CH32V003F4U6 |
| Architecture | RISC-V (RV32EC) |
| Clock | 48 MHz (HSI + PLL x6) |
| RAM | 2 KB |
| Flash | 16 KB |
| LED Output Pin | PD0 |
| UART TX Pin | PD5 |
| UART RX Pin | PD6 |

## Supported Drivers

| Driver | Peripheral | Purpose |
|---|---|---|
| GPIO | GPIOD | LED output control |
| UART | USART1 | Serial command interface |
| Timer | TIM2 | Periodic ISR for pattern playback |

## Folder Structure

```
task4/submission/
├── main.c                  # Entry point, clock config, main loop
├── lib/
│   ├── gpio.h / gpio.c     # GPIO driver
│   ├── uart.h / uart.c     # UART driver
│   └── timer.h / timer.c   # Timer driver + ISR
└── app/
    ├── sequencer.h         # Sequencer public interface
    └── sequencer.c         # Pattern storage, command parser, tick handler

task5/
└── documentation/
    ├── README.md
    ├── API_REFERENCE.md
    ├── ARCHITECTURE.md
    ├── APPLICATION_GUIDE.md
    ├── DEMO_GUIDE.md
    └── CHANGELOG.md
```

## Quick Start

```bash
# 1. Clone and open project
git clone https://github.com/pvarunraj05-arch/vsdsquadron_mini_internship
cd vsdsquadron_mini_internship

# 2. Build and flash using PlatformIO
pio run --target upload

# 3. Open serial monitor (or use PuTTY at 115200 baud on your COM port)
pio device monitor

# 4. Press RESET on the board — you should see:
#    READY - Type 'load', 'play', or 'stop'

# 5. Run a demo sequence
load 1010
play
```
