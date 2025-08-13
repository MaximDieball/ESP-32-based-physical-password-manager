#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

static void onBackBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->queueScreen(ADD_PASSWORD_SCREEN_PASSWORD);
}

static void onSaveBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));

    // save new password / check if website already exists and delete old entry
    std::vector<Password>* passwordList = &self->passwordManager.passwordList;
    String newWebsite = self->newPassword.website;
    String prevWebsite = self->newPassword.prevWebsite; // used to find and delete old password if website edited 

    for(size_t i = 0; i < passwordList->size(); i++){
        if((*passwordList)[i].website == newWebsite || (*passwordList)[i].website == prevWebsite ){
            passwordList->erase(passwordList->begin() + i);
            i--; // skip index because one entry was removed
        }
    }

    passwordList->push_back(self->newPassword);
    self->passwordManager.savePasswordData();

    self->queueScreen(PWM_SCR);

    // clear input fieds
    lv_obj_t *passwordInput = self->screenArray[ADD_PASSWORD_SCREEN_PASSWORD].updatableObjects["passwordInput"];
    lv_obj_t *usernameInput = self->screenArray[ADD_PASSWORD_SCREEN_USERNAME].updatableObjects["usernameInput"];
    lv_obj_t *websiteInput = self->screenArray[ADD_PASSWORD_SCREEN_WEBSITE].updatableObjects["websiteInput"];

    lv_textarea_set_text(passwordInput, "");
    lv_textarea_set_text(usernameInput, "");
    lv_textarea_set_text(websiteInput, "");

    // clear new password cache
    self->newPassword.website = "";
    self->newPassword.username = "";
    self->newPassword.password = "";
}


Screen ScreenManager::createAddPasswordScreen_Confirm() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set style
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_add_style(lvScreen, &globalStyle, 0);

    // website label
    lv_obj_t *websiteLabel = lv_label_create(lvScreen);
    lv_label_set_text(websiteLabel, "Website");
    lv_obj_align(websiteLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_add_style(websiteLabel, &titel, 0);

    // username label
    lv_obj_t *usernameLabel = lv_label_create(lvScreen);
    lv_label_set_text(usernameLabel, "Username");
    lv_obj_align(usernameLabel, LV_ALIGN_CENTER, 0, -30);
    lv_obj_add_style(usernameLabel, &globalStyle, 0);
    lv_obj_set_style_border_opa(usernameLabel, LV_OPA_TRANSP, LV_PART_MAIN);

    // password label
    lv_obj_t *passwordLabel = lv_label_create(lvScreen);
    lv_label_set_text(passwordLabel, "Password");
    lv_obj_align(passwordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(passwordLabel, &globalStyle, 0);
    lv_obj_set_style_border_opa(passwordLabel, LV_OPA_TRANSP, LV_PART_MAIN);

    // save/enter button
    lv_obj_t *saveButton = lv_btn_create(lvScreen);
    lv_obj_set_size(saveButton, 100, 40);
    lv_obj_align(saveButton, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *saveButtonLabel = lv_label_create(saveButton);
    lv_label_set_text(saveButtonLabel, "Save");
    lv_obj_center(saveButtonLabel);
    lv_obj_add_event_cb(saveButton, onSaveBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_add_style(saveButton, &globalStyle, 0);
    

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

    screen.onDisplayFunc = [this, usernameLabel, passwordLabel, websiteLabel]() {
        String website = this->shortenString(this->newPassword.website, 12);
        lv_label_set_text(websiteLabel, website.c_str());   // shorten string

        String usernameLabelString = "Username: " + this->newPassword.username;   
        usernameLabelString = this->shortenString(usernameLabelString, 30); // shorten string
        lv_label_set_text(usernameLabel, usernameLabelString.c_str());

        String passwordLabelString = "Password: " + this->newPassword.password;
        passwordLabelString = this->shortenString(passwordLabelString, 30); // shorten string
        lv_label_set_text(passwordLabel, passwordLabelString.c_str());
    };

    return screen;
}