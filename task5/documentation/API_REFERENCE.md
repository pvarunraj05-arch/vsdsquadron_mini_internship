# API Reference

This document describes all public driver and application APIs in the LED Pattern Sequencer firmware.

---

## GPIO Driver (`lib/gpio.h`)

### `My_GPIO_Init`

```c
/**
 * @brief  Initializes PD0 as a push-pull output for LED control.
 *         Enables the GPIOD peripheral clock and configures the pin
 *         at 50 MHz slew rate. Must be called once before any
 *         My_GPIO_Write calls.
 *
 * @param  None
 * @return None
 *
 * @note   Only PD0 is configured. To add more LED pins, extend
 *         GPIO_InitStructure.GPIO_Pin with additional GPIO_Pin_x values.
 */
void My_GPIO_Init(void);
```

**Example:**
```c
My_GPIO_Init();  // call once in main before using My_GPIO_Write
```

---

### `My_GPIO_Write`

```c
/**
 * @brief  Sets or clears a GPIO pin on GPIOD.
 *
 * @param  pin    Pin number (0–7), corresponding to GPIOD bit position.
 *                Use LED_PIN (defined as 0) for PD0.
 * @param  state  1 to set the pin HIGH (LED ON), 0 to set LOW (LED OFF).
 * @return None
 *
 * @note   Uses GPIO_SetBits / GPIO_ResetBits with a bitmask of (1 << pin).
 *         Calling with pin > 7 produces undefined behavior.
 */
void My_GPIO_Write(uint8_t pin, uint8_t state);
```

**Example:**
```c
My_GPIO_Write(LED_PIN, 1);  // turn LED on
My_GPIO_Write(LED_PIN, 0);  // turn LED off
```

**Defined Constants:**
```c
#define LED_PIN   0      // PD0
#define GPIO_PORT GPIOD
```

---

## UART Driver (`lib/uart.h`)

### `UART_Init`

```c
/**
 * @brief  Initializes USART1 on PD5 (TX) and PD6 (RX).
 *         Configures GPIO alternate function, sets baud rate,
 *         8N1 frame format, no hardware flow control, and
 *         enables both TX and RX modes.
 *
 * @param  baudrate  Desired baud rate in bits per second (e.g., 115200).
 *                   Must match the terminal/host baud rate exactly.
 * @return None
 *
 * @note   Requires the system clock to be configured first (48 MHz via PLL).
 *         Calling UART_Init before SystemClock_Config will result in an
 *         incorrect baud rate divider and garbled output.
 */
void UART_Init(uint32_t baudrate);
```

**Example:**
```c
SystemClock_Config();   // must come first
UART_Init(115200);
```

---

### `UART_SendString`

```c
/**
 * @brief  Transmits a null-terminated string over USART1.
 *         Blocks until each byte is transferred to the TX shift register.
 *
 * @param  s  Pointer to a null-terminated character string.
 * @return None
 *
 * @note   Blocking call. Do not call from within a time-critical ISR.
 *         Maximum practical string length is limited by available stack.
 */
void UART_SendString(char *s);
```

**Example:**
```c
UART_SendString("Pattern Loaded\r\n");
```

---

### `UART_ReceiveString`

```c
/**
 * @brief  Non-blocking UART line reader. Accumulates characters into an
 *         internal static buffer until a newline or carriage return is
 *         received, then copies the result to the caller's buffer.
 *
 * @param  buffer  Pointer to caller-provided buffer of at least 64 bytes.
 *                 On return value 1, contains the received null-terminated string.
 * @return 1  A complete line was received and copied to buffer.
 *         0  No complete line yet; call again on the next loop iteration.
 *
 * @note   Uses an internal 64-byte static buffer. Maximum receivable line
 *         length is 63 characters. Characters beyond this are silently dropped.
 *         Both '\n' and '\r' are treated as line terminators.
 *         Empty lines (bare Enter key) are ignored and return 0.
 */
uint8_t UART_ReceiveString(char *buffer);
```

**Example:**
```c
char buf[64];
if (UART_ReceiveString(buf)) {
    Sequencer_ProcessCommand(buf);
}
```

---

## Timer Driver (`lib/timer.h`)

### `Timer_Init`

```c
/**
 * @brief  Configures TIM2 to generate periodic update interrupts at 500ms.
 *         Prescaler = 499, Period = 47999 → tick = 48MHz / 500 / 48000 = 2 Hz.
 *         Enables the TIM2 update interrupt in NVIC at highest priority.
 *         Does NOT start the timer — call Timer_Start() separately.
 *
 * @param  None
 * @return None
 *
 * @note   The ISR (TIM2_IRQHandler) calls Sequencer_PlayTick() on each tick.
 *         Requires __enable_irq() to be called in main for interrupts to fire.
 *         To change tick rate, adjust TIM_Prescaler and TIM_Period values.
 */
void Timer_Init(void);
```

**Tick Rate Calculation:**
```
Tick interval = (Prescaler + 1) * (Period + 1) / SystemClock
             = 500 * 48000 / 48,000,000
             = 500ms per tick
```

---

### `Timer_Start`

```c
/**
 * @brief  Enables TIM2 counter, starting the periodic ISR.
 *
 * @param  None
 * @return None
 */
void Timer_Start(void);
```

---

### `Timer_Stop`

```c
/**
 * @brief  Disables TIM2 counter, halting the periodic ISR.
 *
 * @param  None
 * @return None
 */
void Timer_Stop(void);
```

---

## Sequencer Application (`app/sequencer.h`)

### `Sequencer_Init`

```c
/**
 * @brief  Resets sequencer to IDLE state. Clears pattern buffer,
 *         resets index and length counters.
 *
 * @param  None
 * @return None
 *
 * @note   Must be called once in main before processing any commands.
 */
void Sequencer_Init(void);
```

---

### `Sequencer_ProcessCommand`

```c
/**
 * @brief  Parses and dispatches a received UART command string.
 *         Supported commands:
 *           "load <pattern>"  — stores pattern (e.g., "load 1010")
 *           "play"            — starts timer-driven pattern playback
 *           "stop"            — halts playback and turns LED off
 *
 * @param  cmd  Null-terminated command string from UART.
 * @return None
 *
 * @note   Pattern must consist of '0' and '1' characters only.
 *         Maximum pattern length is 63 characters (MAX_PATTERN_LEN - 1).
 *         Calling "play" without a loaded pattern sends an error message.
 *         Unrecognized commands send a usage hint over UART.
 */
void Sequencer_ProcessCommand(char* cmd);
```

**Command Table:**

| Command | Example | Response |
|---|---|---|
| `load <pattern>` | `load 1010` | `Pattern Loaded: 1010` |
| `play` | `play` | `Sequence Started...` |
| `stop` | `stop` | `Sequence Stopped.` |
| unknown | `foo` | `Unknown command. Use: load <pattern>, play, stop` |

---

### `Sequencer_PlayTick`

```c
/**
 * @brief  Advances the sequencer by one step. Called from TIM2_IRQHandler.
 *         Reads the current pattern character, drives LED accordingly,
 *         then increments index. Wraps to 0 at end of pattern (loop).
 *
 * @param  None
 * @return None
 *
 * @note   This function must ONLY be called from within the TIM2 ISR.
 *         Calling it from main loop context will cause timing inconsistency.
 *         Has no effect if sequencer state is IDLE.
 */
void Sequencer_PlayTick(void);
```
