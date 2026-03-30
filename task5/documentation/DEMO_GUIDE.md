# Demo Guide

This guide allows a reviewer to reproduce the LED Pattern Sequencer demo from scratch in under 5 minutes.

---

## Hardware Required

| Item | Specification |
|---|---|
| Board | VSDSquadron Mini (CH32V003F4U6) |
| LED | Any standard 5mm LED |
| Resistor | 220Ω |
| Jumper wires | 2× |
| USB cable | USB-C (for board power + WCH-Link programming) |

---

## Hardware Setup

Wire one external LED to the board as follows:

```
VSDSquadron Mini
  PD0 (Pin 8) ---[220Ω]--- LED(+) --- LED(-) --- GND
```

- Connect the **anode (long leg)** of the LED to the resistor
- Connect the **cathode (short leg)** to any GND pin on the board
- The onboard WCH-Link handles both programming and UART — no separate USB-Serial adapter needed

---

## Flashing Steps

1. Connect the board to your PC via USB-C
2. Open the project in VS Code with PlatformIO installed
3. Confirm `platformio.ini` has:
   ```ini
   board = vsdsquadronMini
   framework = noneos-sdk
   monitor_speed = 115200
   ```
4. Click **Upload** in PlatformIO (or run `pio run --target upload` in terminal)
5. Wait for:
   ```
   [SUCCESS] Took x.xx seconds
   ```

---

## UART Settings

Open PuTTY (or any serial terminal) with these exact settings:

| Setting | Value |
|---|---|
| Connection type | Serial |
| Port | COMx (check Device Manager — look for WCH USB-Serial) |
| Speed | 115200 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Local echo | Force On |
| Local line editing | Force On |

> **Important:** Open the terminal window **before** pressing RESET, otherwise you will miss the startup message.

---

## Demo Sequence

### Step 1 — Verify Board is Alive

Press the **RESET** button on the board. You should immediately see:

```
READY - Type 'load', 'play', or 'stop'
```

If nothing appears, check COM port number in Device Manager and confirm baud rate is 115200.

---

### Step 2 — Load a Pattern

Type the following and press Enter:

```
load 1010
```

Expected response:
```
Pattern Loaded: 1010
```

---

### Step 3 — Start Playback

Type:
```
play
```

Expected response:
```
Sequence Started...
```

**Expected hardware behavior:**
- LED turns ON for 500ms
- LED turns OFF for 500ms
- LED turns ON for 500ms
- LED turns OFF for 500ms
- Pattern repeats indefinitely

---

### Step 4 — Stop Playback

Type:
```
stop
```

Expected response:
```
Sequence Stopped.
```

**Expected hardware behavior:** LED turns off immediately and stays off.

---

### Step 5 — Try Different Patterns

| Command | Pattern | Expected LED Behavior |
|---|---|---|
| `load 1111` then `play` | All ON | LED stays on continuously |
| `load 0000` then `play` | All OFF | LED stays off |
| `load 1100` then `play` | ON-ON-OFF-OFF | 1s on, 1s off cycle |
| `load 10110100` then `play` | Complex | Mixed sequence, 4s full cycle |

---

### Step 6 — Test Error Handling

Type `play` without loading a pattern first (after a fresh reset):
```
play
```
Expected response:
```
No pattern loaded! Use 'load 1010' first.
```

Type an unknown command:
```
hello
```
Expected response:
```
Unknown command. Use: load <pattern>, play, stop
```

---

## Troubleshooting

| Symptom | Likely Cause | Fix |
|---|---|---|
| No READY message on reset | Missed startup message | Keep PuTTY open before pressing RESET |
| Garbled text in terminal | Wrong baud rate | Set PuTTY to exactly 115200 |
| PuTTY says "Access Denied" | Another app holds the COM port | Close PlatformIO device monitor first |
| LED never blinks | ISR not firing | Confirm `__enable_irq()` is in main.c |
| LED always on/off | Wrong pin | Confirm LED is wired to PD0 (Pin 8) |
| Commands typed but no response | UART RX issue | Check PD5/PD6 are not shorted |
