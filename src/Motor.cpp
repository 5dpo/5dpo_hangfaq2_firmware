#include "Motor.h"

void Motor::setPWM(int16_t new_pwm) {
  // Saturation
  if (new_pwm > kMotPWMmax) {
    new_pwm = kMotPWMmax;
  } else if (new_pwm < -kMotPWMmax) {
    new_pwm = -kMotPWMmax;
  }

  // Set pwm
  if (enable) {
    if (new_pwm >= 0) {
      ptr->setSpeed((uint8_t) new_pwm);
      ptr->run(FORWARD);

    } else {
      ptr->setSpeed((uint8_t) abs(new_pwm));
      ptr->run(BACKWARD);
    }

  } else {
    ptr->run(RELEASE);
  }
}
