// -------------------------------------------------------------------------------------
// file = "rm_equ.h" (Robot Mitya equipment header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   This static class initialize and control Robot Mitya's equipment:
//   DC motors, head and tail servos, headlights etc.
// -------------------------------------------------------------------------------------

#ifndef rm_equ_h
#define rm_equ_h

#include <Arduino.h>
#include "rm_btn.h"

namespace robot_mitya
{
  enum HeadPlane
  {
    HORIZONTAL = 0,
    VERTICAL = 1
  };

  class Equipment
  {
    public:
      static void initialize();
      static void refresh();      
      
      static void executeInstruction(int value);
      
      static void moveHead(HeadPlane plane, int degree);
      static void rotateHead(HeadPlane plane, signed int period);
      static void swingHead(HeadPlane plane, int mode);
      static void swingHeadEx(HeadPlane plane, int mode, signed long period, double iterations,
          signed long amplitude, double amplitudeCoefficient, bool positiveDirection);
      static int getHeadPosition(HeadPlane plane);

      static void moveTail(int degree);
      static void swingTail(int mode);
      static void swingTailEx(int mode, signed long period, double iterations,
          signed long amplitude, double amplitudeCoefficient, bool positiveDirection);
      static int getTailPosition();

      static void moveMotor(String side, int speed);
      
      static unsigned int getVoltageDividerPinValue(int dividerPin);
      static unsigned int getVoltage(int dividerIndex);
      static void setVoltageTimer(int dividerIndex, unsigned long timerDelay, void (*handler)(int, unsigned int));
    private:
      static void buttonsHandler(ButtonState buttonState, Button button);
      static void chargerHandler(int voltageDivider, unsigned int voltage);
  };
}

#endif
