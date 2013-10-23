#include "rm_sts.h"

#include <Arduino.h>

using namespace robot_mitya;

static ButtonsControlMode buttonsControlMode;

void State::initialize()
{
  buttonsControlMode = DISABLED;
}

void State::setNextButtonsControlMode()
{
  int mode = buttonsControlMode + 1;
  if (mode > (int)OTHER_CONTROL) mode = (int)DISABLED;
  buttonsControlMode = (ButtonsControlMode)mode;
  logButtonsMode();
}

ButtonsControlMode State::getButtonsControlMode()
{
  return buttonsControlMode;
}

void State::logButtonsMode()
{
  String text = "Switched to ";
  switch (buttonsControlMode)
  {
    case DISABLED:        text += "DISABLED"; break;
    case HEAD_CONTROL:    text += "HEAD_CONTROL"; break;
    case MOTORS_CONTROL:  text += "MOTORS_CONTROL"; break;
    default:              text += "OTHER_CONTROL";
  }
  Message::debugOutput(text);
}

