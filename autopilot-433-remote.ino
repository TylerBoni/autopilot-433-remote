/*
 * ESP32 433MHz Transceiver - SYN115/SYN480R Modules
 * This code handles both transmission and reception of 433MHz signals using RCSwitch
 * Transmitter data pin connected to GPIO 4
 * Receiver data pin connected to GPIO 16 (RX2)
 */

#include <RCSwitch.h>

// Pin configuration
const int TX_PIN = 4;     // Using GPIO 4 for transmitter
const int RX_PIN = 16;    // Using GPIO 16 (RX2) for receiver
const int LED_PIN = 2;    // Built-in LED

// Configuration constants
const bool ENABLE_REPEAT_SIGNALS = false;  // Whether to show repeated received signals
const int REPEAT_COUNT = 60;             // Number of times to repeat transmission
const int REPEAT_DELAY = 100;             // Delay between repeated transmissions (ms)

// Remote signal configuration
const unsigned long REMOTE_CODES[] = {
    230821,  // Button 1
    211124,  // Button 2
    880810,  // Button 3
    190728   // Button 4
};
const int REMOTE_BIT_LENGTH = 24;  // Signal bit length
const int REMOTE_PROTOCOL = 1;     // Signal protocol
const int REMOTE_DELAY = 400;      // Signal pulse delay (microseconds)

// RCSwitch instances
RCSwitch txSwitch = RCSwitch();  // For transmitting
RCSwitch rxSwitch = RCSwitch();  // For receiving

// Last received signal data
struct {
    unsigned long value = 0;
    unsigned long timestamp = 0;
} lastSignal;

void setLED(bool state) {
    digitalWrite(LED_PIN, state ? HIGH : LOW);
}

void flashLED() {
    setLED(true);
    delay(50);
    setLED(false);
}

void setup() {
    delay(1000);  // Wait before initializing Serial
    Serial.begin(115200);
    
    while (!Serial) {
        delay(10);
    }

    pinMode(LED_PIN, OUTPUT);
    setLED(false);

    Serial.println("\nESP32 433MHz Transceiver Started");
    Serial.println("Using RCSwitch library for signal transmission and reception");

    // Configure transmitter
    txSwitch.enableTransmit(TX_PIN);
    txSwitch.setPulseLength(REMOTE_DELAY);
    txSwitch.setProtocol(REMOTE_PROTOCOL);

    // Configure receiver
    rxSwitch.enableReceive(RX_PIN);

    // Print configuration
    Serial.println("\n=== Transmitter Configuration ===");
    Serial.printf("Bit Length: %d\n", REMOTE_BIT_LENGTH);
    Serial.printf("Protocol: %d\n", REMOTE_PROTOCOL);
    Serial.printf("Pulse Length: %d microseconds\n", REMOTE_DELAY);
    Serial.println("\nReady to transmit and receive!");
    Serial.println("Send '1-4' to transmit the configured signals");
    Serial.println("Receiving is automatic - detected signals will be printed\n");
}

void loop() {
    // Handle serial commands
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd >= '1' && cmd <= '4') {
            int index = cmd - '1';
            sendClonedSignal(REMOTE_CODES[index]);
        }
    }

    // Handle received signals
    if (rxSwitch.available()) {
        handleReceivedSignal();
        rxSwitch.resetAvailable();
    }
}

void handleReceivedSignal() {
    unsigned long currentTime = millis();
    unsigned long value = rxSwitch.getReceivedValue();

    // Skip repeated signals if configured
    if (!ENABLE_REPEAT_SIGNALS && value == lastSignal.value) {
        return;
    }

    flashLED();

    // Print signal details
    Serial.println("\n=== Received Signal ===");
    if (lastSignal.timestamp > 0) {
        Serial.printf("Time since last signal: %lu ms\n", currentTime - lastSignal.timestamp);
    }
    
    Serial.println("Signal Details:");
    Serial.printf("  Value (DEC): %lu\n", value);
    Serial.printf("  Value (HEX): 0x%lX\n", value);
    Serial.print("  Value (BIN): ");
    printBinary(value, rxSwitch.getReceivedBitlength());
    Serial.println();
    Serial.printf("  Bit Length : %d\n", rxSwitch.getReceivedBitlength());
    Serial.printf("  Protocol   : %d\n", rxSwitch.getReceivedProtocol());
    Serial.printf("  Pulse Delay: %d microseconds\n", rxSwitch.getReceivedDelay());
    Serial.println("====================");

    // Update last signal data
    lastSignal.value = value;
    lastSignal.timestamp = currentTime;
}

void sendClonedSignal(unsigned long code) {
    Serial.printf("Sending code: %lu\n", code);
    
    setLED(true);
    for (int i = 0; i < REPEAT_COUNT; i++) {
        Serial.println(REPEAT_COUNT - i);
        txSwitch.send(code, REMOTE_BIT_LENGTH);
        delay(REPEAT_DELAY);
    }
    setLED(false);

    Serial.println("Signal transmission complete!\n");
}

void printBinary(unsigned long value, int bitLength) {
    for (int i = bitLength - 1; i >= 0; i--) {
        Serial.print((value >> i) & 1);
    }
}