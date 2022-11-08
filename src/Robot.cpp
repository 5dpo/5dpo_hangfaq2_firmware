#include "Robot.h"

void Robot::init(
    void (*serialWriteChannelFunction)(char c, int32_t v),
    Encoder *new_enc) {
  uint8_t i;

  // Serial write channels function
  serialWriteChannel = serialWriteChannelFunction;

  // Encoders
  enc = new_enc;

  // Controllers
  for (i = 0; i < 4; i++) {
    pid[i].active = false;
    pid[i].kp = kMotCtrlKc;
    if (kMotCtrlTi == 0) {
      pid[i].ki = 0;
    } else {
      pid[i].ki = kMotCtrlKc / kMotCtrlTi;
    }
    pid[i].kd = 0;
    pid[i].kf = 0;
    pid[i].dt = kMotCtrlTime;

    pid[i].m_max = kMotVmax;

    pid[i].hamm_vd = 0;
    pid[i].hamm_v0 = 0;

    pid[i].reset();
  }
}

void Robot::update(uint32_t &delta) {
  uint8_t i;
  dt = delta;

  // Encoders
  for (i = 0; i < 4; i++) {
    enc[i].updateTick();
  }

  // Controllers
  for (i = 0; i < 4; i++) {
    pid[i].update(enc[i].odo * kEncImp2MotW);
  }

  // Actuators
  for (i = 0; i < 4; i++) {
    if (pid[i].active) {
      mot[i].setPWM( round( kMotV2MotPWM * pid[i].m ) );
    }
  }
}

void Robot::send(void) {
  (*serialWriteChannel)('g', enc[0].odo);
  (*serialWriteChannel)('h', enc[1].odo);
  (*serialWriteChannel)('i', enc[2].odo);
  (*serialWriteChannel)('j', enc[3].odo);

  (*serialWriteChannel)('k', dt);
}

void Robot::stop(void) {
  uint8_t i;

  for (i = 0; i < 4; i++) {
    setMotorWref(i, 0);
  }
}

void Robot::setMotorWref(uint8_t index, float new_w_r) {
  pid[index].enable(true);
  pid[index].w_ref = new_w_r;
}

void Robot::setMotorPWM(uint8_t index, int16_t pwm) {
  pid[index].enable(false);
  mot[index].setPWM(pwm);
}
