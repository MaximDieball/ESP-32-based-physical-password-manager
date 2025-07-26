#include "ScreenManager.h"
#include <Arduino.h>  
#include "SDManager.h"

void ScreenManager::createScreens() {
    screenArray[0] = createAddMasterPasswordScreen();
    Serial.println("CREATE SCREEN AddMasterPasswordScreen");  
    screenArray[1] = createLockScreen();
    Serial.println("CREATE SCREEN LockScreen");
    screenArray[2] = createHomeScreen();
    Serial.println("CREATE SCREEN HomeScreen");
    screenArray[3] = createPasswordManagerScreen();
    Serial.println("CREATE SCREEN PasswordManagerScreen");
    screenArray[4] = createPasswordScreen();
    Serial.println("CREATE SCREEN PasswordScreen");
}

void ScreenManager::changeScreen(int index) {
    Screen screen = screenArray[index];
    this->currentScreen = screen;
    
    // load screen
    lv_scr_load(screen.lvScreen);
    
    // set enterFunc
    this->enterFunc = screen.enterFunc;
    // set swipeFunc
    this->swipeFunc = screen.swipeFunc;
    
    // focus textarea
    lv_obj_t *textarea = screen.mainTextarea;
    this->focusedTextarea = textarea;  // focus text area / for physical keyboard
    if(textarea){
      lv_textarea_set_cursor_pos(textarea, LV_TEXTAREA_CURSOR_LAST);
      lv_obj_add_state(textarea, LV_STATE_FOCUSED);
    }

    // run onDisplayFunc
    if(screen.onDisplayFunc){
        Serial.println("running onDisplayFunc");
        screen.onDisplayFunc();
    }
  
    Serial.println("succesfully changed screen");
}

void ScreenManager::enterBtnFunc(lv_event_t *e){  
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);
  ScreenManager *self = static_cast<ScreenManager*>(lv_event_get_user_data(e)); // get self even though it is a static function 
  if(code == LV_EVENT_CLICKED){
    Serial.println("enter/ok button clicked");
    if (self && self->enterFunc){
      self->enterFunc();
    }
  }
}

// sets queued screen
void ScreenManager::queueScreen(int screenIndex){
  queuedScreen = screenIndex;
}

// changes screen if any screen is queued
void ScreenManager::screenChangeHandler(){
  if(queuedScreen != -1){
    changeScreen(queuedScreen);
    queuedScreen = -1;
  }
}

void ScreenManager::onTextAreaFocused(lv_event_t *e) {
    ScreenManager *self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->focusedTextarea = lv_event_get_target(e);
}

void ScreenManager::reset(){
  this->passwordManager.passwordList.clear();
  this->focusedTextarea = NULL;
  //TODO delete saved password
}
