#pragma once
#include <Arduino.h>
#include "USBHIDKeyboard.h"

class HidManager {
  public:
      HidManager(USBHIDKeyboard& Keyboard)
        : Keyboard(Keyboard) {}

      String language = "GER";

      void typeString(String string);
      void typeEnter();
      void typeTab();
  private:
    USBHIDKeyboard Keyboard;
    void typeGER(String string);
    void typeENG(String string);
};
