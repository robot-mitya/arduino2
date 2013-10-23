// -------------------------------------------------------------------------------------
// file = "rm_div.h" (Robot Mitya voltage dividers header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   This static class initialize and control Robot Mitya's voltage dividers.
// -------------------------------------------------------------------------------------

#ifndef rm_div_h
#define rm_div_h

#include <Arduino.h>

namespace robot_mitya
{
  class VoltageDivider
  {
    public:
      // Constructor's params:
      //   dividerIndex - 0: battery voltage divider, 1: charger voltage divider
      //   dividerPin - pin on Arduino controller that is used to measure voltage
      //   arefVoltage - voltage in Volts on AREF pin of Arduino controller.
      //   r1 - voltage divider resistor R1
      //   r2 - voltage divider resistor R2 (voltage is measured on this resistor)
      VoltageDivider(int dividerIndex, int dividerPin, float arefVoltage, float r1, float r2);
      
      // Return value is measured voltage * 100
      unsigned int getVoltage();
      
      void setTimer(unsigned long timerDelay, void (*handler)(int, unsigned int) /* param1 - divider index, param2 - voltage */);
      void refresh();
    private:
      int dividerIndex;
      int dividerPin;
      float voltRatio;
      boolean timerEnabled;
      unsigned long timerDelay;
      unsigned long timerNextTime;
      void (*timerHandler)(int, unsigned int); // param1 - divider index, param2 - voltage
  };
}

#endif
