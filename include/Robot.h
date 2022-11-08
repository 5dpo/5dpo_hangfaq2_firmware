#ifndef ROBOT_H
#define ROBOT_H

#include "robotconfig.h"
#include "Encoder.h"
#include "Motor.h"
#include "CtrlPID.h"

class Robot {
 public:
  int32_t dt;
  Encoder *enc;
  Motor mot[4];
  CtrlPID pid[4];

  void (*serialWriteChannel)(char channel, int32_t value);

 public:
  void init(
      void (*serialWriteChannelFunction)(char c, int32_t v),
      Encoder *new_enc);

  void update(int32_t &delta);
  void send(void);

  void setMotorWref(uint8_t index, float new_w_r);
  void setMotorPWM(uint8_t index, int16_t pwm);
};

#endif