#include "rm_msg.h"
#include "rm_cfg.h"
#include <HardwareSerial.h>

using namespace robot_mitya;

#ifdef USBCON
  #define ROBO_SERIAL Serial//1
  #define DEBUG_SERIAL Serial
#else
  #define ROBO_SERIAL Serial
#endif

static String messageBuffer = "";

void Message::initialize()
{
  ROBO_SERIAL.begin(Cfg::SERIAL_BAUD_RATE);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.begin(Cfg::SERIAL_BAUD_RATE);
#endif
}

void Message::debugOutput(String text)
{
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println(text);
#endif
}

void Message::send(String message)
{
  ROBO_SERIAL.print(message);
}

void Message::send(String command, unsigned int value)
{
  String message = buildMessage(command, value);
  send(message);
}

void Message::sendResult(int resultCode)
{
  send("#", resultCode);
}

String Message::loadIncoming()
{
  while (ROBO_SERIAL.available() > 0)
  {
    messageBuffer += (char)ROBO_SERIAL.read();
  }
  
  return messageBuffer;
}

void Message::processIncomingMessages(String incoming, void (*handler)(String, int))
{
// Размер буфера
  int bufferLength = incoming.length();

  if (bufferLength < Cfg::MESSAGELENGTH)
  {
    messageBuffer = incoming;
    return;
  }
//  Serial1.println("incoming: " + incoming);

  // Последовательно извлекаю из полученной строки команды длиной MESSAGELENGTH символов.
  // Что не уместилось пойдёт снова в MessageBuffer.
  int i = 0;
  String message;

  while (i < bufferLength - Cfg::MESSAGELENGTH+1)
  { //Ищем первый символ не-цифру в шестнадцатиричом исчислении. Это будет комманда значит.
    char firstChar = incoming[i];
    if (((firstChar >= '0') && (firstChar <= '9')) ||
        ((firstChar >= 'A') && (firstChar <= 'F')) ||
        ((firstChar >= 'a') && (firstChar <= 'f')))
    {  //Оказалась цифра
      sendResult(RET_COMMAND_CHAR_MISSED);
      i++;
    }
    else
    {  //Попалась не цифра
        message = incoming.substring(i, i + Cfg::MESSAGELENGTH);
      
        processMessage(message, handler);
        i += Cfg::MESSAGELENGTH;        
    }
  }
  messageBuffer = incoming.substring(i, bufferLength); 
}

String Message::buildMessage(String command, unsigned int value)
{
  String result = String(value, HEX);
  result.toUpperCase();
  while (result.length() < 4) result = "0" + result;
  return command + result;
}

boolean Message::parseMessage(String message, String &command, int &value)
{
  if (message.length() != Cfg::MESSAGELENGTH)
  {
    command = "";
    value = 0;
    return false;
  }
    
  command = (String)message[0];
  int digit1, digit2, digit3, digit4;
  if (hexCharToInt(message[1], digit1) && hexCharToInt(message[2], digit2) &&
    hexCharToInt(message[3], digit3) && hexCharToInt(message[4], digit4))
  {
    value = 4096 * digit1 + 256 * digit2 + 16 * digit3 + digit4;
    return true;
  }
  else
  {
    command = "";
    value = 0;
    return false;
  }
}

boolean Message::hexCharToInt(char ch, int &value)
{
  if ((ch >= '0') && (ch <= '9'))
    value = ch - '0';
  else if ((ch >= 'A') && (ch <= 'F'))
    value = 10 + ch - 'A';
  else if ((ch >= 'a') && (ch <= 'f'))
    value = 10 + ch - 'a';
  else
  {
    value = 0;
    return false;
  }
  
  return true;
}

// Процедура обработки сообщения:
void Message::processMessage(String message, void (*handler)(String, int))
{
  if (Cfg::ECHO_MODE) send(message);
  
  // Парсер команды:
  String command;
  int value;
  if (! parseMessage(message, command, value))
  {
    ROBO_SERIAL.flush();
    sendResult(RET_WRONG_MESSAGE);
    return;
  }
  
  if (handler != NULL)
  {
    handler(command, value);
  }
}

