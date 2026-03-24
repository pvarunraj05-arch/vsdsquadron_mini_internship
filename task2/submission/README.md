# Task 2: VSDSquadron Mini Board Bring-Up

## Implementation Overview
For this task, I successfully transitioned from host-based development to real hardware execution on the VSDSquadron Mini RISC-V board. I implemented a layered firmware architecture where the application logic in `main.c` remains independent of hardware registers

## GPIO Mapping & Validation
* **GPIO Pin Chosen**: PD4.
* **Reason**: This pin is clearly labeled on the board silkscreen and is easily accessible for probing/LED connection.
* **Firmware Mapping**: The code uses GPIO Number 4, which matches the CH32V003 datasheet mapping for physical pin PD4.
***API Layer**: I created `gpio.c` and `gpio.h` to provide `gpio_init()` and `gpio_toggle()` functions, ensuring `main.c` contains no direct register manipulation.

## UART Communication
* **Configuration**: 115200 Baud Rate.
* **Message Description**: On reset, the board prints a startup header including the board name and firmware version . It then enters a loop printing a continuously increasing counter to validate persistent communication.

## How to Build and Flash
1. Open the project folder in VS Code with PlatformIO installed.
2. Ensure the `platformio.ini` file is in the root directory.
3. Connect the VSDSquadron Mini board via USB.
4. Click the **Checkmark (Build)** icon in the bottom status bar.
5. Click the **Right Arrow (Upload)** icon to flash the firmware.
6. Open the **Serial Monitor** to view the UART output.