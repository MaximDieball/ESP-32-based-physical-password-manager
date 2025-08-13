#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

Screen ScreenManager::createLockScreen() {
  lv_obj_t *lvScreen = lv_obj_create(NULL);

  // set style
  lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
  //lv_obj_add_style(lvScreen, &globalStyle, 0);

  // device name label
  lv_obj_t *deviceName = lv_label_create(lvScreen);
  lv_label_set_text(deviceName, "Prototype_Name");
  lv_obj_align(deviceName, LV_ALIGN_TOP_MID, 0, 15);
  lv_obj_add_style(deviceName, &titel, 0);

  // password input field
  lv_obj_t *passwordInput = lv_textarea_create(lvScreen);
  lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);
  lv_obj_set_width(passwordInput, 200);
  lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
  lv_textarea_set_one_line(passwordInput, true);
  lv_obj_add_style(passwordInput, &globalStyle, 0);

  // ok/enter button
  lv_obj_t *okButton = lv_btn_create(lvScreen);
  lv_obj_set_size(okButton, 100, 40);
  lv_obj_align(okButton, LV_ALIGN_CENTER, 0, 60);
  lv_obj_t *okButtonLabel = lv_label_create(okButton);
  lv_label_set_text(okButtonLabel, "Unlock");
  lv_obj_center(okButtonLabel);
  lv_obj_add_event_cb(okButton, ScreenManager::enterBtnFunc, LV_EVENT_CLICKED, this); // ScreenManager::enterBtnFunc / function that calls the current enter function
  lv_obj_add_style(okButton, &globalStyle, 0);

  // result label
  lv_obj_t *resultLabel = lv_label_create(lvScreen);
  lv_label_set_text(resultLabel, "");  // emtpy label
  lv_obj_align(resultLabel, LV_ALIGN_CENTER, 0, 100);

  // screen stuct
  Screen screen;
  screen.lvScreen = lvScreen;
  screen.mainTextarea = passwordInput;
  
  // enter function / called when pressing enter
  screen.enterFunc = [this, passwordInput, resultLabel]() {
    String masterPassword = String(lv_textarea_get_text(passwordInput));
    if(passwordManager.checkMasterPassword(masterPassword)){
      Serial.println("Correct Master Password");  
      lv_label_set_text(resultLabel, ""); // make sure to remove "Wrong" 

      // load passwords with master password / not using encryption yet so no master password needed
      passwordManager.loadPasswordData();

      this->queueScreen(HOME_SCR);

    } else {
      Serial.println("Wrong Master Password");  
      lv_label_set_text(resultLabel, "Wrong");
      lv_obj_set_style_text_color(resultLabel, lv_color_hex(0xFF0000), LV_PART_MAIN); // red

      // clear password input field
      lv_textarea_set_text(this->focusedTextarea, "");
    }
  };

  screen.onDisplayFunc = [this]() {
    this->displayPasswords(0);
    lv_textarea_set_text(this->focusedTextarea, "");
  };

  return screen;
}