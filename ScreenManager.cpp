#include "ScreenManager.h"
#include <Arduino.h>  
#include "SDManager.h"
#include "PasswordManager.h"

void ScreenManager::createScreens() {
    screenArray[0] = createAddMasterPasswordScreen();
    screenArray[1] = createLockScreen();
}

void ScreenManager::changeScreen(int index) {
    lv_scr_load(screenArray[index].lvScreen);
    this->enterFunc = screenArray[index].enterFunc;
    Serial.println("succesfully changed screen");  
    lv_refr_now(NULL);
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

// Screen to add the Master Password / ADD_MPS_SCR / 0
Screen ScreenManager::createAddMasterPasswordScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set background color
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);

    // label
    lv_obj_t *label = lv_label_create(lvScreen);
    lv_label_set_text(label, "Set your Master Password");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);

    // password input field
    lv_obj_t *passwordInput = lv_textarea_create(lvScreen);

    focusedTextarea = passwordInput;  // focus text area / for physical keyboard

    lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);

    lv_obj_set_width(passwordInput, 200);
    lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, 0);

    lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
    lv_textarea_set_one_line(passwordInput, true);

    // ok/enter button
    lv_obj_t *okButton = lv_btn_create(lvScreen);
    lv_obj_set_size(okButton, 100, 40);
    lv_obj_align(okButton, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *okButtonLabel = lv_label_create(lvScreen);
    lv_label_set_text(okButtonLabel, "Enter");
    lv_obj_align(okButtonLabel, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(okButton, ScreenManager::enterBtnFunc, LV_EVENT_CLICKED, this);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    
    // enter function / called when pressing enter
    screen.enterFunc = [passwordInput]() {
      String masterPassword = String(lv_textarea_get_text(passwordInput));
      PasswordManager::setMasterPassword(masterPassword);
      //SDManager::writeFile("/salt.txt", masterPassword);  old way of saving password
      Serial.print("Setting Master Password: ");
      Serial.println(masterPassword);
    };

    return screen;
}

Screen ScreenManager::createLockScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set background color
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);

    // label
    lv_obj_t *label = lv_label_create(lvScreen);
    lv_label_set_text(label, "Enter Master Password:");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);

    // password input field
    lv_obj_t *passwordInput = lv_textarea_create(lvScreen);

    focusedTextarea = passwordInput;  // focus text area / for physical keyboard

    lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);

    lv_obj_set_width(passwordInput, 200);
    lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, 0);

    lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
    lv_textarea_set_one_line(passwordInput, true);

    // ok/enter button
    lv_obj_t *okButton = lv_btn_create(lvScreen);
    lv_obj_set_size(okButton, 100, 40);
    lv_obj_align(okButton, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *okButtonLabel = lv_label_create(lvScreen);
    lv_label_set_text(okButtonLabel, "Enter");
    lv_obj_align(okButtonLabel, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(okButton, ScreenManager::enterBtnFunc, LV_EVENT_CLICKED, this);

    // result label
    lv_obj_t *resultLabel = lv_label_create(lvScreen);
    lv_label_set_text(resultLabel, "");  // emtpy label
    lv_obj_align(resultLabel, LV_ALIGN_CENTER, 0, 100);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    
    // enter function / called when pressing enter
    screen.enterFunc = [passwordInput, resultLabel]() {
      String masterPassword = String(lv_textarea_get_text(passwordInput));
      if(PasswordManager::checkMasterPassword(masterPassword)){
        Serial.println("Correct Master Password");  
        lv_label_set_text(resultLabel, "Correct");
        lv_obj_set_style_text_color(resultLabel, lv_color_hex(0x00FF00), LV_PART_MAIN); // green
      } else {
        Serial.println("Wrong Master Password");  
        lv_label_set_text(resultLabel, "Wrong");
        lv_obj_set_style_text_color(resultLabel, lv_color_hex(0xFF0000), LV_PART_MAIN); // red
      }
      
    };

    return screen;
}

//  gpt code
void ScreenManager::onTextAreaFocused(lv_event_t *e) {
    ScreenManager *self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->focusedTextarea = lv_event_get_target(e);
}
