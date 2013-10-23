// -------------------------------------------------------------------------------------
// file = "rm_cfg.h" (Robot Mitya's configuration constants)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
// -------------------------------------------------------------------------------------

#ifndef rm_cfg_h
#define rm_cfg_h

#include <Arduino.h>

namespace robot_mitya
{
  class Cfg
  {
    public:
      // Baud rate of the controller-PC serial link.
      static const int SERIAL_BAUD_RATE = 9600;
      // Length of RoboScript messages.
      static const int MESSAGELENGTH = 5;
      // Echo-mode - all commands sent to robot will be returned back.
      // This is a debug option.
      static const boolean ECHO_MODE = false;
      
      // Pins to control DC drive motors.
      static const int MOTOR_LEFT_SPEED_PIN = 5;
      static const int MOTOR_LEFT_DIRECTION_PIN = 4;
      static const int MOTOR_RIGHT_SPEED_PIN = 6;
      static const int MOTOR_RIGHT_DIRECTION_PIN = 7;
      
      // Pins to control servos.
      static const int SERVO_HEAD_HORIZONTAL_PIN = 9;
      static const int SERVO_HEAD_VERTICAL_PIN = 10;
      static const int SERVO_TAIL_PIN = 11;
      
      // Headlights pin.
      static const int LIGHT_PIN = 13;
      
      // Romeo buttons
      static const boolean USE_BUTTONS = true;
      // Romeo V2.0 buttons.
      static const int BUTTONS_PIN = A0;
      // Romeo V1.1 buttons.
//      static const int BUTTONS_PIN = A7;
      
      // Boundary values for servos.
      static const int SERVO_HEAD_HORIZONTAL_MIN_DEGREE = 0;
      static const int SERVO_HEAD_HORIZONTAL_MAX_DEGREE = 180;
      static const int SERVO_HEAD_VERTICAL_MIN_DEGREE = 0;
      static const int SERVO_HEAD_VERTICAL_MAX_DEGREE = 90;
      static const int SERVO_TAIL_MIN_DEGREE = 10; // Not ZERO, because in the boundary position servo is vibrating a lot
      static const int SERVO_TAIL_MAX_DEGREE = 170; // Not 180, because in the boundary position servo is vibrating a lot
      
      // Default values for servos.
      static const int SERVO_HEAD_HORIZONTAL_DEFAULT_STATE = 90;
      static const int SERVO_HEAD_VERTICAL_DEFAULT_STATE = 90;
      static const int SERVO_TAIL_DEFAULT_STATE = 90;
      
      // Voltage dividers.
      static const float AREF_VOLTAGE = 4.95;

      static const int VOLTAGE_BATTERY_DIVIDER_INDEX = 0;
      static const int VOLTAGE_BATTERY_PIN = A5;
      static const float VOLTAGE_BATTERY_R1 = 7.5; // It doesn't matter Om or kOm.
      static const float VOLTAGE_BATTERY_R2 = 4.7;

      static const int VOLTAGE_CHARGER_DIVIDER_INDEX = 1;
      static const int VOLTAGE_CHARGER_PIN = A4;
      static const float VOLTAGE_CHARGER_R1 = 7.5;
      static const float VOLTAGE_CHARGER_R2 = 4.7;
      
  };
}

#endif
