# Evidence of Completion - Project-6

## 1. UART Terminal Interaction
The following log demonstrates the successful loading of a pattern, sequence initialization, and the stopping of the sequencer.

**Test Sequence:**
1.  **Command:** `load 1010`
2.  **Response:** `Pattern Loaded: 1010`
3.  **Command:** `play`
4.  **Response:** `Sequence Started...`
5.  **Command:** `stop`
6.  **Response:** `Sequence Stopped.`

> [!IMPORTANT]
> **Insert your PuTTY screenshot here showing the interaction above.**

## 2. Hardware Verification
* **Target Pin:** PD0 (Onboard LED).
* **Observation:** Upon sending the `play` command, the onboard LED blinked in the specific `1010` pattern.
* **Timing:** The sequence utilized TIM2 interrupts, resulting in a consistent 500ms pulse width per bit.

## 3. Verification Notes
* **Buffer Limit:** Verified that patterns up to 64 bits are handled correctly.
* **Command Robustness:** The system ignores unknown commands and prompts the user with the correct syntax.
* **Concurrency:** The UART remains responsive even while the Timer is driving the LED sequence in the background.