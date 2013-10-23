// -------------------------------------------------------------------------------------
// file = "robot_body_bt.ino"
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   Simple sketch that demonstrates how to control Robot through serial connection.
//   It could be USB wire or Bluetooth connection.
// -------------------------------------------------------------------------------------

#include "rm_act.h"
#include "rm_cfg.h"
#include "rm_equ.h"
#include "rm_msg.h"

#include <Arduino.h>
#ifdef USBCON    // For Leonardo (Romeo V2) board we use SoftwareServo library, because of lack of Timers.
  #include <SoftwareServo.h>
#else
  #include <Servo.h>
#endif
#include <SmartServo.h>
#include <HardwareSerial.h>

using namespace robot_mitya;

// Sketch initialization
void setup()
{
  // Initializing serial port
  Message::initialize();
  
  // Initializing controller's pins, motors, servos and headlights states
  Equipment::initialize();

  Message::debugOutput("------------- Robot Mitya -------------");
}

// Sketch iteration
void loop()
{
  String incomingText = Message::loadIncoming();
  Message::processIncomingMessages(incomingText, Action::execute);
  Equipment::refresh();
}

