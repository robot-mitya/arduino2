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
  class Equipment
  {
    public:
      static void initialize();
      static void refresh();      
      
      static void setHeadlightState(int value);
      
      static void moveHead(String plane, int degree);
      static void rotateHead(String plane, signed int period);
      static void swingHead(String plane, int mode);

      static void moveTail(int degree);
      static void swingTail(int mode);

      static void moveMotor(String side, int speed);
      
      static unsigned int getVoltageDividerPinValue(int dividerPin);
      static unsigned int getVoltage(int dividerIndex);
      static void setVoltageTimer(int dividerIndex, unsigned long timerDelay, void (*handler)(int, unsigned int));
    private:
      static void buttonsHandler(ButtonState buttonState, Button button);
  };
}

#endif
