#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

static void onBackBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->queueScreen(ADD_PASSWORD_SCREEN_WEBSITE);
}

Screen ScreenManager::createAddPasswordScreen_Username() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set style
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_add_style(lvScreen, &globalStyle, 0);

    // username label
    lv_obj_t *usernameLabel = lv_label_create(lvScreen);
    lv_label_set_text(usernameLabel, "Username");
    lv_obj_align(usernameLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_add_style(usernameLabel, &titel, 0);

    // website input field
    lv_obj_t *usernameInput = lv_textarea_create(lvScreen);
    lv_obj_add_event_cb(usernameInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);
    lv_obj_set_width(usernameInput, 200);
    lv_obj_align(usernameInput, LV_ALIGN_CENTER, 0, 0);
    lv_textarea_set_placeholder_text(usernameInput, "Enter Username");
    lv_textarea_set_one_line(usernameInput, true);
    lv_obj_add_style(usernameInput, &globalStyle, 0);

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
    screen.mainTextarea = usernameInput;

    // enter function / called when pressing enter
    screen.enterFunc = [this, usernameInput]() {
        this->newPassword.username = String(lv_textarea_get_text(usernameInput));
        this->queueScreen(ADD_PASSWORD_SCREEN_PASSWORD);
    };

    screen.onDisplayFunc = [this, usernameInput](){
        // get username from newPassword / used when editing password
        String username = this->newPassword.username;
        lv_textarea_set_text(usernameInput, username.c_str());
    };

    screen.updatableObjects["usernameInput"] = usernameInput;

    return screen;
}