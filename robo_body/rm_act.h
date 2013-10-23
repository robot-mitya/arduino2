// -------------------------------------------------------------------------------------
// file = "rm_act.h" (Robot Mitya's command handler)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   Every executed command pass through Action::execute(...).
//   This class is excluded from Message class in order to make
//   unit-tests for class Message free from command excution logic.
// -------------------------------------------------------------------------------------

#ifndef rm_act_h
#define rm_act_h

#include <Arduino.h>

namespace robot_mitya {
  class Action {
    public:
      static void execute(String command, int value);
    private:
      static boolean isMotorCommand(String command);
      static boolean isMoveHeadCommand(String command);
      static boolean isRotateHeadCommand(String command);
      static boolean isSwingHeadCommand(String command);
      static boolean isMoveTailCommand(String command);
      static boolean isSwingTailCommand(String command);
      static void voltageTimerHandler(int voltageDivider, unsigned int voltage);
  };
}

#endif
