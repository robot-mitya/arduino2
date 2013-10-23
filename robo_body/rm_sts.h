// -------------------------------------------------------------------------------------
// file = "rm_sts.h" (Robot Mitya's current state)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
// -------------------------------------------------------------------------------------

#ifndef rm_sts_h
#define rm_sts_h

#include <Arduino.h>

#include "rm_msg.h"

namespace robot_mitya
{
  enum ButtonsControlMode { DISABLED, HEAD_CONTROL, MOTORS_CONTROL, OTHER_CONTROL };
  
  class State
  {
    public:
      static void initialize();
      static void setNextButtonsControlMode();
      static ButtonsControlMode getButtonsControlMode();
    private:
      static void logButtonsMode();
  };
}

#endif
