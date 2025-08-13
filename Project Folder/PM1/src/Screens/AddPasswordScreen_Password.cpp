#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

static void onBackBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->queueScreen(ADD_PASSWORD_SCREEN_USERNAME);
}

static void onGeneratePasswordBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    lv_obj_t* passwordInput = self->currentScreen.updatableObjects["passwordInput"];
    String newPassword = self->passwordManager.generatePassword();
    lv_textarea_set_text(passwordInput, newPassword.c_str());
}


Screen ScreenManager::createAddPasswordScreen_Password() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set style
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_add_style(lvScreen, &globalStyle, 0);   

    // password label
    lv_obj_t *passwordLabel = lv_label_create(lvScreen);
    lv_label_set_text(passwordLabel, "Password");
    lv_obj_align(passwordLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_add_style(passwordLabel, &titel, 0);

    // password input field
    lv_obj_t *passwordInput = lv_textarea_create(lvScreen);
    lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);
    lv_obj_set_size(passwordInput, 175, 40);
    lv_obj_align(passwordInput, LV_ALIGN_CENTER, -22, 0);
    lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
    lv_textarea_set_one_line(passwordInput, true);
    lv_obj_add_style(passwordInput, &globalStyle, 0);

    // generage password button
    lv_obj_t *generatePasswordBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(generatePasswordBtn, 40, 36);
    lv_obj_align(generatePasswordBtn, LV_ALIGN_CENTER, 90, 0);
    lv_obj_add_event_cb(generatePasswordBtn, onGeneratePasswordBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *generatePasswordLabel = lv_label_create(generatePasswordBtn);
    lv_label_set_text(generatePasswordLabel, "Gen");
    lv_obj_align(generatePasswordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(generatePasswordBtn, &globalStyle, 0);

    // ok/enter button
    lv_obj_t *okButton = lv_btn_create(lvScreen);
    lv_obj_set_size(okButton, 100, 40);
    lv_obj_align(okButton, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *okButtonLabel = lv_label_create(okButton);
    lv_label_set_text(okButtonLabel, "OK");
    lv_obj_center(okButtonLabel);
    lv_obj_add_event_cb(okButton, ScreenManager::enterBtnFunc, LV_EVENT_CLICKED, this); // ScreenManager::enterBtnFunc / function that calls the current enter function
    lv_obj_add_style(okButton, &globalStyle, 0);

    // back button
    lv_obj_t *backBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(backBtn, 40, 40);
    lv_obj_align(backBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(backBtn, onBackBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *backBtnLabel = lv_label_create(backBtn);
    lv_label_set_text(backBtnLabel, "<-");
    lv_obj_align(backBtnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(backBtn, &globalStyle, 0);


    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.mainTextarea = passwordInput;

    // enter function / called when pressing enter
    screen.enterFunc = [this, passwordInput]() {
        this->newPassword.password = String(lv_textarea_get_text(passwordInput));
        this->queueScreen(ADD_PASSWORD_SCREEN_CONFIRM);
    };

    screen.onDisplayFunc = [this, passwordInput](){
        // get password from newPassword / used when editing password
        String password = this->newPassword.password;
        lv_textarea_set_text(passwordInput, password.c_str());
    };

    screen.updatableObjects["passwordInput"] = passwordInput;

    return screen;
}