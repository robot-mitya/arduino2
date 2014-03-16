#include <SmartServo.h>
#include "rm_rfx.h"
#include "rm_act.h"

using namespace robot_mitya;

static RoboScript* happyReflex = new RoboScript();
static RoboScript* playReflex = new RoboScript();
static RoboScript* sadReflex = new RoboScript();
static RoboScript* angryReflex = new RoboScript();
static RoboScript* danceReflex = new RoboScript();
static RoboScript* noseReflex = new RoboScript();
RoboScript* executingReflex;

void Reflex::initialize()
{
  initializeHappyReflex();
  initializePlayReflex();
  initializeSadReflex();
  initializeAngryReflex();
  initializeDanceReflex();
  initializeNoseReflex();
}

void Reflex::refresh()
{
  if (executingReflex == NULL)
  {
    return;
  }
  
  if (!executingReflex->getIsExecuting())
  {
    executingReflex = NULL;
    return;
  }
  
  String command;
  int value;
  if (executingReflex->hasActionToExecute(command, value))
  {
    Action::execute(command, value);
  }
}

boolean Reflex::start(ReflexKind reflexKind)
{
  if (executingReflex != NULL)
  {
    return false;
  }
  
  switch (reflexKind)
  {
    case HAPPY:
      executingReflex = happyReflex;
      break;
    case PLAY:
      executingReflex = playReflex;
      break;
    case SAD:
      executingReflex = sadReflex;
      break;
    case ANGRY:
      executingReflex = angryReflex;
      break;
    case DANCE:
      executingReflex = danceReflex;
      break;
    case NOSE:
      executingReflex = noseReflex;
      break;
  }
  
  if (executingReflex != NULL)
  {
    executingReflex->startExecution();
    return true;
  }
  
  return false;
}

void Reflex::initializeHappyReflex()
{
  happyReflex->initialize(1);
  RoboAction* roboAction = new RoboAction();
  fillAction(roboAction, 't', 1, 4000);
  happyReflex->addAction(*roboAction);
}

void Reflex::fillAction(RoboAction* roboAction, uint32_t command, uint32_t value, uint16_t delay)
{
  roboAction->Command = command;
  roboAction->Value = value;
  roboAction->Delay = delay;
}

void Reflex::initializePlayReflex()
{
  const int kickSpeed = 192;
  const int kickDuration = 90;
  const int freezeDuration = 200;
  
  playReflex->initialize(8);
  RoboAction* act;
  for (int i = 0; i < 2; i++)
  {
    act = new RoboAction();
    fillAction(act, 'L', kickSpeed, kickDuration);
    playReflex->addAction(*act);

    act = new RoboAction();
    fillAction(act, 'L', 0, freezeDuration);
    playReflex->addAction(*act);

    act = new RoboAction();
    fillAction(act, 'R', kickSpeed, kickDuration);
    playReflex->addAction(*act);

    act = new RoboAction();
    fillAction(act, 'R', 0, freezeDuration);
    playReflex->addAction(*act);
  }
}

void Reflex::initializeSadReflex()
{
}

void Reflex::initializeAngryReflex()
{
}

void Reflex::initializeDanceReflex()
{
}

void Reflex::initializeNoseReflex()
{
}

