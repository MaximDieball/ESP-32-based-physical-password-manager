// Screen to add the Master Password / ADD_MPS_SCR / 0
#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>


Screen ScreenManager::createAddMasterPasswordScreen() {
  // base screen
  lv_obj_t *lvScreen = lv_obj_create(NULL);

  // set background color
  lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);

  // label
  lv_obj_t *label = lv_label_create(lvScreen);
  lv_label_set_text(label, "Set your Master Password");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);
  
  // password input field
  lv_obj_t *passwordInput = lv_textarea_create(lvScreen);

  lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);

  lv_obj_set_width(passwordInput, 200);
  lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, 0);

  lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
  lv_textarea_set_one_line(passwordInput, true);

  // focus textarea
  focusedTextarea = passwordInput;
  lv_textarea_set_cursor_pos(passwordInput, LV_TEXTAREA_CURSOR_LAST);
  lv_obj_add_state(passwordInput, LV_STATE_FOCUSED);
  
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
  screen.mainTextarea = passwordInput;
  
  // enter function / called when pressing enter
  screen.enterFunc = [passwordInput, this]() {
    String masterPassword = String(lv_textarea_get_text(passwordInput));
    this->passwordManager.setMasterPassword(masterPassword);
    //SDManager::writeFile("/salt.txt", masterPassword);  old way of saving password
    Serial.print("Setting Master Password: ");
    Serial.println(masterPassword);
  };

  return screen;
}