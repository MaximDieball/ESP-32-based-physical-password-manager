#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling

Screen ScreenManager::createLockScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set background color
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);

    // label
    lv_obj_t *label = lv_label_create(lvScreen);
    lv_label_set_text(label, "Enter Master Password:");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);
    lv_obj_add_style(label, &globalStyle, 0);

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
    lv_obj_t *okButtonLabel = lv_label_create(lvScreen);
    lv_label_set_text(okButtonLabel, "Enter");
    lv_obj_align(okButtonLabel, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(okButton, ScreenManager::enterBtnFunc, LV_EVENT_CLICKED, this);
    lv_obj_add_style(okButtonLabel, &globalStyle, 0);

    // result label
    lv_obj_t *resultLabel = lv_label_create(lvScreen);
    lv_label_set_text(resultLabel, "");  // emtpy label
    lv_obj_align(resultLabel, LV_ALIGN_CENTER, 0, 100);
    lv_obj_add_style(resultLabel, &globalStyle, 0);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.mainTextarea = passwordInput;
    
    // enter function / called when pressing enter
    screen.enterFunc = [this, passwordInput, resultLabel]() {
      String masterPassword = String(lv_textarea_get_text(passwordInput));
      if(passwordManager.checkMasterPassword(masterPassword)){
        // show result
        Serial.println("Correct Master Password");  
        lv_label_set_text(resultLabel, "Correct");
        lv_obj_set_style_text_color(resultLabel, lv_color_hex(0x00FF00), LV_PART_MAIN); // green 

        // load passwords with master password / not using encryption yet so no master password needed
        passwordManager.loadPasswordData();

        // queue home screen
        this->queueScreen(HOME_SCR);

      } else {
        Serial.println("Wrong Master Password");  
        lv_label_set_text(resultLabel, "Wrong");
        lv_obj_set_style_text_color(resultLabel, lv_color_hex(0xFF0000), LV_PART_MAIN); // red

        // clear password input field
        lv_textarea_set_text(this->focusedTextarea, "");
      }
      
    };

    return screen;
}