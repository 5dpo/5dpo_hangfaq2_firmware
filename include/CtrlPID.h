#include <Arduino.h>

class CtrlPID {
 public:
  bool active;
  float kp, ki, kd, kf;
  float w, w_ref;
  float e, e_prev, e_sum;
  float dt;
  float m, m_max;
  float hamm_vd, hamm_v0;

 public:
  void update(float new_w_ref, float new_w);
  void reset(void);

 private:
  void hammerstein(float &mmot);
};
