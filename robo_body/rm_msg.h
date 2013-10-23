#ifndef rm_msg_h
#define rm_msg_h

#include <Arduino.h>

namespace robot_mitya
{
  class Message
  {
    public:
      static const int RET_OK = 0;
      static const int RET_WRONG_MESSAGE = 1;
      static const int RET_UNKNOWN_COMMAND = 2; // Deprecated. Unknown commands are ignored now.
      static const int RET_ROBOSCRIPT_ILLEGAL_COMMAND = 3;
      static const int RET_ROBOSCRIPT_CORRUPT_SEQUENCE = 4;
      static const int RET_NO_MEMORY = 5;
      static const int RET_OUT_OF_MEMORY_BOUNDS = 6;
      static const int RET_ILLEGAL_COMMAND_OUT_OF_ROBOSCRIPT = 7;
      static const int RET_COMMAND_CHAR_MISSED = 8;
      static const int RET_WRONG_VOLTAGE_DIVIDER = 9;
    
      static void initialize();
      static void send(String command, unsigned int value);
      static void send(String message);
      static void sendResult(int resultCode);
      static void debugOutput(String text);
      static String loadIncoming();
      static void processIncomingMessages(String incoming, void (*handler)(String, int));
    private:
      static void processMessage(String message, void (*handler)(String, int));
      static String buildMessage(String command, unsigned int value);
      static boolean parseMessage(String message, String &command, int &value);
      static boolean hexCharToInt(char ch, int &value);
  };
}

#endif
