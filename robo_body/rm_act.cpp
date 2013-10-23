#include "rm_act.h"
#include "rm_cfg.h"
#include "rm_equ.h"
#include "rm_msg.h"

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
    Equipment::moveHead(command, value);
    processedCommand = true;
  }

  if (isRotateHeadCommand(command))
  {
    signed int periodInSeconds = value * 10;
    Equipment::rotateHead(command, periodInSeconds);
    processedCommand = true;
  }

  if (isSwingHeadCommand(command))
  {
    Equipment::swingHead(command, value);
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

  if (command == "I")
  {
    Equipment::setHeadlightState(value);
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

void Action::voltageTimerHandler(int voltageDivider, unsigned int voltage)
{
  Message::send("~", voltage);
}







//  switch(command[0]) {  // Сейчас у нас односимвольные команды, но на случай развития команда определена как String
//    case 'M':
//    {
//      switch(value)
//      {
//        case 0x0101:
//        {
//          happyReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break; 
//        }
//        case 0x0102:
//        {
//          const int ReadyToPlayVerticalDegree = 70;
//          int verticalAmplitude = abs((ReadyToPlayVerticalDegree - servoHeadVertical.read()) * 2);
//          boolean swingDirection = ReadyToPlayVerticalDegree > servoHeadVertical.read();
//          servoHeadVertical.startSwing(2, 400, 0.25, verticalAmplitude, 1, swingDirection);
//          servoHeadHorizontal.startSwing(2, 250, 3.5, 40, 0.8, true);
//          servoTail.startSwing(value, 250, 6, 70, 0.9, true);
//          readyToPlayReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break; 
//        }
//        case 0x0103:
//        {
//          int verticalAmplitude = (servoHeadVertical.read() - Configuration::SERVO_HEAD_VERTICAL_MIN_DEGREE) * 2;
//          servoHeadVertical.startSwing(2, 6000, 0.25, verticalAmplitude, 1, false);
//          servoHeadHorizontal.startSwing(2, 750, 2, 60, 0.6, true);
//          sadReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break;
//        }
//        case 0x0104:
//        {
//          angryReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break;
//        }
//        case 0x0105:  
//        {
//          int musicTacts = 12;
//          servoHeadVertical.startSwing(1, musicPeriod, musicTacts, 30, 1, true);
//          servoHeadHorizontal.startSwing(2, musicPeriod * musicTacts / 1.5, 1.5, 50, 1, true);
//          servoTail.startSwing(value, musicPeriod, musicTacts, 70, 1, true);
//          musicReflexStart();
//          break;
//        }
//        default:
//        {
//          sendMessageToRobot(command, value);
//        }
//      } // (M-command's switch)
//      break;
//    }
////    case 's':    
////    {
////      // Команда выстрела пушке:
////      irsend.sendSony(0xABC0, 16);
////      irrecv.enableIRIn(); // (надо для повторной инициализации ИК-приёмника)
////      break;
////    }

