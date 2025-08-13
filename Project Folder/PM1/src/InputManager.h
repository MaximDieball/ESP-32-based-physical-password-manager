#pragma once
#include <lvgl.h>
#include <Arduino.h>
#include <iostream>
#include <vector>
#include <string>

#include "TouchDrvGT911.hpp"

#include "ScreenManager.h"
#include "PasswordManager.h"

class InputManager{
  public:
    int16_t startX, startY;
    int16_t currentX, currentY;
    bool swipe = false;
    InputManager(ScreenManager& scrManager, TouchDrvGT911& touch) 
      : scrManager(scrManager), touch(touch) {}
    
    void keyboardUpdate();
    void touchUpdate(); 
    
  private:
    ScreenManager& scrManager;
    TouchDrvGT911& touch;
    unsigned long lastSwipeTime = 0;
};
