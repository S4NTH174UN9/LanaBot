/*this is the code of security kame for CS50 final project
//Project_Title : Security Kame
//Name : San Thit Aung
//GitHub : S4NTH174UN9
//edX : san_429
//City : Yangon, Myanmar
*/

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
#define LIGHT_PIN 4

MPU6050 mpu(0x68);
// Constants and Macros
#define CUR_SENSE_PIN 14
#define BAT_SENSE_PIN 15
#define BATTERY_FULL 100
#define BATTERY_LOW 30

// Variables
int batteryLevel = 0;
double current = 0;;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define RESET 5
#define STOP 0

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int PWMLightChannel = 4;

//Camera related constants
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid     = "Kame Control";
const char* password = "12345678";

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);

AsyncWebServer server(80);
AsyncWebSocket wsCamera("/Camera");
AsyncWebSocket wsCarInput("/CarInput");

uint32_t cameraClientId = 0;

const char* htmlHomePage PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <title>Control Panel</title>
    <style>
      /* CSS Reset */
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }

      body {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        margin: 0;
        padding: 10px;
        background-color: #f9f9f9;
        color: #333;
      }

      table {
        margin: auto;
        border-spacing: 15px;
        width: 90%;
        max-width: 400px;
      }

      .batteryLevel {
        font-size: small;
        color: #5a30b5;
      }

      .currentLevel {
        font-size: small;
        color: #e64a19;
      }

      img {
        display: block;
        margin: 0 auto;
        border: 2px solid #ddd;
        border-radius: 15px;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }

      td.button {
        width: 70px;
        height: 70px;
        background-color: #5a30b5;
        color: white;
        font-size: 40px;
        font-weight: bold;
        border: 2px solid white;
        border-radius: 10px;
        text-align: center;
        line-height: 70px;
        cursor: pointer;
        transition: transform 0.2s, box-shadow 0.2s;
      }

      td.button:active {
        transform: scale(0.95);
        box-shadow: none;
      }

      td.reset-button {
        background-color: #ff5722;
        color: white;
        border-radius: 10px;
        width: 70px;
        height: 70px;
        text-align: center;
        line-height: 70px;
        font-size: 18px;
        cursor: pointer;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);
        transition: transform 0.1s, background-color 0.2s;
      }

      td.reset-button:hover {
        background-color: #e64a19;
      }

      td.reset-button:active {
        transform: scale(0.95);
        box-shadow: none;
      }

      .action-button {
        background-color: #0c97ce;
        color: white;
        border: none;
        border-radius: 8px;
        padding: 10px 20px;
        font-size: 16px;
        cursor: pointer;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);
        transition: transform 0.1s, box-shadow 0.1s, background-color 0.2s;
      }

      .action-button:hover {
        background-color: #007acc;
      }

      .action-button:active {
        transform: scale(0.95);
        box-shadow: none;
      }

      .slidecontainer {
        margin: 10px 0;
        width: 100%;
      }

      .slider {
        width: 100%;
        -webkit-appearance: none;
        appearance: none;
        height: 15px;
        border-radius: 5px;
        background: #ddd;
        outline: none;
        opacity: 0.9;
        transition: background 0.3s;
      }

      .slider:hover {
        background: #ccc;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        width: 30px;
        height: 30px;
        border-radius: 50%;
        background: #ff5722;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 30px;
        height: 30px;
        border-radius: 50%;
        background: #ff5722;
        cursor: pointer;
      }

      b {
        font-size: 1rem;
        color: #555;
      }

      @media (max-width: 600px) {
        td.button {
          width: 50px;
          height: 50px;
          line-height: 50px;
          font-size: 16px;
        }

        img {
          width: 100%;
          height: auto;
        }
      }
    </style>
  </head>
  <body>
    <table>
      <tr>
        <td colspan="1">
          <b class="batteryLevel">Battery:</b> <span id="batteryLevel">0</span>%
        </td>
        <td colspan="1">
          <b class="currentLevel">Current:</b> <span id="currentAmp">0.00</span>A
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <img id="cameraImage" src="" alt="Camera Feed" style="width:100%; height:250px;">
        </td>
      </tr>
      <tr>
        <td></td>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","1")' ontouchend='sendButtonInput("MoveCar","0")'>&#8679;</td>
        <td></td>
      </tr>
      <tr>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","3")' ontouchend='sendButtonInput("MoveCar","0")'>&#8678;</td>
        <td class="reset-button" ontouchstart='sendButtonInput("MoveCar","5")' ontouchend='sendButtonInput("MoveCar","0")'>RESET</td>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","4")' ontouchend='sendButtonInput("MoveCar","0")'>&#8680;</td>
      </tr>
      <tr>
        <td></td>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","2")' ontouchend='sendButtonInput("MoveCar","0")'>&#8681;</td>
        <td></td>
      </tr>
      <tr>
        <td colspan="3">
          <div style="display: flex; justify-content: space-between;">
            <button class="action-button" onclick="sendButtonInput('Action', '0')">DOWN</button>
            <button class="action-button" onclick="sendButtonInput('Action', '1')">STAND</button>
            <button class="action-button" onclick="sendButtonInput('Action', '2')">YES</button>
            <button class="action-button" onclick="sendButtonInput('Action', '3')">NO</button>
          </div>
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <div style="display: flex; justify-content: space-between;">
            <button class="action-button" onclick="sendButtonInput('Action', '4')">JUMP</button>
            <button class="action-button" onclick="sendButtonInput('Action', '5')">CRAB</button>
            <button class="action-button" onclick="sendButtonInput('Action', '6')">STAND</button>
            <button class="action-button" onclick="sendButtonInput('Action', '7')">DANCE</button>
          </div>
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <b>Pan:</b>
          <div class="slidecontainer">
            <input type="range" min="0" max="180" value="100" class="slider" id="Pan" oninput='sendButtonInput("Pan", value)'>
          </div>
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <b>Tilt:</b>
          <div class="slidecontainer">
            <input type="range" min="0" max="180" value="60" class="slider" id="Tilt" oninput='sendButtonInput("Tilt", value)'>
          </div>
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <b>Light:</b>
          <div class="slidecontainer">
            <input type="range" min="0" max="255" value="0" class="slider" id="Light" oninput='sendButtonInput("Light", value)'>
          </div>
        </td>
      </tr>
      <tr>
        <td colspan="3">
          <b>Speed:</b>
          <div class="slidecontainer">
            <input type="range" min="0" max="100" value="90" class="slider" id="Speed" oninput='sendButtonInput("Speed", value)'>
          </div>
        </td>
      </tr>
    </table>
    <script>
      var webSocketCameraUrl = "ws:\/\/" + window.location.hostname + "/Camera";
      var webSocketCarInputUrl = "ws:\/\/" + window.location.hostname + "/CarInput";      
      var websocketCamera;
      var websocketCarInput;
      
      function initCameraWebSocket() 
      {
        websocketCamera = new WebSocket(webSocketCameraUrl);
        websocketCamera.binaryType = 'blob';
        websocketCamera.onopen    = function(event){};
        websocketCamera.onclose   = function(event){setTimeout(initCameraWebSocket, 2000);};
        websocketCamera.onmessage = function(event)
        {
          var imageId = document.getElementById("cameraImage");
          imageId.src = URL.createObjectURL(event.data);
        };
      }
      
      function initCarInputWebSocket() 
      {
        websocketCarInput = new WebSocket(webSocketCarInputUrl);
        websocketCarInput.onopen    = function(event)
        {
          sendButtonInput("Light", document.getElementById("Light").value);
          sendButtonInput("Pan", document.getElementById("Pan").value);
          sendButtonInput("Tilt", document.getElementById("Tilt").value);
          sendButtonInput("Speed", document.getElementById("Speed").value);                    
        };
        websocketCarInput.onclose   = function(event){setTimeout(initCarInputWebSocket, 2000);};
        websocketCarInput.onmessage = function(event){};        
      }
      
      function initWebSocket() 
      {
        initCameraWebSocket ();
        initCarInputWebSocket();
      }

      function sendButtonInput(key, value) 
      {
        var data = key + "," + value;
        websocketCarInput.send(data);
      }
    
      window.onload = initWebSocket;
      document.getElementById("mainTable").addEventListener("touchend", function(event){
        event.preventDefault()
      }); 

      websocketCarInput.onmessage = function(event) {
        const data = event.data;
        const params = data.split(',');
        params.forEach(param => {
          const [key, value] = param.split(':');
          if (key === 'battery') {
            document.getElementById('batteryLevel').innerText = value + '%';
          } else if (key === 'current') {
            document.getElementById('currentAmp').innerText = parseFloat(value).toFixed(2) + 'A';
          }
        });
      };
    </script>
  </body>
</html>
)HTMLHOMEPAGE";

void moveCar(int inputValue)
{
  Serial.printf("Got value as %d\n", inputValue);  
  switch(inputValue)
  {

    case UP:
      moveForward();                  
      break;
  
    case DOWN:
      moveBackward();  
      break;
  
    case LEFT:
      turnLeft();  
      break;
  
    case RIGHT:
      turnRight();
      break;
 
    case RESET:
      resetServos(); 
      break;
      
    case STOP:
      resetServos();
      break;
  }
}

void handleRoot(AsyncWebServerRequest *request) 
{
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "File Not Found");
}

void onCarInputWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      DefaultStand();
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      moveCar(0);
      ledcWrite(PWMLightChannel, 0); 
      setServoPosition(8, 90); 
      setServoPosition(9, 60);
      resetServos();      
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        std::istringstream ss(myData);
        std::string key, value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        Serial.printf("Key [%s] Value[%s]\n", key.c_str(), value.c_str()); 
        int valueInt = atoi(value.c_str());     
        if (key == "MoveCar")
        {
          moveCar(valueInt);        
        }
        if (key == "Action")
        {
          switch(valueInt){
            case 0: DefaultPosition(); break;
            case 1: DefaultStand(); break;
            case 2: yes(); break;
            case 3: no(); break;
            case 4: jump(); break;
            case 5: crabDefault(); break;
            case 6: crabStand(); break;
            case 7: Dance(); break;
            default: resetServos(); break;
          }
        }
        else if (key == "Light")
        {
          ledcWrite(PWMLightChannel, valueInt);         
        }
        else if (key == "Pan")
        {
          setServoPosition(8, valueInt);
          panServo = valueInt;
        }
        else if (key == "Tilt")
        {
          setServoPosition(9, valueInt);  
          tiltServo = valueInt; 
        }
        else if (key == "Speed");
          stepDelay = map(valueInt,0,100,150,50);             
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}

void onCameraWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      cameraClientId = client->id();
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      cameraClientId = 0;
      break;
    case WS_EVT_DATA:
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}

void setupCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_4;
  config.ledc_timer = LEDC_TIMER_2;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }  

  sensor_t *s = esp_camera_sensor_get();
    if (s != NULL) {
        s->set_vflip(s, 1); // Enable vertical flip
    } else {
        Serial.println("Failed to get camera sensor");
    }
  
  if (psramFound())
  {
    heap_caps_malloc_extmem_enable(20000);  
    Serial.printf("PSRAM initialized. malloc to take memory from psram above this size");    
  }  
}

void sendCameraPicture()
{
  if (cameraClientId == 0)
  {
    return;
  }
  unsigned long  startTime1 = millis();
  //capture a frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) 
  {
      Serial.println("Frame buffer could not be acquired");
      return;
  }

  unsigned long  startTime2 = millis();
  wsCamera.binary(cameraClientId, fb->buf, fb->len);
  esp_camera_fb_return(fb);
    
  //Wait for message to be delivered
  while (true)
  {
    AsyncWebSocketClient * clientPointer = wsCamera.client(cameraClientId);
    if (!clientPointer || !(clientPointer->queueIsFull()))
    {
      break;
    }
    delay(1);
  }
  
  unsigned long  startTime3 = millis();  
  Serial.printf("Time taken Total: %d|%d|%d\n",startTime3 - startTime1, startTime2 - startTime1, startTime3-startTime2 );
}

void setUpPinModes()
{
  pinMode(CUR_SENSE_PIN, INPUT);
  pinMode(BAT_SENSE_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);    
  ledcAttach(LIGHT_PIN, PWMFreq, PWMResolution);
}

// Updates battery and current sensors
void updateSensors() {
  int rawBattery = analogRead(BAT_SENSE_PIN);
  batteryLevel = map(rawBattery, 0, 4095, 0, 100); // Scale to 0-100%
  
  int rawCurrent = analogRead(CUR_SENSE_PIN);
  double voltage = (rawCurrent / 4095.0) * 3.3;
  if (voltage < 1.55) voltage = 1.55;
  double current = (voltage - 1.55) / 0.185;

  String sensorData = String("battery:") + batteryLevel + ",current:" + current;
  wsCarInput.textAll(sensorData); //chatGPT
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

void setup(void) 
{
  setUpPinModes();
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
      
  wsCamera.onEvent(onCameraWebSocketEvent);
  server.addHandler(&wsCamera);

  wsCarInput.onEvent(onCarInputWebSocketEvent);
  server.addHandler(&wsCarInput);

  server.begin();
  Serial.println("HTTP server started");

  setupCamera();
  
  initializeLU9685(0x80, 12, 13); //i^C address AND i^C Pin numbers
  mpu6050setup();

 xTaskCreate([](void *) {
    for (;;) {
      updateSensors();
      vTaskDelay(1000 / portTICK_PERIOD_MS); // Update every 1 second
    }
  }, "SensorUpdateTask", 2048, NULL, 1, NULL);

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

void show_oled(){
  u8g2.clearBuffer();                 // Clear the internal buffer
  u8g2.drawStr(0, 10, "Hello! I am Kame"); // Draw text  
  u8g2.drawCircle(64, 32, 10);        // Draw a circle
  u8g2.sendBuffer();
  delay(1);
}
void loop() 
{
  wsCamera.cleanupClients(); 
  wsCarInput.cleanupClients(); 
  sendCameraPicture(); 
  if(lastmode != 0){
    switch(lastmode) {
      case 1: moveForward(); break;
      case 2: moveBackward(); break;
      case 3: turnLeft(); break;
      case 4: turnRight(); break;
    }
  }
  show_oled();
}
