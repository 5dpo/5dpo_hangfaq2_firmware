#include <Arduino.h>

#include <TimerOne.h>
#include <Wire.h>

#include <SPI.h>
#include <Adafruit_MotorShield.h>

#include <channels.h>

#include "robotconfig.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
unsigned long current_micros = 0, previous_micros = 0;
unsigned long last_motor_update_millis = 0;
uint8_t timeout = 0;
channels_t serial_channels;
uint8_t builtin_led_state;



/******************************************************************************
 * FUNCTIONS HEADERS
 ******************************************************************************/
void processSerialPacket(char channel, uint32_t value, channels_t& obj);
void serialWrite(uint8_t b);
void serialRead();
void checkMotorsTimeout();



/******************************************************************************
 * IMPLEMENTATION
 ******************************************************************************/
void setup() {
  // Built-in LED
  builtin_led_state = LOW;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, builtin_led_state);

  // Motor encoders
  pinMode(kMotEncPin0A, INPUT_PULLUP);
  pinMode(kMotEncPin0B, INPUT_PULLUP);
  pinMode(kMotEncPin1A, INPUT_PULLUP);
  pinMode(kMotEncPin1B, INPUT_PULLUP);
  pinMode(kMotEncPin2A, INPUT_PULLUP);
  pinMode(kMotEncPin2B, INPUT_PULLUP);
  pinMode(kMotEncPin3A, INPUT_PULLUP);
  pinMode(kMotEncPin3B, INPUT_PULLUP);

  // Serial communication
  Serial.begin(115200);
  serial_channels.init(processSerialPacket, serialWrite);

  // Reset signal
  serial_channels.send('r', 0);

  // Initialization
  current_micros = micros();
  previous_micros = current_micros;
  last_motor_update_millis = millis();
}

void loop() {
  static unsigned long blink_led_decimate = 0;

  serialRead();

  current_micros = micros();
  if (current_micros - previous_micros >= kMotCtrlTimeUs) {
    if (kMotCtrlTimeoutEnable) {
      checkMotorsTimeout();
    }

    if (!timeout) {
      previous_micros = micros();
      
      // do things

      // Blink LED
      blink_led_decimate++;
      if (blink_led_decimate > kMotCtrlLEDOkCount) {
        if (builtin_led_state == LOW) {
          builtin_led_state = HIGH;
        } else {
          builtin_led_state = LOW;
        }
        digitalWrite(LED_BUILTIN, builtin_led_state);
        blink_led_decimate = 0;
      }
    }
  }
}



/******************************************************************************
 * FUNCTIONS IMPLEMENTATIONS
 ******************************************************************************/
void processSerialPacket(char channel, uint32_t value, channels_t& obj) {
  uint8_t mot_i;
  int16_t pwm;

  // Reset watchdog
  if ((channel == 'G') || (channel == 'K')) {
    last_motor_update_millis = millis();
  }

  // Process incomming serial packet
  switch (channel) {
    // - reference angular speed
    case 'G':
    case 'H':
    case 'I':
    case 'J':
      mot_i = channel - 'G';
      // set reference angular speed for the motors
#ifdef CONFIG_LAZARUS
#endif
#ifdef CONFIG_ROS
#endif
      break;
  
    // - PWM
    case 'K':
      mot_i = (value >> 24) & 0x03;
      pwm = value & 0xFFFF;
      // set motors pwm value
      break;
  }
}

void serialWrite(uint8_t b) {
  Serial.write(b);
}

void serialRead() {
  uint8_t serial_byte;

  if (Serial.available() > 0) {
    serial_byte = Serial.read();
    serial_channels.StateMachine(serial_byte);
  }
}

void checkMotorsTimeout() {
  if (millis() - last_motor_update_millis > kMotCtrlTimeout) {
    timeout = 1;

    // reset / stop robot

    builtin_led_state = LOW;
    digitalWrite(LED_BUILTIN, builtin_led_state);

  } else {
    timeout = 0;
  }
}