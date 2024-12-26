# SECURITY KAME
#### Video Demo  <https://youtu.be/Wg2thCpf57M?si=TqslLem-lVwnqvRm>

---

#### Description  
This project is my CS50 final project.  

### The Idea  
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

### Assembly Highlights  
- Ordered PCBs from [JLCPCB](https://www.jlc.com/).  
- Purchased parts from [TaoBao](https://www.taobao.com/).  

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
        - **Example usage**
        ```cpp
        writeRegister(0xFD,0xFD); //Resetallservos
        ```

2. **`kame.h`**  
   - Handles the robot's movements and gait algorithms.  

3. **`main.c`**  
   - Integrates the project’s core functionalities.  

---

## Future Improvements  
- Implement animations on the OLED display for a livelier and more engaging robot.  
- Enhance PCB designs for better functionality and durability.  
- Optimize the codebase for performance.  

---
