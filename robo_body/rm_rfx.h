#ifndef rm_rfx_h
#define rm_rfx_h

#include <RoboScript.h>

namespace robot_mitya
{
  enum ReflexKind
  {
    HAPPY = 0x0101,
    PLAY = 0x0102,
    SAD = 0x0103,
    ANGRY = 0x0104,
    DANCE = 0x0105,
    NOSE = 0x0106
  };

  class Reflex
  {
    public:
      static void initialize();
      static void refresh();
      static boolean start(ReflexKind reflexKind);
    private:
      static void initializeHappyReflex();
      static void initializePlayReflex();
      static void initializeSadReflex();
      static void initializeAngryReflex();
      static void initializeDanceReflex();
      static void initializeNoseReflex();
      static void fillAction(RoboAction* roboAction, uint32_t command, uint32_t value, uint16_t delay);
  };
}

#endif