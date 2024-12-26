#ifndef LU9685_H
#define LU9685_H

#include <Wire.h>

// Default configuration constants
#define DEFAULT_PWM_SPEED 50
#define DEFAULT_ADDRESS 0x80
#define DEFAULT_SDA A4
#define DEFAULT_SCL A5

// Global variables
uint8_t PWM_SPEED = DEFAULT_PWM_SPEED;
uint8_t ADDRESS = DEFAULT_ADDRESS;

// Function to write to a register
bool writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission() == 0);  // Return success/failure
}

// Initialization function for LU9685
void initializeLU9685(uint8_t address = DEFAULT_ADDRESS, uint8_t SDA = DEFAULT_SDA, uint8_t SCL = DEFAULT_SCL) {
    if (address > 0x80) {
        address = DEFAULT_ADDRESS;  // Clamp address to default if out of range
    }
    ADDRESS = address;

    Wire.begin(SDA, SCL);
}

// Function to control a single servo
void setServoPosition(uint8_t servo, uint8_t degree) {
    if (servo >= 20 || degree > 180) {
        return;  // Invalid parameters, exit
    }
    writeRegister(servo, degree);
}

// Function to control all servos with an array of degrees
void setAllServos(uint8_t address, const int *degrees, uint8_t length) {
    if (length > 20) {
        return;  // Ensure array length does not exceed maximum servo channels
    }
    Wire.beginTransmission(address);
    Wire.write(0xFD);  // Command to write to all servos
    for (uint8_t i = 0; i < length; i++) {
        Wire.write(degrees[i] > 180 ? 180 : degrees[i]);  // Clamp values
    }
    Wire.endTransmission();
}

// Function to control all servos with direct degree inputs
void DCAS(uint8_t d1 = 0, uint8_t d2 = 0, uint8_t d3 = 0, uint8_t d4 = 0, uint8_t d5 = 0, //DirectControlAllServos
                             uint8_t d6 = 0, uint8_t d7 = 0, uint8_t d8 = 0, uint8_t d9 = 0, uint8_t d10 = 0,
                             uint8_t d11 = 0, uint8_t d12 = 0, uint8_t d13 = 0, uint8_t d14 = 0, uint8_t d15 = 0,
                             uint8_t d16 = 0, uint8_t d17 = 0, uint8_t d18 = 0, uint8_t d19 = 0, uint8_t d20 = 0) {
    uint8_t degrees[20] = {d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20};
    for (uint8_t i = 0; i < 20; i++) {
        degrees[i] = (degrees[i] > 180) ? 180 : degrees[i];  // Clamp values
    }

    Wire.beginTransmission(ADDRESS);
    Wire.write(0xFD);  // Command to write to all servos simultaneously
    for (uint8_t i = 0; i < 20; i++) {
        Wire.write(degrees[i]);
    }
    Wire.endTransmission();
}

// Function to reset all servos
void resetServos() {
    writeRegister(0xFB, 0xFB);  // Reset command
}

#endif
