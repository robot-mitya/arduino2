#include "rm_act.h"
#include "rm_cfg.h"
#include "rm_equ.h"
#include "rm_msg.h"
#include "rm_rfx.h"

using namespace robot_mitya;

void Action::execute(String command, int value)
{
  if ((command == "W") || (command == "Z"))
  {
    Message::sendResult(Message::RET_ILLEGAL_COMMAND_OUT_OF_ROBOSCRIPT);
    return;
  }

  boolean processedCommand = false;

  if (isMotorCommand(command))
  {
    Equipment::moveMotor(command, value);
    processedCommand = true;
  }
  
  if (isMoveHeadCommand(command))
  {
    if ((command == "H") && (!Equipment::isHeadServoBusy(HORIZONTAL)))
    {
      Equipment::moveHead(HORIZONTAL, value);
      processedCommand = true;
    }
    else if ((command == "V") && (!Equipment::isHeadServoBusy(VERTICAL)))
    {
      Equipment::moveHead(VERTICAL, value);
      processedCommand = true;
    }
  }

  if (isRotateHeadCommand(command))
  {
    signed int periodInSeconds = value * 10;
    if (command == "h")
      Equipment::rotateHead(HORIZONTAL, periodInSeconds);
    else
      Equipment::rotateHead(VERTICAL, periodInSeconds);
    processedCommand = true;
  }

  if (isSwingHeadCommand(command))
  {
    if (command == "n")
      Equipment::swingHead(HORIZONTAL, value);
    else
      Equipment::swingHead(VERTICAL, value);
    processedCommand = true;
  }

  if (isMoveTailCommand(command))
  {
    Equipment::moveTail(value);
    processedCommand = true;
  }

  if (isSwingTailCommand(command))
  {
    Equipment::swingTail(value);
    processedCommand = true;
  }
  
  if (isStartReflexCommand(command))
  {
    processedCommand = Reflex::start((ReflexKind)value);
  }

  if (command == "I")
  {
    if (value == Cfg::INSTRUCTION_STATE_REQUEST)
    {
      voltageTimerHandler(0, Equipment::getVoltage(0));
      voltageTimerHandler(1, Equipment::getVoltage(1));
      Message::send("I", Cfg::stateHeadlights);
      Message::send("I", Cfg::stateMainAccumulatorCharge);
      Message::send("I", Cfg::statePhoneAccumulatorCharge);
    }
    else
    {
      Equipment::executeInstruction(value);
    }
    processedCommand = true;
  }

  if (command == "=")
  {
    int voltageDividerIndex = value >> 12;
    if ((voltageDividerIndex != Cfg::VOLTAGE_BATTERY_DIVIDER_INDEX) && (voltageDividerIndex != Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX))
    {
      Message::sendResult(Message::RET_WRONG_VOLTAGE_DIVIDER);
      return;
    }
    unsigned int timerValue = (value & 0x0FFF) * 10;
    if (timerValue == 0)
    {
      voltageTimerHandler(voltageDividerIndex, Equipment::getVoltage(voltageDividerIndex));
    }
    Equipment::setVoltageTimer(voltageDividerIndex, timerValue, voltageTimerHandler);
    processedCommand = true;
  }

  if (processedCommand)
  {
    Message::sendResult(Message::RET_OK);
  }
}

boolean Action::isMotorCommand(String command)
{
  return (command == "L") || (command == "R") || (command == "G");
}

boolean Action::isMoveHeadCommand(String command)
{
  return (command == "H") || (command == "V");
}

boolean Action::isRotateHeadCommand(String command)
{
  return (command == "h") || (command == "v");
}

boolean Action::isSwingHeadCommand(String command)
{
  return (command == "y") || (command == "n");
}

boolean Action::isMoveTailCommand(String command)
{
  return (command == "T");
}

boolean Action::isSwingTailCommand(String command)
{
  return (command == "t");
}

boolean Action::isStartReflexCommand(String command)
{
  return (command == "M");
}

void Action::voltageTimerHandler(int voltageDivider, unsigned int voltage)
{
  Message::send("~", (voltageDivider << 12) | voltage);
}

