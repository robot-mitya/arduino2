#include <SmartServo.h>
#include "rm_equ.h"
#include "rm_act.h"
#include "rm_cfg.h"
#include "rm_div.h"
#include "rm_btn.h"
#include "rm_msg.h"
#include "rm_sts.h"
#include "rm_rfx.h"

#ifdef USBCON    // For Leonardo (Romeo V2) board we use SoftwareServo library, because of lack of Timers.
  #include <SoftwareServo.h>
#else
  #include <Servo.h>
#endif

using namespace robot_mitya;

static SmartServo* servoHeadHorizontal = new SmartServo();
static SmartServo* servoHeadVertical = new SmartServo();
static SmartServo* servoTail = new SmartServo();

static bool isHeadHorizontalServoBusy;
static bool isHeadVerticalServoBusy;

static VoltageDivider* voltageDividerBattery = new VoltageDivider(Cfg::VOLTAGE_BATTERY_DIVIDER_INDEX,
  Cfg::VOLTAGE_BATTERY_PIN, Cfg::AREF_VOLTAGE, Cfg::VOLTAGE_BATTERY_R1, Cfg::VOLTAGE_BATTERY_R2);
static VoltageDivider* voltageDividerCharger = new VoltageDivider(Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX,
  Cfg::VOLTAGE_CHARGER_PIN, Cfg::AREF_VOLTAGE, Cfg::VOLTAGE_CHARGER_R1, Cfg::VOLTAGE_CHARGER_R2);

// Voltage divider to identify charging process. Plugging in an external power source should start charging.
// When power source is plugged off the charging must be interrupted.
static VoltageDivider* chargerListener = new VoltageDivider(Cfg::VOLTAGE_CHARGER_DIVIDER_INDEX,
  Cfg::VOLTAGE_CHARGER_PIN, Cfg::AREF_VOLTAGE, Cfg::VOLTAGE_CHARGER_R1, Cfg::VOLTAGE_CHARGER_R2);
static bool connectedToCharger = false;

void Equipment::initialize()
{
  const int MAGIC_DELAY = 30; //todo: it doesn't help in #5 issue
  
  // Initializing headlights:
  pinMode(Cfg::LIGHT_PIN, OUTPUT);
  delay(MAGIC_DELAY);
  executeInstruction(Cfg::INSTRUCTION_HEADLIGHTS_OFF);

  // Initializing accumulators' charging:
  pinMode(Cfg::MAIN_ACCUMULATOR_CHARGER_PIN, OUTPUT);
  delay(MAGIC_DELAY);
  executeInstruction(Cfg::INSTRUCTION_MAIN_ACCUMULATOR_CHARGE_OFF);
  pinMode(Cfg::PHONE_ACCUMULATOR_CHARGER_PIN, OUTPUT);
  delay(MAGIC_DELAY);
  executeInstruction(Cfg::INSTRUCTION_PHONE_ACCUMULATOR_CHARGE_OFF);

  // Initializing and setting horizontal servo into install-phone position:
//  pinMode(Cfg::SERVO_HEAD_HORIZONTAL_PIN, OUTPUT);
  servoHeadHorizontal->attach(
    Cfg::SERVO_HEAD_HORIZONTAL_PIN,
    Cfg::SERVO_HEAD_HORIZONTAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_HORIZONTAL_MAX_DEGREE);
  delay(MAGIC_DELAY);
  moveHead(HORIZONTAL, Cfg::SERVO_HEAD_HORIZONTAL_DEFAULT_STATE);
  isHeadHorizontalServoBusy = false;

  // Initializing and setting vertical servo into install-phone position:
//  pinMode(Cfg::SERVO_HEAD_VERTICAL_PIN, OUTPUT);
  servoHeadVertical->attach(
    Cfg::SERVO_HEAD_VERTICAL_PIN,
    Cfg::SERVO_HEAD_VERTICAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_VERTICAL_MAX_DEGREE);
  delay(MAGIC_DELAY);
  moveHead(VERTICAL, Cfg::SERVO_HEAD_VERTICAL_DEFAULT_STATE);
  isHeadVerticalServoBusy = false;

  // Setting up the tail:
//  pinMode(Cfg::SERVO_TAIL_PIN, OUTPUT);
  servoTail->attach(
    Cfg::SERVO_TAIL_PIN,
    Cfg::SERVO_TAIL_MIN_DEGREE,
    Cfg::SERVO_TAIL_MAX_DEGREE);
  delay(MAGIC_DELAY);
  moveTail(Cfg::SERVO_TAIL_DEFAULT_STATE);

  // Motors initializing:
  pinMode(Cfg::MOTOR_LEFT_SPEED_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_LEFT_DIRECTION_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_RIGHT_SPEED_PIN, OUTPUT);
  pinMode(Cfg::MOTOR_RIGHT_DIRECTION_PIN, OUTPUT);
  delay(MAGIC_DELAY);
  moveMotor("G", 0);

  // Initializing buttons:
  if (Cfg::USE_BUTTONS)
  {
    RomeoButtons::initialize(Cfg::BUTTONS_PIN);
    RomeoButtons::setHandler(Equipment::buttonsHandler);
  }

  // Initializing robot's state:
  State::initialize();
  
  connectedToCharger = false;
  chargerListener->setTimer(Cfg::CHARGER_LISTENER_DELAY, chargerHandler);
}

void Equipment::refresh()
{
  voltageDividerBattery->refresh();
  voltageDividerCharger->refresh();
  chargerListener->refresh();
  
  Reflex::refresh();
  
  // Swinging head in horizontal plane.
  isHeadHorizontalServoBusy = servoHeadHorizontal->update();
  if (isHeadHorizontalServoBusy)
  {
    //Message::sendResult(10);
    // We know the angle here, we can transmit...
  }

  // Swinging head in vertical plane.
  isHeadVerticalServoBusy = servoHeadVertical->update();
  if (isHeadVerticalServoBusy)
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

void Equipment::executeInstruction(int value)
{
  switch (value) {
    case Cfg::INSTRUCTION_HEADLIGHTS_OFF:
      digitalWrite(Cfg::LIGHT_PIN, LOW);
      Cfg::stateHeadlights = value;
      Message::send("I", value);
      break;
    case Cfg::INSTRUCTION_HEADLIGHTS_ON:
      digitalWrite(Cfg::LIGHT_PIN, HIGH);
      Cfg::stateHeadlights = value;
      Message::send("I", value);
      break;
    case Cfg::INSTRUCTION_MAIN_ACCUMULATOR_CHARGE_OFF:
      digitalWrite(Cfg::MAIN_ACCUMULATOR_CHARGER_PIN, LOW);
      Cfg::stateMainAccumulatorCharge = value;
      Message::send("I", value);
      break;
    case Cfg::INSTRUCTION_MAIN_ACCUMULATOR_CHARGE_ON:
      digitalWrite(Cfg::MAIN_ACCUMULATOR_CHARGER_PIN, HIGH);
      Cfg::stateMainAccumulatorCharge = value;
      Message::send("I", value);
      break;
    case Cfg::INSTRUCTION_PHONE_ACCUMULATOR_CHARGE_OFF:
      digitalWrite(Cfg::PHONE_ACCUMULATOR_CHARGER_PIN, LOW);
      Cfg::statePhoneAccumulatorCharge = value;
      Message::send("I", value);
      break;
    case Cfg::INSTRUCTION_PHONE_ACCUMULATOR_CHARGE_ON:
      digitalWrite(Cfg::PHONE_ACCUMULATOR_CHARGER_PIN, HIGH);
      Cfg::statePhoneAccumulatorCharge = value;
      Message::send("I", value);
      break;
  }
}

void Equipment::moveHead(HeadPlane plane, int degree)
{
  if (plane == HORIZONTAL)
  {
    servoHeadHorizontal->write(degree);
  }
  else if (plane == VERTICAL)
  {
    servoHeadVertical->write(degree);
  }
}

void Equipment::rotateHead(HeadPlane plane, signed int period)
{
  if (plane == HORIZONTAL)
  {
    servoHeadHorizontal->stop();
    isHeadHorizontalServoBusy = true;
    servoHeadHorizontal->startTurn(period, true);
  }
  else if (plane == VERTICAL)
  {
    servoHeadVertical->stop();
    isHeadVerticalServoBusy = true;
    servoHeadVertical->startTurn(period, true);
  }
}

void Equipment::swingHead(HeadPlane plane, int mode)
{
  if (plane == HORIZONTAL)
  {
    servoHeadHorizontal->stop();
    if ((mode == 1) || (mode == 2))
    {
      isHeadHorizontalServoBusy = true;
      servoHeadHorizontal->startSwing(mode, 400, 2.5, 60, 0.75, true);
    }
  }
  else if (plane == VERTICAL)
  {
    servoHeadVertical->stop();
    if ((mode == 1) || (mode == 2))
    {
      isHeadVerticalServoBusy = true;
      servoHeadVertical->startSwing(mode, 400, 2.5, 30, 0.8, true);
    }
  }
}

void Equipment::swingHeadEx(HeadPlane plane, int mode, signed long period, double iterations,
    signed long amplitude, double amplitudeCoefficient, bool positiveDirection)
{
  SmartServo* servo;
  if (plane == HORIZONTAL)
  {
    servo = servoHeadHorizontal;
  }
  else if (plane == VERTICAL)
  {
    servo = servoHeadVertical;
  }
  
  servo->stop();

  if (plane == HORIZONTAL)
  {
    isHeadHorizontalServoBusy = true;
  }
  else if (plane == VERTICAL)
  {
    isHeadVerticalServoBusy = true;
  }

  servo->startSwing(mode, period, iterations, amplitude, amplitudeCoefficient, positiveDirection);
}
    
int Equipment::getHeadPosition(HeadPlane plane)
{
  if (plane == HORIZONTAL)
    return servoHeadHorizontal->read();
  else
    return servoHeadVertical->read();
}

bool Equipment::isHeadServoBusy(HeadPlane plane)
{
  if (plane == HORIZONTAL)
    return isHeadHorizontalServoBusy;
  else
    return isHeadVerticalServoBusy;
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

void Equipment::swingTailEx(int mode, signed long period, double iterations,
    signed long amplitude, double amplitudeCoefficient, bool positiveDirection)
{
  servoTail->stop();
  servoTail->startSwing(mode, period, iterations, amplitude, amplitudeCoefficient, positiveDirection);
}
    
int Equipment::getTailPosition()
{
  return servoTail->read();
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

void Equipment::chargerHandler(int voltageDivider, unsigned int voltage)
{
  if (voltage > Cfg::CHARGER_MIN_VOLTAGE)
  {
    if (!connectedToCharger)
    {
      executeInstruction(Cfg::INSTRUCTION_MAIN_ACCUMULATOR_CHARGE_ON);
      connectedToCharger = true;
    }
  }
  else
  {
    if (connectedToCharger)
    {
      executeInstruction(Cfg::INSTRUCTION_MAIN_ACCUMULATOR_CHARGE_OFF);
      connectedToCharger = false;
    }
  }
}

