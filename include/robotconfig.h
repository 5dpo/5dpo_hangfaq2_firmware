#ifndef ROBOTCONFIG_H
#define ROBOTCONFIG_H

#include <Arduino.h>

/******************************************************************************
 * Configuration (uncomment the appropriate configuration)
 * - Lazarus-based applications (original channels implementation)
 * - ROS-based navigation stack (modified implementation of channels with the
 *     using the ROS package serial_communication_channels
 ******************************************************************************/
// #define CONFIG_LAZARUS
#define CONFIG_ROS

/******************************************************************************
 * Robot parameters
 * - kinematic configuration
 * - battery
 ******************************************************************************/
const float kRobotL[] = {
  0.268,
  0.268
};
const float kRobotWhD[] = {
  0.1016,
  0.1016,
  0.1016,
  0.1016
};
const float kRobotBattVnom = 12.0;

/******************************************************************************
 * Motor parameters
 * - gear reduction ratio and encoders resolution
 * - controllers parameters
 ******************************************************************************/
const float kMotNgear = 64;
const float kMotEncRes = 12;

const int kMotEncPin0A = 22;
const int kMotEncPin0B = 23;
const int kMotEncPin1A = 24;
const int kMotEncPin1B = 25;
const int kMotEncPin2A = 26;
const int kMotEncPin2B = 27;
const int kMotEncPin3A = 28;
const int kMotEncPin3B = 29;

const float kMotModelKp = 1;    // ?
const float kMotModelTau = 0.1; // ?
const float kMotModelLag = 0.0; // ?

const unsigned long kMotCtrlTime = 10000UL; // (us)

const float kMotVmax = 12;
const int kMotPWMmax = 255;

const float kMotHammV0 = 0;
const float kMotHammVd = 0;

const float kMotCtrlPITauCl = kMotModelTau / 1.5;
const float kMotCtrlPIKcKp = kMotModelTau / (kMotCtrlPITauCl + kMotModelLag);
const float kMotCtrlPIKc = kMotCtrlPIKcKp / kMotModelKp;
const float kMotCtrlPITi = kMotModelTau;

/******************************************************************************
 * Conversion constants
 ******************************************************************************/
const float kEncImp2MotW =
    2 * PI * 1000000 / (1.0 * kMotCtrlTime * kMotNgear * kMotEncRes);
const float kMotV2MotPWM = kMotPWMmax * 1.0 / kRobotBattVnom;

#endif
