#include <Arduino.h>

#include <TimerOne.h>
#include <Wire.h>

#include <SPI.h>
#include <Adafruit_MotorShield.h>

#include <channels.h>

#include "Robot.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

unsigned long current_micros = 0, previous_micros = 0;
unsigned long last_motor_update_millis = 0;
uint8_t timeout = 0;
channels_t serial_channels;
uint8_t builtin_led_state;

Robot robot;
Encoder encoders[4];



/******************************************************************************
 * FUNCTIONS HEADERS
 ******************************************************************************/
void processSerialPacket(char channel, uint32_t value, channels_t& obj);
void serialWrite(uint8_t b);
void serialWriteChannel(char channel, int32_t value);
void serialRead();
void checkMotorsTimeout();



/******************************************************************************
 * IMPLEMENTATION
 ******************************************************************************/
void setup() {
  uint8_t i;

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

  updateEncodersState();
  for (i = 0; i < 4; i++) {
    encoders[i].delta = 0;
  }

  Timer1.attachInterrupt(updateEncodersState);
  Timer1.initialize(50);  // calls every 50us

  // Serial communication
  Serial.begin(115200);
  serial_channels.init(processSerialPacket, serialWrite);

  // Motors
  for (i = 0; i < 4; i++) {
    robot.mot[i].ptr = AFMS.getMotor(i + 1);
    robot.mot[i].enable = true;
    //robot.mot[i].setPWM(0);
  }

  // Robot
  robot.init(serialWriteChannel, encoders);

  // Reset signal
  serialWriteChannel('r', 0);

  // Initialization
  current_micros = micros();
  previous_micros = current_micros;
  last_motor_update_millis = millis();
}

void loop() {
  static unsigned long blink_led_decimate = 0;
  uint32_t delta;

  serialRead();

  current_micros = micros();
  delta = current_micros - previous_micros;
  if (delta > kMotCtrlTimeUs) {
    if (kMotCtrlTimeoutEnable) {
      checkMotorsTimeout();
    }

    if (!timeout) {
      previous_micros = current_micros;
      
      // Update and send data
      robot.update(delta);
      robot.send();

      // Blink LED
      blink_led_decimate++;
      if (blink_led_decimate >= kMotCtrlLEDOkCount) {
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
      robot.setMotorWref(mot_i, ((int32_t) value) * kEncImp2MotW );
#endif
#ifdef CONFIG_ROS
      robot.setMotorWref(mot_i, *((float*) &value) );
#endif
      break;
  
    // - PWM
    case 'K':
      mot_i = (value >> 24) & 0x03;
      pwm = value & 0xFFFF;
      robot.setMotorPWM(mot_i, pwm);
      break;
  }
}

void serialWrite(uint8_t b) {
  Serial.write(b);
}

void serialWriteChannel(char channel, int32_t value) {
  serial_channels.send(channel, value);
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
