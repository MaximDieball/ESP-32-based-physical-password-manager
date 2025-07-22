#include <Arduino.h>
#include "InputManager.h"

void InputManager::keyboardUpdate(){
  // Check physical keyboard over I2C
  Wire.requestFrom(0x55, 1);  // Address of T-Deck keyboard
  while (Wire.available()) {
      char key = Wire.read();
      if (key != 0x00 && scrManager.focusedTextarea != NULL) {
          if (key == '\b') {
              lv_textarea_del_char(scrManager.focusedTextarea);  // backspace
              Serial.println("backsapce");
          } else if (key == '\r' && scrManager.enterFunc){
            scrManager.enterFunc();   // enter
            Serial.println("enter");
          } else {
              lv_textarea_add_char(scrManager.focusedTextarea, key);  // add char to input field
          }
      }
  }

}

void InputManager::touchUpdate(){
     // Touch Input
  if (touch.isPressed()) {
        int16_t x[5] = {0}, y[5] = {0};
        uint8_t touched = touch.getPoint(x, y, 1);
        if (touched > 0) {
            this->currentY = y[0];
            this->currentX = x[0];
            
            if(!this->swipe){
              this->swipe = true;
              this->startX = currentX;
              this->startY = currentY;
            }            
        } else if(this->swipe){
           unsigned long now = millis();
           if(this->currentY - this->startY > 5 && now - lastSwipeTime > 150){
              Serial.printf("swipe up startY: %d  currentY: %d\n", this->startY, currentY);
              if(scrManager.swipeFunc){
                scrManager.swipeFunc("UP");
              }
              lastSwipeTime = now;
           } else if(this->startY - this->currentY > 5 && now - lastSwipeTime > 150){
               Serial.printf("swipe up startY: %d  currentY: %d\n", this->startY, currentY);
              if(scrManager.swipeFunc){
                scrManager.swipeFunc("DOWN");
              }
              lastSwipeTime = now;
            }
           this->swipe = false;         
        }
    } 
}
