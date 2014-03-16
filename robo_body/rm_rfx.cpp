#include <SmartServo.h>
#include "rm_rfx.h"
#include "rm_act.h"
#include "rm_equ.h"
#include "rm_cfg.h"

using namespace robot_mitya;

static RoboScript* reflex;

const int MUSIC_PERIOD = 535;

void Reflex::refresh()
{
  if (reflex == NULL)
  {
    return;
  }
  
  if (!reflex->getIsExecuting())
  {
    reflex = NULL;
    return;
  }
  
  String command;
  int value;
  if (reflex->hasActionToExecute(command, value))
  {
    Action::execute(command, value);
  }
}

boolean Reflex::start(ReflexKind reflexKind)
{
  if (reflex != NULL)
  {
    return false;
  }

  if (reflexKind == HAPPY)
  {
    reflex = new RoboScript();
    reflex->initialize(1);
    RoboAction* act = new RoboAction();
    fillAction(act, 't', 1, 0);
    reflex->addAction(*act);
    reflex->startExecution();
    return true;
  }
  else if (reflexKind == PLAY)
  {
    const int playHeadVerticalDegree = 70;
    const int verticalAmplitude = abs((playHeadVerticalDegree - Equipment::getHeadPosition(VERTICAL)) * 2);
    const boolean swingDirection = playHeadVerticalDegree > Equipment::getHeadPosition(VERTICAL);
    Equipment::swingHeadEx(VERTICAL, 2, 400, 0.25, verticalAmplitude, 1, swingDirection);
    Equipment::swingHeadEx(HORIZONTAL, 2, 250, 3.5, 40, 0.8, true);
    Equipment::swingTailEx(2, 250, 6, 70, 0.9, true);
  
    const int kickSpeed = 192;
    const int kickDuration = 90;
    const int freezeDuration = 200;
    
    reflex = new RoboScript();
    reflex->initialize(8);
    RoboAction* act;
    for (int i = 0; i < 2; i++)
    {
      act = new RoboAction();
      fillAction(act, 'L', kickSpeed, kickDuration);
      reflex->addAction(*act);
  
      act = new RoboAction();
      fillAction(act, 'L', 0, freezeDuration);
      reflex->addAction(*act);
  
      act = new RoboAction();
      fillAction(act, 'R', kickSpeed, kickDuration);
      reflex->addAction(*act);
  
      act = new RoboAction();
      fillAction(act, 'R', 0, freezeDuration);
      reflex->addAction(*act);
    }
    reflex->startExecution();
    return true;
  }
  else if (reflexKind == SAD)
  {
    const int verticalAmplitude = (Equipment::getHeadPosition(VERTICAL) - Cfg::SERVO_HEAD_VERTICAL_MIN_DEGREE) * 2;
    Equipment::swingHeadEx(VERTICAL, 2, 6000, 0.25, verticalAmplitude, 1, false);
    Equipment::swingHeadEx(HORIZONTAL, 2, 750, 2, 60, 0.6, true);
    return true;
  }
  else if (reflexKind == ANGRY)
  {
    reflex = new RoboScript();
    reflex->initialize(3);
    
    RoboAction* act;
  
    act = new RoboAction();
    fillAction(act, 'G', -192, 100);
    reflex->addAction(*act);
  
    act = new RoboAction();
    fillAction(act, 'G', 255, 20);
    reflex->addAction(*act);
  
    act = new RoboAction();
    fillAction(act, 'G', 0, 0);
    reflex->addAction(*act);
    
    reflex->startExecution();
    return true;
  }
  else if (reflexKind == DANCE)
  {
    const int musicTacts = 12;
    Equipment::swingHeadEx(VERTICAL, 1, MUSIC_PERIOD, musicTacts, 30, 1, true);
    Equipment::swingHeadEx(HORIZONTAL, 2, MUSIC_PERIOD * musicTacts / 1.5, 1.5, 50, 1, true);
    Equipment::swingTailEx(2, MUSIC_PERIOD, musicTacts, 70, 1, true);
    return true;
  }
  else if (reflexKind == NOSE)
  {
    return true;
  }
  
  return false;
}

void Reflex::fillAction(RoboAction* roboAction, uint32_t command, uint32_t value, uint16_t delay)
{
  roboAction->Command = command;
  roboAction->Value = value;
  roboAction->Delay = delay;
}

