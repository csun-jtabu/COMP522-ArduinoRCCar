/*
Jaztin Tabunda
Arduino RC Car - Semester Project
COMP 522 - V. Bhatt
4/30/2023
*/

#include <Arduino_FreeRTOS.h> //this line of code includes the FreeRTOS Library
#include <semphr.h>
#include <queue.h>

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX
//static const uint8_t bluetooth_TX = 8; // Connects to module's RX
//static const uint8_t bluetooth_RX = 7; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
//SoftwareSerial softwareSerial2(bluetooth_RX, bluetooth_TX);

// Create the Player object
DFRobotDFPlayerMini player;

#define in1 5 //this is for the rear DC motor (LEFT)
#define in2 6 //this is for the rear DC motor (RIGHT)
#define in3 10 //this is for the front DC motor (BACK)
#define in4 11 //this is for the front DC motor (FORWARD)

volatile int state; //current state of the rc car
int leds = 12; //headlight and taillight leds pin
volatile int currentMillis = millis();
volatile int previousMillis = millis();
volatile int previousMillisLights = millis();
//volatile int previousMillisMovement = millis();
//int starvPrev = 1000;
int hornDelay = 500;
int currentHornState = 0;
int currentLightState = 0;
int lightDelay = 500;
int buzzer = 4;
int songVolume = 20; //0-30

TaskHandle_t hornHandler;
TaskHandle_t carHandler;
TaskHandle_t lightHandler;
TaskHandle_t song1Handler;
TaskHandle_t song2Handler;
TaskHandle_t song3Handler;
TaskHandle_t song4Handler;

SemaphoreHandle_t xCountingSemaphore;

void setup() {
  Serial.begin(9600); //Begin communication 9600 
  //softwareSerial2.begin(9600); 
  pinMode(in1, OUTPUT); //OUT1 - LEFT / white
  pinMode(in2, OUTPUT); //OUT2 - RIGHT / Blue
  pinMode(in3, OUTPUT); //OUT3 - BACK / Yellow
  pinMode(in4, OUTPUT); //OUT4 - FORWARD / Green
  pinMode(leds, OUTPUT); //pin that powers tail and headlights
  pinMode(buzzer,OUTPUT); //PIN that powers buzzer sound
  
  softwareSerial.begin(9600);
  ////////////////////////////////////////////////////////
  /*if (player.begin(softwareSerial)) {
      Serial.println("OK");

      // Set volume to maximum (0 to 30).
      player.volume(10);
      // Play the "0001.mp3" in the "mp3" folder on the SD card
      player.playMp3Folder(1);

    } else {
      Serial.println("Connecting to DFPlayer Mini failed!");
  }*/
  /////////////////////////////////////////////////////////
  xTaskCreate(carMovement, "carMovement", 128, NULL,  0, &carHandler);
  xCountingSemaphore = xSemaphoreCreateCounting(4,1);
  xSemaphoreGive(xCountingSemaphore);
  vTaskStartScheduler(); //this is needed to start scheduling tasks whichever has the highest priority is done first 
  
}

void loop() {
}

void forward() {
  analogWrite(in4, 255); //move forward
}
void forwardleft() {
  analogWrite(in4, 255); //move forward
  analogWrite(in1, 50); //turn slightly right
}
void forwardright() {
  analogWrite(in4, 255); //move forward
  analogWrite(in2, 50); //turn slightly left
}
void back() {
  analogWrite(in3, 255); //move backward
}
void backright() {
  analogWrite(in3, 255); //move backward
  analogWrite(in2, 255); //turn full right
}
void backleft() {
  analogWrite(in3, 255); //move backward
  analogWrite(in1, 255); //turn full left
}

void left() {
  analogWrite(in4, 255); //move forward
  analogWrite(in1, 255); //turn full left
}

void right() {
  analogWrite(in4, 255); //move forward
  analogWrite(in2, 255); //turn full right
}

void Stop() {
  analogWrite(in1, 0); //stops all movement in all directions
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void turnOnLights()
{
  digitalWrite(leds, HIGH); //turn on headlights and taillights
}

void turnOffLights()
{
  digitalWrite(leds, LOW); //turn off headlights and taillights
}

void crazyHorn(void *pvParameters)
{
  while(1)
  {
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    //Serial.println("crazyhornTask");
    currentMillis = millis();
    if((currentMillis - previousMillis) >= hornDelay)
    {
      //Serial.println("crazyhornTask");
      //currentHornState =! currentHornState;
      digitalWrite(buzzer, HIGH);
      previousMillis = currentMillis;
    }
    else
    {
      digitalWrite(buzzer, LOW);
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

void crazyLights(void *pvParameters)
{
  while(1)
  {
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    //Serial.println("crazyhornTask");
    currentMillis = millis();
    if((currentMillis - previousMillisLights) >= lightDelay)
    {
      Serial.println("crazyLightTask");
      //currentHornState =! currentHornState;
      digitalWrite(leds, HIGH);
      previousMillisLights = currentMillis;
    }
    else
    {
      digitalWrite(leds, LOW);
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

void carMovement(void *pvParameters)
{
  //checks if data has been received
  while(1)
  {
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("carMovementTask"); //serial
    if (Serial.available() > 0) //serial
    {  
      state = Serial.read(); //takes in the value being received and stores it as a state Serial
      Stop();
      switch (state) {
        case 'F':
          forward();
          
          break;
        case 'G':
          forwardleft();
          
          break;  
        case 'I':
          forwardright();
          
          break;  
        case 'J':
          backright();
          
          break;
        case 'H':
          backleft();
          
          break;
        case 'B':
          back();
          
          break;
        case 'L':
          left();
          
          break;
        case 'R':
          right();
          
          break;
        case 'W':
          turnOnLights();
          
          break;
        case 'w':
          turnOffLights();
          
          break;
        case 'V':
          xTaskCreate(crazyHorn, "horn", 128, NULL,  0, &hornHandler); //613 / 760
          vTaskDelay(1);
          break;
        case 'v':
          vTaskDelete(hornHandler);
          vTaskDelay(1);
          break;
        case 'X':
          xTaskCreate(crazyLights, "lights", 128, NULL,  0, &lightHandler);
          vTaskDelay(1);
          break;
        case 'x':
          vTaskDelete(lightHandler);
          vTaskDelay(1);
          break;
        case '2':
          xTaskCreate(carSong1, "carSong1", 128, NULL,  0, &song1Handler);
          vTaskDelay(1);
          break;
        case '4':
          xTaskCreate(carSong2, "carSong2", 128, NULL,  0, &song2Handler);
          vTaskDelay(1);
          break;
        case '6':
          xTaskCreate(carSong3, "carSong3", 128, NULL,  0, &song3Handler);
          vTaskDelay(1);
          break;
        case '8':
          xTaskCreate(carSong4, "carSong4", 128, NULL,  0, &song4Handler);
          vTaskDelay(1);
          break;           
     } 
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

void carSong1(void *pvParameters)
{
    // Start communication with DFPlayer Mini
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("songtask1");
    if (player.begin(softwareSerial)) {
      Serial.println("OK");
      // Set volume to maximum (0 to 30).
      player.volume(songVolume);
      // Play the "0001.mp3" in the "mp3" folder on the SD card
      player.playMp3Folder(1);
    } 
    else {
      Serial.println("Connecting to DFPlayer Mini failed!");
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
    vTaskDelete(NULL);
}

void carSong2(void *pvParameters)
{
    // Start communication with DFPlayer Mini
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("songtask2");
    if (player.begin(softwareSerial)) {
      Serial.println("OK");
      // Set volume to maximum (0 to 30).
      player.volume(songVolume);
      // Play the "0001.mp3" in the "mp3" folder on the SD card
      player.playMp3Folder(2);
    } 
    else {
      Serial.println("Connecting to DFPlayer Mini failed!");
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
    vTaskDelete(NULL);
}

void carSong3(void *pvParameters)
{
    // Start communication with DFPlayer Mini
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("songtask3");
    if (player.begin(softwareSerial)) {
      Serial.println("OK");
      // Set volume to maximum (0 to 30).
      player.volume(songVolume);
      // Play the "0001.mp3" in the "mp3" folder on the SD card
      player.playMp3Folder(3);
    } 
    else {
      Serial.println("Connecting to DFPlayer Mini failed!");
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
    vTaskDelete(NULL);
}

void carSong4(void *pvParameters)
{
    // Start communication with DFPlayer Mini
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("songtask4");
    if (player.begin(softwareSerial)) {
      Serial.println("OK");
      // Set volume to maximum (0 to 30).
      player.volume(songVolume);
      // Play the "0001.mp3" in the "mp3" folder on the SD card
      player.playMp3Folder(4);
    } 
    else {
      Serial.println("Connecting to DFPlayer Mini failed!");
    }
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
    vTaskDelete(NULL);
}
