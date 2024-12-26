/*This example is writed by STA.
....visit my github page.(www.github.com/S4NTH174UN9)
....no copy right but give me credit if you want to use
....altual lu9685 can also control by serial but I just use i^2C .
....search LU9685 imformations on baidu.com
....registers 0xfd 0xfd - soft reset
*/

#include <Wire.h>
#include "LU9685.h"
#include <MPU6050.h>

MPU6050 mpu(0x68);
// Constants and Macros
#define CUR_SENSE_PIN A0
#define BAT_SENSE_PIN A1
#define BATTERY_FULL 100
#define BATTERY_LOW 30

// Variables
bool firstStep = true;
int batteryLevel = 0;
double current = 0;
int panServo = 90;
int tiltServo = 40;

// Setup
void setup() {
  Serial.begin(57600);
  initializeLU9685(50,0x80);
  mpu6050setup();
  Serial.println("Send single char command to move the robot");
  setToDefaultPosition();
}

// Main Loop
void loop() {
  handleSerialCommands();
  //updateSensors();
  //mpu6050getData();
}

// Handles Serial Commands
void handleSerialCommands() {
  if (Serial.available()) {
    char command = Serial.read();

    switch (command) {
      case 's': case 'S':
        Serial.println("Receive stand");
        defaultStand();
        break;

      case 'd': case 'D':
        Serial.println("Receive default");
        setToDefaultPosition();
        break;

      case 'w': case 'W':
        Serial.println("Reset Servos");
        resetServos();
        break;

      case 'c': case 'C':
        Serial.println("Receive crab default");
        crabDefault();
        break;

      case 't': case 'T':
        Serial.println("Receive crab stand");
        crabStand();
        break;

      case 'f': case 'F':
        Serial.println("Receive move forward");
        moveForward();
        break;

      case 'b': case 'B':
        Serial.println("Receive move backward");
        moveBackward();
        break;

      case 'j': case 'J':
        Serial.println("Receive jump");
        jump();
        break;

      case 'l': case 'L':
        Serial.println("Turning left");
        turnLeft();
        break;

      case 'r': case 'R':
        Serial.println("Turning right");
        turnRight();
        break;

      case 'a': case 'A':
        Serial.println("BatteryLevel: ");
        Serial.println(batteryLevel);
        break;

      case 'z': case 'Z':
        Serial.println("Current: ");
        Serial.println(current);
        break;

      case 'q': case 'Q':
        Serial.println("Resetting servos.");
        resetServos();
        break;
    }
  }
}

// Sets the robot to its default position
void setToDefaultPosition() {
  DCAS(52, 20, 52, 60, 52, 20, 52, 80, 0, tiltServo, panServo);
  delay(50);
  firstStep = true;
}

// Default stand position
void defaultStand() {
  DCAS(52, 75, 52, 0, 52, 85, 52, 25, 0, tiltServo, panServo);
  delay(50);
  firstStep = true;
}

// Crab default position
void crabDefault() {
  DCAS(90, 20, 10, 60, 85, 20, 10, 80, 0, tiltServo, panServo);
  delay(50);
  firstStep = true;
}

// Crab stand position
void crabStand() {
  DCAS(90, 75, 10, 0, 85, 85, 10, 25, 0, tiltServo, panServo);
  delay(50);
  firstStep = true;
}

// Handles jump behavior
void jump() {
  for (int i = 0; i < 2; ++i) {
    DCAS(52, 20, 52, 60, 52, 20, 52, 80, 0, tiltServo, panServo);
    delay(150);
    DCAS(52, 80, 52, 0, 52, 85, 52, 20, 0, tiltServo, panServo);
    delay(150);
  }
  firstStep = true;
}

// Movement functions
void moveForward() {
  if (firstStep) {
    DCAS(52, 75, 52, 0, 52, 85, 52, 25, 0, tiltServo, panServo);  //go to default stand position
    delay(100);
    //DCAS(52,50,52,0,52,45,52,20);
    DCAS(52, 50, 52, 0, 52, 45, 52, 25, 0, tiltServo, panServo);  //lift the Front Right Leg and Back left Leg to start walking
    delay(100);
  }

  DCAS(95, 50, 52, 0, 30, 45, 52, 25, 0, tiltServo, panServo);  //back the Front Right Leg and Back left Leg a little
  delay(100);
  DCAS(73, 20, 66, 0, 39, 15, 31, 25, 0, tiltServo, panServo);  //up the legs and move the leg to the middle of our front position //Also move the Left Front Leg and Right Back legs to back to move the body from ground to middle
  delay(100);
  DCAS(52, 60, 80, 0, 52, 60, 10, 25, 0, tiltServo, panServo);  //move the legs all the way to the front and down the legs but not touching the ground //Also move the Left Front Leg and Right Back legs to back to move the body from ground to the end
  delay(100);
  DCAS(52, 75, 80, 0, 52, 85, 10, 25, 0, tiltServo, panServo);  //and touch the Front Right Leg and Back left Leg to the grond
  delay(100);
  DCAS(52, 75, 80, 30, 52, 85, 10, 50, 0, tiltServo, panServo);  //and lift up the Left Front Leg and Right Back legs about 25 degree
  delay(100);
  DCAS(73, 75, 66, 65, 39, 85, 31, 85, 0, tiltServo, panServo);  //move the legs to the middle the also lift the legs all the way up //also move the Front Right Leg and Back left Leg to move the body from ground by moving the legs to the middle
  delay(100);
  DCAS(95, 75, 52, 20, 30, 85, 52, 45, 0, tiltServo, panServo);  //move the legs to the end
  delay(100);
  DCAS(95, 75, 52, 0, 30, 85, 52, 25, 0, tiltServo, panServo);  //down the Left Front Leg and Right Back legs to the ground
  delay(100);
  firstStep = false;
}

void moveBackward() {
  if (firstStep) {
    DCAS(52, 75, 52, 0, 52, 85, 52, 25, 0, tiltServo, panServo);  //Go default stand
    delay(100);
    DCAS(52, 50, 52, 0, 52, 45, 52, 25, 0, tiltServo, panServo);  //lift the RFL and LBL //2,6
    delay(100);
  }

  DCAS(30, 50, 52, 0, 95, 45, 52, 25, 0, tiltServo, panServo);  //1,5
  delay(100);
  DCAS(39, 20, 31, 0, 73, 15, 66, 25, 0, tiltServo, panServo);  //1,2,3,5,6,7
  delay(100);
  DCAS(52, 60, 10, 0, 52, 60, 80, 25, 0, tiltServo, panServo);  //move all legs to the end
  delay(100);
  DCAS(52, 75, 10, 0, 52, 85, 80, 25, 0, tiltServo, panServo);  //dowm the legs 1,6
  delay(100);
  DCAS(52, 75, 10, 30, 52, 85, 80, 50, 0, tiltServo, panServo);  //lift the opposite legs for the next move 4,8
  delay(100);
  DCAS(39, 75, 30, 65, 73, 85, 65, 85, 0, tiltServo, panServo);  // all servo to the middle
  delay(100);
  DCAS(30, 75, 52, 20, 95, 85, 52, 45, 0, tiltServo, panServo);  //all servo to the end
  delay(100);
  DCAS(30, 75, 52, 0, 95, 85, 52, 25, 0, tiltServo, panServo);  //Down the leg
  delay(100);
  firstStep = false;
}

void turnLeft() {
  char speed = 100;

  DCAS(90, 80, 10, 0, 85, 85, 10, 20, 0, tiltServo, panServo);  //prepare to step with left foot go crab-stand
  delay(speed);
  //left leg step
  DCAS(90, 50, 10, 0, 85, 45, 10, 20, 0, tiltServo, panServo);  //lift the left for 30degree to start move
  delay(speed);
  DCAS(52, 50, 52, 0, 52, 45, 52, 20, 0, tiltServo, panServo);  //turn the body to default position
  delay(speed);
  DCAS(52, 80, 52, 0, 52, 85, 52, 20, 0, tiltServo, panServo);  //down the legs to stable
  delay(speed);
  //right leg step
  DCAS(52, 80, 52, 35, 52, 85, 52, 55, 0, tiltServo, panServo);  //prepare to step with the right foot lift the right leg
  delay(speed);
  DCAS(90, 80, 10, 35, 85, 85, 10, 55, 0, tiltServo, panServo);  //go crab with left leg lifting to move body to the left
  delay(speed);
  DCAS(90, 80, 10, 0, 85, 85, 10, 20, 0, tiltServo, panServo);  //down the legs
  delay(speed);
}

void turnRight() {
  char speed = 100;

  DCAS(90, 80, 10, 0, 85, 85, 10, 20, 0, tiltServo, panServo);  //prepare to step with left foot go crab-stand
  delay(speed);
  //right leg step
  DCAS(90, 80, 10, 30, 85, 85, 10, 60, 0, tiltServo, panServo);  //lift the right for 30degree to start move
  delay(speed);
  DCAS(52, 80, 52, 30, 52, 85, 52, 60, 0, tiltServo, panServo);  //turn the body to default position
  delay(speed);
  DCAS(52, 80, 52, 0, 52, 85, 52, 20, 0, tiltServo, panServo);  //down the legs to stable
  delay(speed);
  //left leg step
  DCAS(52, 50, 52, 0, 52, 45, 52, 20, 0, tiltServo, panServo);  //prepare to step with the left foot lift the right leg
  delay(speed);
  DCAS(90, 50, 10, 0, 85, 45, 10, 20, 0, tiltServo, panServo);  //go crab with right leg lifting to move body to the left
  delay(speed);
  DCAS(90, 80, 10, 0, 85, 85, 10, 20, 0, tiltServo, panServo);  //down the legs
  delay(speed);
}

// Updates battery and current sensors
void updateSensors() {
  int rawBattery = analogRead(BAT_SENSE_PIN);
  batteryLevel = map(rawBattery, 0, 1023, 0, 100);

  if (batteryLevel == BATTERY_FULL) {
    Serial.println("Charging");
    delay(1000);
  } else if (batteryLevel <= BATTERY_LOW) {
    Serial.println("Low Battery! Please Charge!");
  }

  unsigned int rawCurrent = analogRead(CUR_SENSE_PIN);
  double voltage = (rawCurrent / 1023.0) * 3.3;
  if (voltage < 1.55) voltage = 1.55;

  double measuredCurrent = (voltage - 1.55) / 0.185;

  if (abs(measuredCurrent - current) >= 0.31) {
    Serial.println("Current: ");
    Serial.println(measuredCurrent);
  }

  current = measuredCurrent;
}

void mpu6050setup(){
  // Initialize the MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
  }
}

void mpu6050getData(){
  // Read accelerometer and gyroscope data
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  delay(50);
}
// Move with Speed Control (MWSC)
void MWSC(uint16_t servo, uint16_t degree, uint16_t cur_deg, uint16_t speed) {
  if (speed <= 0) speed = 1;

  int time = round(100.0 / speed);
  int angleIncrement = round((float)(degree - cur_deg) / time);

  for (int i = 0; i < time; ++i) {
    cur_deg += angleIncrement;
    servoWrite(servo - 1, cur_deg);
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
