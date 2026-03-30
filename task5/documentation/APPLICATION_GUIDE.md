# Application Guide

## Overview

The LED Pattern Sequencer application allows a user to define a binary pattern of LED states over UART and play it back in a loop at a fixed 500ms tick rate. The application is entirely driven by two mechanisms: a polling loop for command reception and a hardware timer interrupt for pattern playback.

---

## Application States

The sequencer operates as a two-state machine:

```
         load <pattern>
              |
         +----v----+        play (pattern_len > 0)       +---------+
         |         +-------------------------------------->|         |
  reset  |  IDLE   |                                      | PLAYING |
+------->|         |<--------------------------------------+         |
         |         |               stop                   |         |
         +---------+                                      +---------+
```

| State | Description | Timer | LED |
|---|---|---|---|
| `IDLE` | Waiting for commands. Pattern may be loaded but not playing. | Stopped | Off (forced low on entry) |
| `PLAYING` | Timer ISR fires every 500ms, advancing pattern index and driving LED. | Running | Controlled by pattern |

### State Transitions

| From | Event | To | Side Effect |
|---|---|---|---|
| IDLE | `play` received, pattern_len > 0 | PLAYING | Timer_Start(), current_index = 0 |
| PLAYING | `stop` received | IDLE | Timer_Stop(), LED forced off |
| Any | `load <pattern>` received | (unchanged) | Pattern buffer updated, index reset |

---

## Task Flow

### Main Loop

The main loop runs continuously and does one thing: poll for a complete UART line. When a line is received, it is dispatched to the command processor. The loop has no blocking delays, so UART input is processed within one polling cycle (~microseconds).

```c
while(1) {
    char buf[64];
    if (UART_ReceiveString(buf)) {
        Sequencer_ProcessCommand(buf);
    }
}
```

### Timer ISR (500ms tick)

The ISR runs independently of the main loop. On each tick, it reads the current pattern character and drives the LED accordingly, then advances the index. If the index reaches the end of the pattern, it wraps to zero, creating a continuous loop.

---

## Timing Behavior

| Parameter | Value |
|---|---|
| Tick interval | 500ms |
| Timer clock source | TIM2, fed by APB1 (48 MHz) |
| Prescaler | 499 |
| Period | 47999 |
| Effective tick rate | 2 Hz |

For a pattern `1010` (4 characters), one full cycle takes 4 × 500ms = **2 seconds**.

To change the playback speed, modify in `timer.c`:
```c
TIM_TimeBaseStructure.TIM_Prescaler = 499;   // increase to slow down
TIM_TimeBaseStructure.TIM_Period    = 47999; // increase to slow down
```

For a 250ms tick (4 Hz), use Prescaler = 249, Period = 47999.

---

## How Drivers Are Orchestrated

The application layer calls drivers but never accesses hardware registers directly:

| Application action | Driver call |
|---|---|
| Turn LED on | `My_GPIO_Write(LED_PIN, 1)` |
| Turn LED off | `My_GPIO_Write(LED_PIN, 0)` |
| Start playback | `Timer_Start()` |
| Stop playback | `Timer_Stop()` |
| Send response | `UART_SendString(...)` |

The timer driver calls back into the application via a forward declaration (`extern void Sequencer_PlayTick(void)`) in `timer.c`. This avoids a circular include between the driver and application layers.

---

## Command Parser Details

`Sequencer_ProcessCommand()` uses `strncmp` for prefix matching:

| Check | Method | Reason |
|---|---|---|
| `load ` (with space) | `strncmp(cmd, "load ", 5)` | Separates command from argument |
| `play` | `strncmp(cmd, "play", 4)` | Matches even if trailing spaces present |
| `stop` | `strncmp(cmd, "stop", 4)` | Same |

The pattern argument is extracted as `cmd + 5` (everything after "load ").

---

## Edge Cases Handled

| Scenario | Behavior |
|---|---|
| `play` with no pattern loaded | Sends error: "No pattern loaded! Use 'load 1010' first." — does not start timer |
| `load` while already playing | Pattern is updated mid-playback. Index resets to 0. Playback continues with new pattern on next tick. |
| `stop` while already stopped | Safe — Timer_Stop() on a disabled timer is a no-op in the SDK. LED is set low. |
| Pattern longer than 63 chars | Truncated silently by `strncpy` with `MAX_PATTERN_LEN - 1` limit |
| Characters other than '0'/'1' | Not validated — non-binary characters result in LED off (treated as '0') since only `== '1'` is checked |
| Empty Enter key press | `UART_ReceiveString` ignores lines with `i == 0`, preventing spurious command dispatches |
| Bare `\r` or `\n` | Both treated as line terminators — compatible with Windows (CRLF) and Unix (LF) terminals |
