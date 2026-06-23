# Car Black Box System Using PIC16F877A

## Overview

The Car Black Box System is an embedded application developed using the PIC16F877A microcontroller to monitor, record, and manage vehicle events.

The system continuously logs vehicle events along with speed and timestamp information. Logged data is stored in external EEPROM and can be viewed, downloaded, or managed through a password-protected user interface.

This project simulates the functionality of an automotive black box used in modern vehicles for diagnostics, event tracking, and accident investigation.

---

## Features

* Real-Time Event Logging
* RTC-Based Time Stamping
* Vehicle Speed Monitoring
* Gear/Event Tracking
* External EEPROM Data Storage
* Password-Protected Access
* Event Log Viewer
* Log Download via UART
* Change Password Feature
* CLCD User Interface
* Menu Driven Navigation

---

## Hardware Used

| Component            | Purpose          |
| -------------------- | ---------------- |
| PIC16F877A           | Main Controller  |
| DS1307 RTC           | Real-Time Clock  |
| 24C02 EEPROM         | Event Storage    |
| Character LCD (16x2) | User Interface   |
| Potentiometer        | Speed Simulation |
| Digital Keypad       | User Input       |
| UART Interface       | Log Download     |
| Timer2               | System Timing    |

---

## Technologies Used

* Embedded C
* MPLAB X IDE
* XC8 Compiler
* I2C Communication
* UART Communication
* EEPROM Management
* RTC Interfacing
* State Machine Design

---

## Project Structure

```text
.
├── Demo Videos/
|  └── Video.mp4
|
├── main.c
├── main.h
├── car_black_box_def.c
├── car_black_box_def.h
├── clcd.c
├── clcd.h
├── i2c.c
├── i2c.h
├── ds1307.c
├── ds1307.h
├── EEprom.c
├── adc.c
├── adc.h
├── digital_keypad.c
├── digital_keypad.h
├── uart.c
├── timers.c
├── isr.c
└── README.md
```

---

## System Architecture

```text
                    +----------------+
                    | Digital Keypad |
                    +----------------+
                             |
                             v

+----------+      +--------------------+      +---------+
| DS1307   | ---> | PIC16F877A MCU     | ---> | CLCD    |
| RTC      |      | Car Black Box      |      | Display |
+----------+      +--------------------+      +---------+
                             |
                             |
                             v
                     +---------------+
                     | 24C02 EEPROM  |
                     +---------------+
                             |
                             v
                       +-----------+
                       | UART PC   |
                       +-----------+

                             ^
                             |
                     +---------------+
                     | ADC Speed     |
                     | Input         |
                     +---------------+
```

---
## Demo Video

[Watch Demo Video](Screencast from 2026-06-23 09-55-39.mp4)


---
## Working Principle

### Dashboard Mode

The dashboard continuously displays:

* Current Time
* Vehicle Event
* Current Speed

Example:

```text
TIME      EV SP
12:45:10  G2 45
```

---

### Event Logging

Whenever an event occurs:

* Collision
* Reverse Gear
* Gear Change

The system records:

```text
Time + Event + Speed
```

Example:

```text
124510G245
```

Stored inside external EEPROM.

---

### Login System

Access to logs is password protected.

Default Password:

```text
0000
```

Features:

* 3 Login Attempts
* User Blocking Mechanism
* Password Change Support

---

### Main Menu

```text
1. View Log
2. Clear Log
3. Download Log
4. Set Time
5. Change Password
```

---

## Functional Modules

### Dashboard Module

#### display_dashboard_screen()

* Displays speed, event and current time.

#### display_time()

* Updates RTC time on LCD.

#### get_time()

* Reads current time from DS1307 RTC.

---

### Event Logging Module

#### log_event()

* Creates event records.
* Combines timestamp, event and speed.

#### store_log()

* Stores records into EEPROM.

---

### Login Module

#### login_screen()

* Handles user authentication.
* Verifies password from EEPROM.

---

### Menu Module

#### menu_screen()

* Provides navigation through available options.

---

### View Log Module

#### view_log()

* Displays stored events from EEPROM.

---

### Clear Log Module

#### clear_log()

* Erases stored event history.

---

### Download Log Module

#### download_log()

* Sends stored logs through UART.

---

### Time Management Module

#### set_time()

* Updates RTC time settings.

---

### Password Module

#### change_password()

* Allows user to modify login password.

---

## Event Types

```text
ON  - System Start
C   - Collision
GN  - Neutral Gear
GR  - Reverse Gear
G1  - Gear 1
G2  - Gear 2
G3  - Gear 3
G4  - Gear 4
```

---

## EEPROM Log Format

```text
HHMMSS EV SP
```

Example:

```text
124510 G2 45
```

Meaning:

```text
Time  : 12:45:10
Event : Gear 2
Speed : 45 km/h
```

---

## Build Instructions

### Open Project

```text
MPLAB X IDE
```

### Compiler

```text
XC8 Compiler
```

### Build

```text
Clean and Build Project
```

### Program Device

```text
PIC16F877A
```

---

## Sample Execution

### Dashboard

```text
TIME      EV SP
12:45:10  G2 45
```

### View Log

```text
SL NO TIME   EV SP

01    12:45 G1 25
02    12:47 G2 35
03    12:50 C  40
```

---

## Development Environment

### Software

| Tool            | Purpose                   |
| --------------- | ------------------------- |
| MPLAB X IDE     | Project Development       |
| XC8 Compiler    | Embedded C Compilation    |
| PICSimLab       | Hardware Simulation       |
| PICGenios Board | Virtual Development Board |
| Cutecom         | Virtual UART to UART      |

### Target Platform

```text
PIC16F877A Microcontroller
PICSimLab Simulator
PICGenios Development Board
```

---

## Hardware Platform

This project was developed and tested using the **PICGenios Development Board** available in **PICSimLab**.

The PICGenios board provides virtual peripherals required for the implementation:

* PIC16F877A MCU
* Character LCD (CLCD)
* Matrix/Digital Keypad
* Potentiometer (ADC Input)
* UART Terminal
* I2C EEPROM
* DS1307 RTC
* LEDs and Switches

This environment enables complete firmware development and testing without physical hardware.

---

## Simulation Setup

### Simulator

```text
PICSimLab
```

### Development Board

```text
PICGenios
```

### Microcontroller

```text
PIC16F877A
```

### Peripherals Used

```text
CLCD
Digital Keypad
ADC (Potentiometer)
DS1307 RTC
24C02 EEPROM
UART Terminal
Timer2
```

---

## How to Run

1. Build the project using MPLAB X IDE and XC8 Compiler.
2. Generate the HEX file.
3. Open PICSimLab.
4. Select:

```text
Board : PICGenios
MCU   : PIC16F877A
```

5. Load the generated HEX file.
6. Run the simulation.
7. Interact using keypad, LCD, UART, and ADC controls.

---

## Screenshots

Add screenshots of:

* Dashboard Screen
* Login Screen
* Main Menu
* View Log Screen
* UART Download Screen
* PICSimLab PICGenios Board

These screenshots significantly improve project presentation on GitHub.





## Concepts Demonstrated

* Embedded C
* State Machine Design
* I2C Communication
* UART Communication
* RTC Interfacing
* EEPROM Interfacing
* ADC Reading
* Event Logging
* Password Authentication
* Modular Firmware Design

---

## Applications

* Automotive Event Recorders
* Fleet Monitoring Systems
* Vehicle Diagnostics
* Accident Investigation Systems
* Data Logging Devices
* Embedded Monitoring Systems

---

## Future Improvements

* SD Card Logging
* GPS Integration
* CAN Bus Support
* GSM Alert System
* Cloud Connectivity
* Accelerometer-Based Crash Detection
* Real Vehicle Deployment

---

## Learning Outcomes

Through this project, I gained practical experience in:

* PIC16F877A Programming
* Embedded Firmware Development
* EEPROM Management
* RTC Integration
* UART Communication
* I2C Protocol
* State Machine Architecture
* Real-Time Embedded Systems

---

## Author

**Darsh Patel**

Electronics & Communication Engineer

Firmware Engineer | Embedded Systems | IoT Developer

GitHub: https://github.com/darshpatel001

---

## License

This project is licensed under the MIT License.
