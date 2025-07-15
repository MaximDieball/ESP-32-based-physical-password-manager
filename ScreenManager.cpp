#include "ScreenManager.h"
#include <Arduino.h>  
#include "SDManager.h"

void ScreenManager::createScreens() {
    screenArray[0] = createAddMasterPasswordScreen();
    screenArray[1] = createLockScreen();
}

void ScreenManager::changeScreen(int index) {
    lv_scr_load(screenArray[index].lvScreen);
    this->enterFunc = screenArray[index].enterFunc;
}

// Screen to add the Master Password / ADD_MPS_SCR / 0
Screen ScreenManager::createAddMasterPasswordScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);
    
    // set background color
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    
    // label
    lv_obj_t *label = lv_label_create(lvScreen);
    lv_label_set_text(label, "Set your Master Password");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);

    // password input field
    lv_obj_t *passwordInput = lv_textarea_create(lvScreen);
    
    focusedTextarea = passwordInput;  // focus text area / for physical keyboard

    lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);
    
    lv_obj_set_width(passwordInput, 200);
    lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, 10);

    lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
    lv_textarea_set_one_line(passwordInput, true);
    
    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.enterFunc = [passwordInput]() {
      String masterPassword = String(lv_textarea_get_text(passwordInput));
      SDManager::writeFile("/salt.txt", masterPassword);
      Serial.print("Setting Master Password: ");
      Serial.println(masterPassword);
    };

    return screen;
}

Screen ScreenManager::createLockScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.enterFunc = [&]() {
      Serial.println("enter");
    };

    return screen;
}

//  gpt code
void ScreenManager::onTextAreaFocused(lv_event_t *e) {
    ScreenManager *self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->focusedTextarea = lv_event_get_target(e);
}
