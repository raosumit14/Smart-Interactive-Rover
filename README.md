# Smart Interactive Rover

## Overview
The Smart Interactive Rover is an ESP32 based Bluetooth controlled robotic vehicle designed for interactive operation and future AI integration. The rover uses FreeRTOS multitasking, OLED based expressions, ultrasonic sensing, and wireless control to create an intelligent embedded platform.

## Features

- Bluetooth control using ESP32
- OLED display for facial expressions
- HC-SR04 ultrasonic obstacle detection
- L293D motor driver based motor control
- FreeRTOS multitasking
- Real-time sensor monitoring
- Modular architecture for future AI integration

## Hardware Components

| Component | Quantity |
|------------|------------|
| ESP32 | 1 |
| HC-SR04 Ultrasonic Sensor | 1 |
| OLED Display | 1 |
| L293D Motor Driver IC | 1 |
| DC Motors | 2 |
| Robot Chassis | 1 |
| Battery Pack | 1 |

## Software Used

- ESP-IDF
- FreeRTOS
- C Programming
- VS Code

## Working Principle

1. Smartphone sends commands via Bluetooth.
2. ESP32 receives and processes commands.
3. Motors are driven through the L293D motor driver.
4. Ultrasonic sensor continuously measures obstacle distance.
5. OLED displays system status and expressions.
6. FreeRTOS manages concurrent tasks for smooth operation.

## FreeRTOS Tasks

### Motor Control Task
Controls rover movement.

### Ultrasonic Sensor Task
Measures obstacle distance.

### OLED Display Task
Updates facial expressions and status.

### Bluetooth Task
Receives commands from smartphone.

## Project Images

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/edf6323d-dc76-4a69-8250-d74dc37a871b" />
<img width="4160" height="3120" alt="WhatsApp Image 2026-06-10 at 9 28 38 AM" src="https://github.com/user-attachments/assets/fde44919-033d-472c-add9-0707e419bf6c" />



## Demo Video

(https://youtu.be/qZVmUPh_stM)

## Future Improvements

- Voice control
- AI integration

## Author

Sumit Rao
