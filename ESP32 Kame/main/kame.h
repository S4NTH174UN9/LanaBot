#ifndef KAME_H
#define KAME_H

#include "LU9685.h"
#include <Wire.h>

unsigned long lastMoveTime = 0;
int moveStep = 0;
extern int lastmode = 0;
extern unsigned long stepDelay = 10;  // 120ms delay between steps
extern int tiltServo = 100;
extern int panServo = 60;

bool firstStep = true;
// Sets the robot to its default position
void DefaultPosition() {
  DCAS(0, 20, 52, 60, 52, 20, 52, 80, panServo, tiltServo, 52);
  delay(50);
  firstStep = true;
}

// Default stand position
void DefaultStand() {
  DCAS(0, 75, 52, 0, 52, 85, 52, 25, panServo, tiltServo, 52);
  delay(50);
  firstStep = true;
}

// Crab default position
void crabDefault() {
  DCAS(0, 20, 10, 60, 85, 20, 10, 80, panServo, tiltServo, 90);
  delay(50);
  firstStep = true;
}

// Crab stand position
void crabStand() {
  DCAS(0, 75, 10, 0, 85, 85, 10, 25, panServo, tiltServo, 90);
  delay(50);
  firstStep = true;
}

// Handles jump behavior
void jump() {
  for (int i = 0; i < 2; ++i) {
    DCAS(0, 20, 52, 60, 52, 20, 52, 80, panServo, tiltServo, 52);
    delay(150);
    DCAS(0, 80, 52, 0, 52, 85, 52, 20, panServo, tiltServo, 52);
    delay(150);
  }
  firstStep = true;
}

void yes() {
  for (int i = 0; i < 3; i++) {
    setServoPosition(8, 90);
    delay(50);
    setServoPosition(8, 30);
    delay(50);
  }
}

void no() {
  for (int i = 0; i < 3; i++) {
    setServoPosition(9, 120);
    delay(50);
    setServoPosition(9, 60);
    delay(50);
  }
}

void Dance() {
}

// Movement functions
void moveForward() {
  unsigned long currentTime = millis();

  if (currentTime - lastMoveTime >= stepDelay) {
    lastMoveTime = currentTime;

    switch (moveStep) {
      case 0:
        if (firstStep) {
          DCAS(0, 75, 52, 0, 52, 85, 52, 25, panServo, tiltServo, 95);
          lastmode = 1;
          firstStep = false;
        } else {
          DCAS(0, 50, 52, 0, 30, 45, 52, 25, panServo, tiltServo, 95);
          lastmode = 1;
        }
        break;

      case 1:
        DCAS(0, 20, 66, 0, 39, 15, 31, 25, panServo, tiltServo, 73);
        lastmode = 1;
        break;

      case 2:
        DCAS(0, 60, 80, 0, 52, 60, 10, 25, panServo, tiltServo, 52);
        lastmode = 1;
        break;

      case 3:
        DCAS(0, 75, 80, 0, 52, 85, 10, 25, panServo, tiltServo, 52);
        lastmode = 1;
        break;

      case 4:
        DCAS(0, 75, 80, 30, 52, 85, 10, 50, panServo, tiltServo, 52);
        lastmode = 1;
        break;

      case 5:
        DCAS(0, 75, 66, 65, 39, 85, 31, 85, panServo, tiltServo, 73);
        lastmode = 1;
        break;

      case 6:
        DCAS(0, 75, 52, 20, 30, 85, 52, 45, panServo, tiltServo, 95);
        lastmode = 1;
        break;

      case 7:
        DCAS(0, 75, 52, 0, 30, 85, 52, 25, panServo, tiltServo, 95);
        lastmode = 1;
        break;

      default:
        moveStep = -1;  // Reset step for the next cycle
        lastmode = 0;
        lastMoveTime = 0;
        break;
    }

    moveStep++;
  }
}

void moveBackward() {
  unsigned long currentTime = millis();

  if (currentTime - lastMoveTime >= stepDelay) {
    lastMoveTime = currentTime;

    switch (moveStep) {
      case 0:
        if (firstStep) {
          DCAS(0, 75, 52, 0, 52, 85, 52, 25, panServo, tiltServo, 52);
          lastmode = 2;
          firstStep = false;
        } else {
          DCAS(0, 50, 52, 0, 95, 45, 52, 25, panServo, tiltServo, 30);
          lastmode = 2;
        }
        break;

      case 1:
        DCAS(0, 20, 31, 0, 73, 15, 66, 25, panServo, tiltServo, 39);
        lastmode = 2;
        break;

      case 2:
        DCAS(0, 60, 10, 0, 52, 60, 80, 25, panServo, tiltServo, 52);
        lastmode = 2;
        break;

      case 3:
        DCAS(0, 75, 10, 0, 52, 85, 80, 25, panServo, tiltServo, 52);
        lastmode = 2;
        break;

      case 4:
        DCAS(0, 75, 10, 30, 52, 85, 80, 50, panServo, tiltServo, 52);
        lastmode = 2;
        break;

      case 5:
        DCAS(0, 75, 30, 65, 73, 85, 65, 85, panServo, tiltServo, 39);
        lastmode = 2;
        break;

      case 6:
        DCAS(0, 75, 52, 20, 95, 85, 52, 45, panServo, tiltServo, 30);
        lastmode = 2;
        break;

      case 7:
        DCAS(0, 75, 52, 0, 95, 85, 52, 25, panServo, tiltServo, 30);
        lastmode = 2;
        break;

      default:
        moveStep = -1;
        lastmode = 0;
        lastMoveTime = 0;
        break;
    }
    moveStep++;
  }
}

void turnLeft() {
  unsigned long currentTime = millis();

  if (currentTime - lastMoveTime >= stepDelay) {
    lastMoveTime = currentTime;

    switch (moveStep) {
      case 0:
        DCAS(0, 80, 10, 0, 85, 85, 10, 20, panServo, tiltServo, 90);
        lastmode = 3;
        break;

      case 1:
        DCAS(0, 50, 10, 0, 85, 45, 10, 20, panServo, tiltServo, 90);
        lastmode = 3;
        break;

      case 2:
        DCAS(0, 50, 52, 0, 52, 45, 52, 20, panServo, tiltServo, 52);
        lastmode = 3;
        break;

      case 3:
        DCAS(0, 80, 52, 0, 52, 85, 52, 20, panServo, tiltServo, 52);
        lastmode = 3;
        break;

      case 4:
        DCAS(0, 80, 52, 35, 52, 85, 52, 55, panServo, tiltServo, 52);
        lastmode = 3;
        break;

      case 5:
        DCAS(0, 80, 10, 35, 85, 85, 10, 55, panServo, tiltServo, 90);
        lastmode = 3;
        break;

      case 6:
        DCAS(0, 80, 10, 0, 85, 85, 10, 20, panServo, tiltServo, 90);
        lastmode = 3;
        break;

      default:
        moveStep = -1;
        lastmode = 0;
        lastMoveTime = 0;
        break;
    }
    moveStep++;
  }
}

void turnRight() {
  unsigned long currentTime = millis();

  if (currentTime - lastMoveTime >= stepDelay) {
    lastMoveTime = currentTime;

    switch (moveStep) {
      case 0:
        DCAS(0, 80, 10, 0, 85, 85, 10, 20, panServo, tiltServo, 90);
        lastmode = 4;
        break;

      case 1:
        DCAS(0, 80, 10, 30, 85, 85, 10, 60, panServo, tiltServo, 90);
        lastmode = 4;
        break;

      case 2:
        DCAS(0, 80, 52, 30, 52, 85, 52, 60, panServo, tiltServo, 52);
        lastmode = 4;
        break;

      case 3:
        DCAS(0, 80, 52, 0, 52, 85, 52, 20, panServo, tiltServo, 52);
        lastmode = 4;
        break;

      case 4:
        DCAS(0, 50, 52, 0, 52, 45, 52, 20, panServo, tiltServo, 52);
        lastmode = 4;
        break;

      case 5:
        DCAS(0, 50, 10, 0, 85, 45, 10, 20, panServo, tiltServo, 90);
        lastmode = 4;
        break;

      case 6:
        DCAS(0, 80, 10, 0, 85, 85, 10, 20, panServo, tiltServo, 90);
        lastmode = 4;
        break;

      default:
        moveStep = -1;
        lastmode = 0;
        lastMoveTime = 0;
        break;
    }
    moveStep++;
  }
}

// Move with Speed Control (MWSC)
void MWSC(uint16_t servo, uint16_t degree, uint16_t cur_deg, uint16_t speed) {
  if (speed <= 0) speed = 1;

  int time = round(100.0 / speed);
  int angleIncrement = round((float)(degree - cur_deg) / time);

  for (int i = 0; i < time; ++i) {
    cur_deg += angleIncrement;
    setServoPosition(servo - 1, cur_deg);
    delay(50);
  }
}

// Write Servo Positions (Example)
void writeServoPositions(int pos0, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7) {
  writeRegister(0x00, pos0);
  writeRegister(0x01, pos1);
  writeRegister(0x02, pos2);
  writeRegister(0x03, pos3);
  writeRegister(0x04, pos4);
  writeRegister(0x05, pos5);
  writeRegister(0x06, pos6);
  writeRegister(0x07, pos7);
}

#endif