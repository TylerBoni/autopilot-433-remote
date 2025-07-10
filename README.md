# Nautinect Autopilot 433MHz Remote Control

This project provides the code for an ESP32-based 433MHz remote control system for the Nautinect autopilot. It allows wireless control of the autopilot's heading adjustments using pre-programmed remote codes.

The receiver is optional, but if you are building your own system you probably already have the receiver, and it makes testing the remote much easier.

## Hardware Requirements

- ESP32 development board
- 433MHz receiver module 
- 433MHz remote control (pre-programmed with specific codes)
- SYN115/SYN480R modules compatible (others probably work)

## Wiring Instructions

### Receiver Connection
```
Receiver Pin | ESP32 Pin
-------------|----------
VCC          | 3.3V
GND          | GND
DATA (D0)    | GPIO 16 (RX2)
```

### Transmitter Connection (if using)
```
Transmitter Pin | ESP32 Pin
----------------|----------
VCC             | 3.3V
GND             | GND
DATA            | GPIO 4
```

### Antennas

If you are having issues with the remote learning, I highly reccommend an antenna on the TX and pressing the antenna to the back of the remote. This is the only wy I was able to make the remote learn the codes as the signal fro mthe tx is too weak.

## Remote Control Functions

The system responds to four pre-programmed codes that control the autopilot:

| id | Code   | Function                                     |
|----|--------|----------------------------------------------|
| 1  | 230821 | -1 degree when AP=on, move motor manually    |
| 2  | 211124 | -10 degrees when AP=on, move motor manually  |
| 3  | 880810 | +1 degree                                    |
| 3  | 190728 | +10 degrees                                  |

## Software Setup

1. Install the Arduino IDE
2. Install the ESP32 board support package
3. Install the RC-Switch library
4. Upload the code to your ESP32

## Configuration

The code includes several configurable parameters:

### RX:
- `ENABLE_REPEAT_SIGNALS`: Enable/disable processing of repeated signals (checks if signal is the same as last received before logging it)

### TX:
- `REPEAT_COUNT`: Number of times to repeat transmission (default: 60)
- `REPEAT_DELAY`: Delay between repeated transmissions in ms (default: 100)

## Serial Monitor Output

The system provides detailed feedback through the serial monitor (115200 baud):
- Signal reception details including timing, values, and protocol information
- Transmission confirmations
- System status messages

## Debugging

The onboard LED (GPIO 2) provides visual feedback:
- Flashes briefly when receiving a signal
- Stays on during transmission
- Off during idle state
