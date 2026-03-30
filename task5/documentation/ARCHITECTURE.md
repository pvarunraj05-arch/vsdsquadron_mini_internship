# System Architecture

## High-Level Block Diagram

```
+---------------------------+          +-------------------+
|        HOST PC            |          |   VSDSquadron Mini|
|                           |          |   CH32V003F4U6    |
|  PuTTY / Serial Terminal  |          |                   |
|                           |  115200  |  +-------------+  |
|  > load 1010   --------+--+--UART--->+->| USART1      |  |
|  > play        --------+  |          |  | PD5(TX)     |  |
|  > stop        --------+  |          |  | PD6(RX)     |  |
|                           |          |  +------+------+  |
|  Pattern Loaded: 1010  <--+--UART----+---------|         |
|  Sequence Started...   <--+          |         v         |
+---------------------------+          |  +-------------+  |
                                       |  | Sequencer   |  |
                                       |  | (app layer) |  |
                                       |  +------+------+  |
                                       |         |         |
                                       |    state=PLAYING  |
                                       |         |         |
                                       |  +------v------+  |
                                       |  |   TIM2 ISR  |  |
                                       |  | (500ms tick)|  |
                                       |  +------+------+  |
                                       |         |         |
                                       |  +------v------+  |
                                       |  |    GPIOD    |  |
                                       |  |    PD0      |  |
                                       |  +------+------+  |
                                       |         |         |
                                       +---------+---------+
                                                 |
                                          [220Ω resistor]
                                                 |
                                               [LED]
                                                 |
                                               [GND]
```

---

## Layer Separation

The firmware is organized into three distinct layers:

### 1. Driver Layer (`lib/`)

Contains hardware abstraction for each peripheral. Each driver owns exactly one peripheral and exposes a minimal public API. Drivers have no knowledge of application logic.

| File | Peripheral | Responsibility |
|---|---|---|
| `gpio.c` | GPIOD | Pin direction setup, set/clear output |
| `uart.c` | USART1 | Baud rate init, blocking TX, non-blocking RX |
| `timer.c` | TIM2 | Periodic ISR setup, start/stop control |

### 2. Application Layer (`app/`)

Contains the sequencer logic. The application layer calls driver APIs but does not access hardware registers directly. It owns the state machine, pattern buffer, and command parser.

| File | Responsibility |
|---|---|
| `sequencer.c` | State management, command parsing, pattern playback |

### 3. Entry Point (`main.c`)

Responsible for system clock configuration, driver initialization ordering, enabling global interrupts, and running the main polling loop. It does not contain business logic.

---

## Data Flow

### Command Reception (main loop)

```
UART RX byte received
        |
        v
UART_ReceiveString() accumulates bytes
        |
        v (on '\r' or '\n')
Complete line copied to buf[64]
        |
        v
Sequencer_ProcessCommand(buf)
        |
        +---> "load 1010" --> store pattern, send ack
        +---> "play"      --> set state=PLAYING, Timer_Start()
        +---> "stop"      --> set state=IDLE, Timer_Stop(), LED off
        +---> unknown     --> send usage hint
```

### Pattern Playback (interrupt-driven)

```
TIM2 update interrupt fires every 500ms
        |
        v
TIM2_IRQHandler()
        |
        v
Sequencer_PlayTick()
        |
        v
Read pattern_buffer[current_index]
        |
        +---> '1' --> My_GPIO_Write(LED_PIN, 1)  [LED ON]
        +---> '0' --> My_GPIO_Write(LED_PIN, 0)  [LED OFF]
        |
        v
current_index++
        |
        v (if index >= pattern_len)
current_index = 0  [loop]
```

---

## Control Flow

```
main()
  |
  +-- SystemClock_Config()   [HSI + PLL → 48 MHz]
  +-- UART_Init(115200)
  +-- My_GPIO_Init()
  +-- Sequencer_Init()
  +-- startup delay
  +-- Timer_Init()
  +-- __enable_irq()
  +-- UART_SendString("READY...")
  |
  +-- while(1) ------------> UART_ReceiveString()
        |                          |
        |                    (returns 1 on full line)
        |                          |
        +<-------------------------+
        |
        +--> Sequencer_ProcessCommand()
                    |
              (if "play")
                    |
              Timer_Start() -----> TIM2 running
                                        |
                                   every 500ms
                                        |
                                   TIM2_IRQHandler()
                                        |
                                   Sequencer_PlayTick()
                                        |
                                   My_GPIO_Write()
```

---

## Architecture Decisions

**Why polling for UART RX instead of interrupt-driven?**
The CH32V003 has 2KB RAM and limited interrupt vectors. For a single-user command interface where commands are typed at human speed, polling in the main loop is sufficient and avoids the complexity of a double-buffered UART RX ISR. Latency is negligible.

**Why a separate app layer instead of putting everything in main?**
Separating the sequencer into `app/sequencer.c` makes it independently testable and replaceable. A different application (e.g., a PWM controller) could reuse the same `lib/` drivers without modification.

**Why TIM2 for the tick and not a software delay?**
A hardware timer interrupt decouples the playback timing from the main loop. With a software delay, the board cannot receive new UART commands while playing a sequence. With TIM2, the main loop remains responsive to `stop` commands at all times.
