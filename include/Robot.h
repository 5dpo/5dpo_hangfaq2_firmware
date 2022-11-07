#ifndef ROBOT_H
#define ROBOT_H

#include "robotconfig.h"
#include "Encoder.h"
#include "Motor.h"

class Robot {
 public:
  Motor mot[4];
};

#endif
