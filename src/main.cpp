#include <Arduino.h>

#include <TimerOne.h>
#include <Wire.h>

#include <SPI.h>
#include <Adafruit_MotorShield.h>

#include "robotconfig.h"

void setup() {
  // Motor encoders
  pinMode(kMotEncPin0A, INPUT_PULLUP);
  pinMode(kMotEncPin0B, INPUT_PULLUP);
  pinMode(kMotEncPin1A, INPUT_PULLUP);
  pinMode(kMotEncPin1B, INPUT_PULLUP);
  pinMode(kMotEncPin2A, INPUT_PULLUP);
  pinMode(kMotEncPin2B, INPUT_PULLUP);
  pinMode(kMotEncPin3A, INPUT_PULLUP);
  pinMode(kMotEncPin3B, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
}
