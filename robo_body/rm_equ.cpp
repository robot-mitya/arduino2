#include <SmartServo.h>
#include "rm_equ.h"
#include "rm_act.h"
#include "rm_cfg.h"
#include "rm_div.h"
#include "rm_btn.h"
#include "rm_msg.h"
#include "rm_sts.h"

#ifdef USBCON    // For Leonardo (Romeo V2) board we use SoftwareServo library, because of lack of Timers.
  #include <SoftwareServo.h>
#else
  #include <Servo.h>
#endif

using namespace robot_mitya;

static SmartServo* servoHeadHorizontal = new SmartServo();
static SmartServo* servoHeadVertical = new SmartServo();
static SmartServo* servoTail = new SmartServo();

static VoltageDivider* voltageDividerBattery = new VoltageDivider(Cfg::VOLTAGE_BATTERY_DIVIDER_INDEX,
  Cfg::VOLTAGE_BATTERY_PIN, Cfg::AREF_VOLTAGE, Cfg::VOLTAGE_BATTERY_R1, Cfg::VOLTAGE_BATTERY_R2);
static VoltageDivider* voltageDividerCharger = new VoltageDivider(Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX,
  Cfg::VOLTAGE_CHARGER_PIN, Cfg::AREF_VOLTAGE, Cfg::VOLTAGE_CHARGER_R1, Cfg::VOLTAGE_CHARGER_R2);

void Equipment::initialize()
{
  // Initializing headlights:
  pinMode(Cfg::LIGHT_PIN, OUTPUT);
  setHeadlightState(0);

  // Initializing and setting horizontal servo into install-phone position:
  pinMode(Cfg::SERVO_HEAD_HORIZONTAL_PIN, OUTPUT);
  servoHeadHorizontal->attach(
    Cfg::SERVO_HEAD_HORIZONTAL_PIN,
    Cfg::SERVO_HEAD_HORIZONTAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_HORIZONTAL_MAX_DEGREE);
  moveHead("H", Cfg::SERVO_HEAD_HORIZONTAL_DEFAULT_STATE);

  // Initializing and setting vertical servo into install-phone position:
  pinMode(Cfg::SERVO_HEAD_VERTICAL_PIN, OUTPUT);
  servoHeadVertical->attach(
    Cfg::SERVO_HEAD_VERTICAL_PIN,
    Cfg::SERVO_HEAD_VERTICAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_VERTICAL_MAX_DEGREE);
  moveHead("V", Cfg::SERVO_HEAD_VERTICAL_DEFAULT_STATE);

  // Setting up the tail:
  pinMode(Cfg::SERVO_TAIL_PIN, OUTPUT);
  servoTail->attach(
    Cfg::SERVO_TAIL_PIN,
    Cfg::SERVO_TAIL_MIN_DEGREE,
    Cfg::SERVO_TAIL_MAX_DEGREE);
  moveTail(Cfg::SERVO_TAIL_DEFAULT_STATE);

  // Motors initializing:
  pinMode(Cfg::MOTOR_LEFT_SPEED_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_LEFT_DIRECTION_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_RIGHT_SPEED_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_RIGHT_DIRECTION_PIN, OUTPUT);
  moveMotor("G", 0);

  // Initializing buttons:
  if (Cfg::USE_BUTTONS)
  {
    RomeoButtons::initialize(Cfg::BUTTONS_PIN);
    RomeoButtons::setHandler(Equipment::buttonsHandler);
  }
  
  // Initializing robot's state:
  State::initialize();
}

void Equipment::refresh()
{
  voltageDividerBattery->refresh();
  voltageDividerCharger->refresh();
  
  // Swinging head in horizontal plane.
  if (servoHeadHorizontal->update())
  {
    // We know the angle here, we can transmit...
  }

  // Swinging head in vertical plane.
  if (servoHeadVertical->update())
  {
    // We know the angle here, we can transmit...
  }

  // Tail swinging.
  if (servoTail->update())
  {
    // We know the angle here, we can transmit...
  }
  
  // Handle buttons.
  if (Cfg::USE_BUTTONS)
  {
    RomeoButtons::refresh(analogRead(Cfg::BUTTONS_PIN));
  }
  
  // For Leonardo (Romeo V2) board we use SoftwareServo library because of Timers lack.
  #ifdef USBCON
    SoftwareServo::refresh();
  #endif
}

void Equipment::setHeadlightState(int value)
{
  if (value == 0)
  {
    digitalWrite(Cfg::LIGHT_PIN, LOW);
  }
  else if (value == 1)
  {
    digitalWrite(Cfg::LIGHT_PIN, HIGH);
  }
}

void Equipment::moveHead(String plane, int degree)
{
  servoHeadHorizontal->stop();
  servoHeadVertical->stop();
  if (plane == "H") // (horizontal plane)
  {
    servoHeadHorizontal->write(degree);
  }
  else if (plane == "V") // (vertical plane)
  {
    servoHeadVertical->write(degree);
  }
}

void Equipment::rotateHead(String plane, signed int period)
{
  if (plane == "h") // (horizontal plane)
  {
    servoHeadHorizontal->stop();
    servoHeadHorizontal->startTurn(period, true);
  }
  else if (plane == "v") // (vertical plane)
  {
    servoHeadVertical->stop();
    servoHeadVertical->startTurn(period, true);
  }
}

void Equipment::swingHead(String plane, int mode)
{
  if (plane == "n") // (horizontal plane)
  {
    servoHeadHorizontal->stop();
    if ((mode == 1) || (mode == 2))
    {
      servoHeadHorizontal->startSwing(mode, 400, 2.5, 60, 0.75, true);
    }
  }
  else if (plane == "y") // (vertical plane)
  {
    servoHeadVertical->stop();
    if ((mode == 1) || (mode == 2))
    {
      servoHeadVertical->startSwing(mode, 400, 2.5, 30, 0.8, true);
    }
  }
}

void Equipment::moveTail(int degree)
{
  servoTail->write(degree);
}

void Equipment::swingTail(int mode)
{
  servoTail->stop();
  moveTail(Cfg::SERVO_TAIL_DEFAULT_STATE);
  if ((mode == 1) || (mode == 2))
  {
    servoTail->startSwing(mode, 250, 6, 70, 0.9, true);
  }
}

void Equipment::moveMotor(String side, int speed)
{
  bool directionPinValue = speed > 0 ? LOW : HIGH;
  
  if (speed < 0) {
    speed = - speed;
  }
 
  if (speed > 255) {
    speed = 255;
  }
  
  if (speed == 0) {
    directionPinValue = LOW; // turns motor off
  }
  
  if ((side == "L") || (side == "G")) {
    digitalWrite(Cfg::MOTOR_LEFT_DIRECTION_PIN, directionPinValue);
    analogWrite(Cfg::MOTOR_LEFT_SPEED_PIN, speed);
  }
  
  if ((side == "R") || (side == "G")) {
    digitalWrite(Cfg::MOTOR_RIGHT_DIRECTION_PIN, directionPinValue);
    analogWrite(Cfg::MOTOR_RIGHT_SPEED_PIN, speed);
  }
}

unsigned int Equipment::getVoltageDividerPinValue(int dividerPin)
{
  return analogRead(dividerPin);
}

unsigned int Equipment::getVoltage(int dividerIndex)
{
  if (dividerIndex == Cfg::VOLTAGE_BATTERY_DIVIDER_INDEX)
  {
    return voltageDividerBattery->getVoltage();
  }
  if (dividerIndex == Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX)
  {
    return voltageDividerCharger->getVoltage();
  }
  return 0;
}

void Equipment::setVoltageTimer(int dividerIndex, unsigned long timerDelay, void (*handler)(int, unsigned int))
{
  if (dividerIndex == Cfg::VOLTAGE_BATTERY_DIVIDER_INDEX)
  {
    voltageDividerBattery->setTimer(timerDelay, handler);
  }
  else if (dividerIndex == Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX)
  {
    voltageDividerCharger->setTimer(timerDelay, handler);
  }
}

void Equipment::buttonsHandler(ButtonState buttonState, Button button)
{
  if (button == S5)
  {
    if (buttonState == PRESSED) State::setNextButtonsControlMode();
    return;
  }

  switch (State::getButtonsControlMode())
  {
    case HEAD_CONTROL:
    {
      int const period = buttonState == PRESSED ? 400 : 0;
      switch (button)
      {
        // Right:
        case S1: Action::execute("h", -period); break;
        // Forward:
        case S2: Action::execute("v", -period); break;
        // Left:
        case S3: Action::execute("h", period); break;
        // Backward:
        case S4: Action::execute("v", period); break;
      }
      break;
    }
    case MOTORS_CONTROL:
    {
      int const speed = buttonState == PRESSED ? 128 : 0;
      switch (button)
      {
        // Right:
        case S1: Action::execute("L", speed); Action::execute("R", -speed); break;
        // Forward:
        case S2: Action::execute("G", speed); break;
        // Left:
        case S3: Action::execute("L", -speed); Action::execute("R", speed); break;
        // Backward:
        case S4: Action::execute("G", -speed); break;
      }
      break;
    }
    case OTHER_CONTROL:
    {
      switch (button)
      {
        // Right:
        case S1: break;
        // Forward:
        case S2: buttonState == PRESSED ? Action::execute("I", 1) : Action::execute("I", 0); break;
        // Left:
        case S3: break;
        // Backward:
        case S4: if (buttonState == PRESSED) Action::execute("t", 1); break;
      }
      break;
    }
  }
}

