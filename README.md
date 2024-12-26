# SECURITY KAME
#### Video Demo:  <URL HERE>
#### Description:
 Hello!This project is made for CS50 final project.

 #### The idea
    At first I plan to make a website but I see a video of qwery3 qaudrapot by Erik lely on YouTube (https://youtu.be/PYqUsu_SGVk?si=kcckK-nIfiR_I-Pc). But he doesn't provide any code or 3d files. Instead he said it is a modified version of Kame on Thingiverse(https://www.thingiverse.com/thing:1265766). I modified the 3d designs in fusion 360 and also make custom pcb for it.
    Also used ESP32S with OV2640 camera for live steaming the fortage from the robot. Add oled for the robot's face. I wrote a library to control LU9685 which is the chinese clone of PCA9685. And borrowed some Idea from the esp32cam car project by Hash Include Electronics (https://www.youtube.com/watch?v=HfQ7lhhgDOk&t=630s). 

 #### Electronics
    I use ESP32S as the main brain of the robot.To which LU9685 servo controller, HS1106 1.3 inch OLED display and MPU6050 is connected via the same i^2C line. The upper board mainly contains ESP32S, OV2640 camera, CH340E TTL converter ic and also TO252 package of AMS1117 3.3V voltage regulator. With the CH340 on board I can reprogrammed the board so effecitvely and I can also test the board directly on USB-C power. I plan to use the HS1106 1.3 oled as the face of robot and right now I haven't implemented the animations which if I have time, the robot can be more lively and lovely. I also add the 3W speaker on the back of the head which is connected to the onbard 8002 ic which amplified the signal from esp32s'IO25. I started the CS50 class in 2024/Augest and it only have 1 months for final project.So there are more space for improvement in PCBs.
    On the lower board I use the STM32F03 which is the ic come form LU9685 servocontroller. And MP1594E 3.3V buck converter for the digital parts and Ap62600 5V 6A buckconverter for 10 Servos. I also add the 2S/3s battery charger ic IP2326 which can charge the robot in 2hours for the full 2000mah lithium  battery. The lower board also contain 3 sensors.One for battery charging status which is a voltage divider circuit come from battery ,that can also sense the battery level to show on the control webpage. MPU6050 gyro sensor and ACS712 5A current sensor.Each sensor connected to ESP32 IO15 and 16 respectively. 

    Notice: I ordered all the pcbs in JLC.com and buy the parts from taobao.com.

#### 3D model 
    I remodel the Kame for Thingivers in Fusion 360 and printed the parts on [ Thingiverse ](https://www.thingiverse.com/thing:1265766).

#### Code 
    I mainly used Arduino IDE V2.3.4 to compile and edit the code and used vscode to edit and preview the Html code. There are 3 file in my code. "LU9685.h","kame.h" and main.c.
** "LU9685.h" **
    I want to "LU9685.h" first. The header file use <Wire.h> to communicate with the i^2C from controller to esp32cam. It contains 6 functions. 
    Write register()function 
    
