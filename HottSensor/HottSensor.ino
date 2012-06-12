/*
Arduino HottSensor by Christian Völlinger & Frank-Christian Henning
June  2012
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
*/

#include <Wire.h>
#include <stdlib.h>
#include "SoftwareSerial.h"
#include "BMP085.h"
#include "lowpass.h"

#define HOTTRX 2
#define HOTTTX 2

#define CURRENTPIN A0
#define VOLTAGEPIN A1
#define LEDPIN 13

SoftwareSerial hottSerial(HOTTRX, HOTTTX);
BMP085 bmp;

int ledState = LOW; 
static volatile boolean inAscii = false;

static uint32_t currentTime = 0;
static uint32_t currentTime1 = 0;
static uint16_t previousTime = 0;
static uint16_t previousTimeBmp = 0;
static uint16_t cycleTime = 0;

// Sensors    
static float    GroundAltitude;
static float    PreviousAltitude;

static Lowpass AbsoluteAltitude(0.5);

static float      Climbrate1;
static float      Climbrate3;

static Lowpass   Current(0.5);

static float    Temperature;
static int32_t  Voltage;
static float    Capacity;

// Settings
static float VoltageAlarm = 9.9;

void setup() {
  pinMode(LEDPIN, OUTPUT);
   
  // Setup Hardware Serial
  Serial.begin(115200);
  
  // Setup Software Serial for Hott
  pinMode(HOTTRX, INPUT);
  hottSerial.begin(19200);
  
  // start Vario
  bmp.begin(BMP085_HIGHRES);
  readBmp085();
  GroundAltitude = AbsoluteAltitude; // Bodenlevel merken
  
  toggleLed();
  
  Capacity = 0;
}


void loop() {
  currentTime1 = millis();
  int delta = currentTime1 - previousTime;
  if(delta >= 50) { // alle 50ms
    // gather data
    readVoltage();
    readCurrent();
    
    Capacity += (Current * (delta / 360000.0f) ); // umrechnen von "Ampere millisekunde" zu mAh
    
    previousTime = currentTime1;
    
    readBmp085();
  }
  
  int bmpDelta = currentTime1 - previousTimeBmp;
  if(bmpDelta > 500) {
    
    Climbrate1 = (AbsoluteAltitude - PreviousAltitude) * ( bmpDelta / 1000.0f );
    PreviousAltitude = AbsoluteAltitude;
    
    Serial.println(bmpDelta);
    
    previousTimeBmp = currentTime1;
  }
  
  // process hott
  loopHott();
  
}

void toggleLed() {
     if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
      
    digitalWrite(LEDPIN, ledState);
}
