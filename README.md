# SECURITY KAME - 
#### Video Demo  <https://youtu.be/Wg2thCpf57M?si=TqslLem-lVwnqvRm>
#### Description    

**Security Kame** is a versatile and interactive robotic system designed to provide enhanced surveillance and control capabilities. It integrates various hardware components and software functionalities, including an ESP32 camera module, servo motors, MPU6050 accelerometer, and a web-based control panel.

---

## Project Details

- **Project Title**: Security Kame  
- **Author**: San Thit Aung  
- **GitHub**: [S4NTH174UN9](https://github.com/S4NTH174UN9)  
- **edX ID**: `san_429`  
- **City**: Yangon, Myanmar  

---

## Key Features

1. **ESP32 Camera Integration**: Real-time video feed for remote monitoring.
2. **Servo Motor Control**: Directional and action-based movement using servos.
3. **WebSocket Communication**: Efficient real-time communication between client and server.
4. **Web-based Control Panel**: User-friendly interface to control robot movements, light, and camera.
5. **Battery and Current Monitoring**: Display current battery level and power usage.
6. **Interactive Actions**: Predefined actions such as dance, jump, and crab movement.
7. **Tilt and Pan Controls**: Adjustable camera angles for enhanced visibility.
8. **1.3-inch OLED Display**: Provides real-time information and feedback to enhance interaction with the environment.
9. **8002 Amplifier Integration**: Delivers audio feedback or alerts for a more interactive experience

## The Idea  
Initially, I planned to build a website, but I was inspired by the **qwery3 quadrapot** project by [Erik Lely](https://youtu.be/PYqUsu_SGVk?si=kcckK-nIfiR_I-Pc). Erik didn't provide code or 3D files, but he mentioned it was a modified version of [Kame](https://www.thingiverse.com/thing:1265766) on Thingiverse.  

Here’s what I did:  
- **Modified** the 3D designs in Fusion 360.  
- **Created** custom PCBs for the robot.  
- Used **ESP32S** with an **OV2640 camera** for live streaming footage.  
- Added an **OLED display** for the robot's face.  
- Wrote a custom library for LU9685 (a Chinese clone of PCA9685).  
- Borrowed ideas from the ESP32CAM car project by [Hash Include Electronics](https://www.youtube.com/watch?v=HfQ7lhhgDOk&t=630s).  

---

## Electronics  
### Main Components
1. **ESP32S**: The robot's main brain.  
   - Connected to LU9685 servo controller, HS1106 OLED, and MPU6050 via I²C.  
2. **Upper Board**:  
   - Features ESP32S, OV2640 camera, CH340E TTL converter, and AMS1117 3.3V regulator.  
   - USB-C support for reprogramming and power testing.  
   - Includes a 3W speaker for sound (via onboard 8002 amplifier IC).  
3. **Lower Board**:  
   - Features STM32F03 (LU9685 servo controller).  
   - MP1594E (3.3V) and AP62600 (5V 6A) buck converters for power supply.  
   - 2S/3S battery charger (IP2326) with 2000mAh lithium battery support.  
   - Sensors:  
     - **Battery Status** (voltage divider circuit).  
     - **MPU6050 Gyro Sensor**.  
     - **ACS712 Current Sensor**.  

> [!NOTE]  
> Ordered PCBs from [JLCPCB](https://www.jlc.com/).  
> Purchased parts from [TaoBao](https://www.taobao.com/).  

---

## 3D Model  
I remodeled the original Kame design from Thingiverse in Fusion 360 and printed the parts.  

---

## Code  
The project uses **Arduino IDE v2.3.4** for compiling and editing the code, with **VS Code** for HTML editing. The code consists of three main files:  

1. **`LU9685.h`**  
   - Manages I²C communication with the LU9685 servo controller.  
   - It contains 6 functions:  
      - writeRegister
        ```cpp
           bool writeRegister(uint8_t reg, uint8_t value) {
               Wire.beginTransmission(ADDRESS);
               Wire.write(reg);
               Wire.write(value);
               return (Wire.endTransmission() == 0);  // Return success/failure
           }
        ```
        > write register functions get two arguments and send to the controller . First is the function register and second is the value.
        ```cpp
           //Example Usage
           writeRegister(0xFD,0xFD); //Resetallservos
        ```
      - initializeLU9685
        ```cpp
           void initializeLU9685(uint8_t address = DEFAULT_ADDRESS, uint8_t SDA = DEFAULT_SDA, uint8_t SCL = DEFAULT_SCL) {
                if (address > 0x80) {
                    address = DEFAULT_ADDRESS;  // Clamp address to default if out of range
                }
                ADDRESS = address;
            
                Wire.begin(SDA, SCL);
            }
        ```
        > This function is used to initilize the wire communication address with the board and configure the **SDA**,**SCL** pins.
        ```cpp
           //Example Usage
           void setup(){
             //other codes
             initilizeLU9685(0x80,12,13);
           }
        ```
      - setServoPositions
        ```cpp
           void setServoPosition(uint8_t servo, uint8_t degree) {
              if (servo >= 20 || degree > 180) {
                    return;  // Invalid parameters, exit
                }
              writeRegister(servo, degree);
           }
        ```
        >Use to control a single servo with degree
        ```cpp
           //example usage
           serServoPosition(0,90); //90 degree in servo number 0
        ```        
      - **setAllServos**
        ```cpp
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
        ```
        > This function controls all servos using an array of degree values. It ensures that the input length does not exceed the maximum number of servo channels (20).  
        ```cpp
        //Example Usage
        int degrees[] = {90, 45, 180, 0}; // Define angles for each servo
        setAllServos(0x80, degrees, 4); // Set first 4 servos to the specified angles
        ```

      - **DCAS (Direct Control All Servos)**
        ```cpp
        void DCAS(uint8_t d1 = 0, uint8_t d2 = 0, uint8_t d3 = 0, uint8_t d4 = 0, uint8_t d5 = 0,
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
        ```
        > This function allows direct control of all 20 servos using individual degree inputs. Values are clamped to the valid range (0-180).  
        ```cpp
        //Example Usage
        DCAS(90, 45, 135, 180, 90, 45); // Set the first six servos to the specified angles
        ```

      - **resetServos**
        ```cpp
        void resetServos() {
            writeRegister(0xFB, 0xFB);  // Reset command
        }
        ```
        > Resets all servos to their default state. This function is useful for reinitializing or clearing servo states during operation.  
        ```cpp
        //Example Usage
        resetServos(); // Reset all servos
        ```

2. **`kame.h`**  
   - Handles the robot's movements and gait algorithms.
     > There are 11 movements functions right now
     ```cpp
     DefaultPositions();
     DefaultStand();
     crabDefault();
     crabStand();
     jump();
     yes();
     no();
     Dance(); //haven't implemented yet
     moveForwward();
     moveBackward();
     turnLeft();
     turnRight();
     ```
> [!TIP]
> I can add more movements as I wish . I think my code is very flexible and easy to understand but still have many rooms for improvements.  

3. **`main.c`**  
   ### Libraries Used
      ```cpp
         #include "esp_camera.h"
         #include <Arduino.h> //Arduino IDE 2.3.4
         #include <WiFi.h>
         #include <AsyncTCP.h> //V1.14
         #include <ESPAsyncWebServer.h> //V3.4.5
         #include <iostream>
         #include <sstream>
         #include <ESP32Servo.h> //V3.0.5
         #include <Wire.h>
         #include "LU9685.h"
         #include "kame.h"
         #include <U8g2lib.h> //by Oliver V2.35.30
         #include <MPU6050.h> //by Electronic Cats V1.4.1
      ```   
   ### Key Functionalities

      - **Web Server and WebSocket Communication**
         - **Home Page (`handleRoot`)**: Renders the HTML interface for controlling the bot.  
         - **WebSocket Events (`onCarInputWebSocketEvent`)**: Processes real-time commands from the client.  
         - **404 Handler (`handleNotFound`)**: Handles invalid requests.  
         
      - **Movement Control**
         - **Function**: `moveCar(int inputValue)`  
         - Controls movement based on commands (e.g., UP, DOWN, LEFT, RIGHT, RESET).  
         - Uses predefined movement functions like `moveForward()` and `turnLeft()`.  
         
      - Battery and Current Monitoring
         - Continuously updates and displays:  
         - **Battery Level** (`batteryLevel`).  
         - **Current** (`current`).  
         
      - Camera Integration
         - Streams video to the web interface using WebSocket (`wsCamera`).  
         - Captures and displays live footage in the HTML interface.  
         
      - Light, Speed, Pan, and Tilt Controls
         - Interactive sliders in the UI for:  
         - Adjusting light intensity (`sendButtonInput("Light", value)`).  
         - Controlling camera angles (Pan and Tilt).  
         - Setting movement speed.  
      
      ---

   ### Web Interface
      - **Built-in HTML/CSS** for a responsive design.  
      - Features:  
        - Camera feed display.  
        - Directional control buttons.  
        - Action buttons for predefined movements (DANCE, JUMP, etc.).  
        - Slider controls for light, pan, tilt, and speed.  
        ![Screenshot of the web control interface](/assets/images/Control_Panel.png)

      ---
## Usage Instructions

   ### Prerequisites
   1. Install the required libraries via Arduino Library Manager.  
   2. Configure WiFi credentials:  
       ```cpp
         const char* ssid = "Your_SSID";
         const char* password = "Your_Password";
       ```
   ### Setup
   1. Upload the code to the ESP32 board using Arduino IDE.
> [!NOTE]
> connect IO0 with the ground and press the reset button. ESP32 will send "Waiting for download..." to the serial monitor
   2. Connect the hardware components as in the schematics
> [!WARNING]
> Hardwares must be disconnected while the code is uploading
   3. Access the web interface
      - Connect to the bot's WiFi network (`Kame Control`).
      - Open the browser and navigate to <http://192.168.4.1 >.

## Future Improvements  
- replace esp32s with esp32s3 for more performance
- add AI Features or Object dectections.
- Implement animations on the OLED display for a livelier and more engaging robot.  
- Enhance PCB designs for better functionality and durability.  
- Optimize the codebase for performance.  

---

## Credits
- **Libraries**: Contributions from the Arduino, ESPAsyncWebServer, U8g2lib, and MPU6050 communities.
- **Support**: Inspiration from CS50 and online ESP32 development resources.
- **Chat GPT**: Thanks ChatGPT 4.0 for making my code more prettier and efficient.
- **Idea**: Qwery3 and 4 By Erik on Youtube
- **Somecodes**: From Hash Include Electronics. Subcribe Them for more inspiring projects
- **Kame 3D Model**: By BQEducacion on Thingiverse
