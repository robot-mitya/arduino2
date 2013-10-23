// -------------------------------------------------------------------------------------
// file = "rm_btn.h" (Robot Mitya's Romeo buttons header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   This static class to handle buttons on Robot Mitya's Romeo controller.
//   Tested on Romeo All-in-one Controller v1.1 and v2.
// -------------------------------------------------------------------------------------

#ifndef rm_btn_h
#define rm_btn_h

//#include <Arduino.h>

namespace robot_mitya
{
  enum ButtonState
  {
    RELEASED = 0,
    PRESSED = 1
  };
  
  enum Button
  {
    NONE = -1,
    S1 = 0,
    S2 = 1,
    S3 = 2,
    S4 = 3,
    S5 = 4
  };
  
  typedef void (*RomeoButtonsHandler)(ButtonState, Button);
  
  class RomeoButtons
  {
    public:
      static void initialize(int buttonsPin);
      static void setHandler(RomeoButtonsHandler handler);
      static void refresh(int analogButtonsValue);
    private:
      static Button getButton(int analogValue);
  };
}

#endif
