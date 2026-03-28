# Evidence of Completion - Project-6 (LED Pattern Sequencer)
**Student Name:** Varun Raj  
**Roll Number:** SC23B152  
**Device:** VSDSquadron Mini (CH32V003)

---

## 📽️ 1. Hardware Demonstration Video
**[Link to Video Demo (YouTube/Google Drive)]** *(The video shows the RESET handshake, pattern loading, and synchronized LED blinking.)*

---

## 📸 2. UART Terminal Interaction (PuTTY Logs)
This screenshot verifies the **115200 baud UART Driver** and the **Command Parser** logic.

> **[INSERT_PUTTY_SCREENSHOT_HERE]** > *Proof of interaction showing `load`, `play`, and `stop` commands with their respective feedback.*

---

## 📸 3. Project Structure & Organization
This screenshot proves compliance with the **Task-4** folder structure and the separation of drivers (`lib/`) from application logic (`app/`).

> **[INSERT_VSCODE_EXPLORER_SCREENSHOT_HERE]** > *Proof that main.c is in the root and drivers are properly isolated in the /lib folder.*

---

## 📸 4. Hardware Setup Photo
A clear view of the **VSDSquadron Mini** and the **WCH-LinkE** connection used for debugging.

> **[INSERT_HARDWARE_PHOTO_HERE]** > *Showing the board powered and the onboard LED (PD0) in an active state.*

---

## 🛠️ 5. Technical Verification

### Test Case: TIM2 Interrupt Precision
* **Configuration:** 48MHz Clock / 500 Prescaler / 48000 Period.
* **Result:** A deterministic 500ms delay between sequence steps.
* **Verification:** Verified by timing the bit transitions against a stopwatch in the demo video.

### Test Case: Command Parsing
* **Input:** `load 10101100`
* **Result:** The sequencer successfully stored the 8-bit string into the `pattern_buffer` and reset the `current_index`.
