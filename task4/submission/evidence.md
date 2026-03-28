# Evidence of Completion - Project-6 (LED Pattern Sequencer)
**Student Name:** Varun Raj  
**Roll Number:** SC23B152  
**Device:** VSDSquadron Mini (CH32V003)

---

## 📽️ 1. Hardware Demonstration Video
**[[Link to Video Demo (YouTube/Google Drive)](https://drive.google.com/file/d/1VMe5HvENXC5zY4u2LEpgyvrt9xXtelfK/view?usp=drivesdk)]** *(The video shows the RESET handshake, pattern loading, and synchronized LED blinking.)*

---

## 📸 2. UART Terminal Interaction (PuTTY Logs)
This screenshot verifies the **115200 baud UART Driver** and the **Command Parser** logic.

> **[INSERT_PUTTY_SCREENSHOT_HERE]**![alt text](<assets/Screenshot 2026-03-29 024142.png>) > *Proof of interaction showing `load`, `play`, and `stop` commands with their respective feedback.*

---

## 📸 3. Project Structure & Organization
This screenshot proves compliance with the **Task-4** folder structure and the separation of drivers (`lib/`) from application logic (`app/`).

> **[INSERT_VSCODE_EXPLORER_SCREENSHOT_HERE]** ![alt text](<assets/Screenshot 2026-03-29 023510.png>)> *Proof that main.c is in the root and drivers are properly isolated in the /lib folder.*

---

## 📸 4. Hardware Setup Photo
A clear view of the **VSDSquadron Mini** and the **WCH-LinkE** connection used for debugging.

> **[INSERT_HARDWARE_PHOTO_HERE]**![alt text](<assets/WhatsApp Image 2026-03-29 at 02.32.00.jpeg>) > *Showing the board powered and the onboard LED (PD0) in an active state.*

---

## 🛠️ 5. Technical Verification

### Test Case: TIM2 Interrupt Precision
* **Configuration:** 48MHz Clock / 500 Prescaler / 48000 Period.
* **Result:** A deterministic 500ms delay between sequence steps.
* **Verification:** Verified by timing the bit transitions against a stopwatch in the demo video.

### Test Case: Command Parsing
* **Input:** `load 10101100`
* **Result:** The sequencer successfully stored the 8-bit string into the `pattern_buffer` and reset the `current_index`.
