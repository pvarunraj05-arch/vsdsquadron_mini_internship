# Task-3: PWM Peripheral Driver Library

## Library Implemented
* **Peripheral:** PWM (Pulse Width Modulation) 
* **Target Board:** VSDSquadron Mini (RISC-V) 

## API List
* `void pwm_init(uint32_t frequency)`: Initializes Timer 2 and sets up PD4 as the PWM output pin.
* `void pwm_set_duty(uint8_t duty_cycle)`: Updates the compare register to change the LED brightness (0-100%)
* `void pwm_start(void)`: Starts the timer counter to begin PWM output.
* `void pwm_stop(void)`: Disables the timer counter.

## Application Description
This application demonstrates variable LED brightness control on pin PD4[cite: 49]. It uses the PWM library APIs to create a "breathing" effect by periodically incrementing and decrementing the duty cycle, with status updates logged over UART.

## How to Build and Flash
1. Open the project in VS Code with the PlatformIO extension.
2. Connect the VSDSquadron Mini via the LinkE debugger.
3. Use the PlatformIO "Build" button followed by the "Upload" button.

## UART Configuration
* **Baud Rate:** 115200 
* **Data Bits:** 8, Stop Bits: 1, Parity: None 