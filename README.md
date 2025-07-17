# Nautinect Autopilot 433MHz Remote Control

This project provides a way to code generic 433mhz remotes with codes provided for the Nautinect autopilot. Really you could use this to program any codes that you want.

The receiver is optional, but if you are building your own system you probably already have the receiver, and it makes testing the remote much easier.

## Hardware Requirements

- [ESP32 development board](https://amzn.to/44mlPRU)
- Wiring/Breadboard
- [433MHz receiver/transmitter modules](https://amzn.to/4nMU6S4)
- [433MHz remote control](https://amzn.to/4kq2HXL)

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

If you are having issues with the remote learning, I highly reccommend an antenna (wire curled in circles works fine) on the TX and pressing the antenna to the back of the remote. This is the only way I was able to make the remote learn the codes as the signal from the tx is too weak.

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
5. Open the serial monitor and follow instructions to send the code.

## Remote stup

### Enter Pairing mode

1. Hold A+B
2. Once light flashes, release B
3. Press B three times
4. Release A

### Pair button
1. Hold remote against TX antenna
2. Hold the button you would like to program.
3. The LED wil flash 3 times, do not let up. Once the LED starts flashing continuously, the programming is complete.
4. Repeat for other buttons.

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
