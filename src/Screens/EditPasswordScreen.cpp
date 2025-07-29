#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

static void onBackBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->queueScreen(PASSWORD_SCREEN);
}

static void onEditBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    // add new password but set values to selected password before to edit the password / adding new password will delete old password
    self->newPassword.website = self->selectedPassword.website;
    self->newPassword.prevWebsite = self->selectedPassword.website;
    self->newPassword.username = self->selectedPassword.username;
    self->newPassword.password = self->selectedPassword.password;
    self->queueScreen(ADD_PASSWORD_SCREEN_WEBSITE);
}

static void onDeleteBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    std::vector<Password>* passwordList = &self->passwordManager.passwordList;
    String website = self->selectedPassword.website;
    for(size_t i = 0; i < passwordList->size(); i++){
        if((*passwordList)[i].website == website){
            passwordList->erase(passwordList->begin() + i);
            i--; // skip index because one entry was removed
        }
    }
    self->passwordManager.savePasswordData();
    self->queueScreen(PWM_SCR);
}

Screen ScreenManager::createEditPasswordScreen() {
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
    lv_obj_align(passwordLabel, LV_ALIGN_CENTER, 0, 20);
    lv_obj_add_style(passwordLabel, &globalStyle, 0);
    lv_obj_set_style_border_opa(passwordLabel, LV_OPA_TRANSP, LV_PART_MAIN);

    // edit button
    lv_obj_t *editButton = lv_btn_create(lvScreen);
    lv_obj_set_size(editButton, 100, 40);
    lv_obj_align(editButton, LV_ALIGN_CENTER, 0, 70);
    lv_obj_t *editButtonLabel = lv_label_create(editButton);
    lv_label_set_text(editButtonLabel, "Edit");
    lv_obj_center(editButtonLabel);
    lv_obj_add_event_cb(editButton, onEditBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_add_style(editButton, &globalStyle, 0);
    
    // back button
    lv_obj_t *backBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(backBtn, 40, 40);
    lv_obj_align(backBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(backBtn, onBackBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *backBtnLabel = lv_label_create(backBtn);
    lv_label_set_text(backBtnLabel, "<-");
    lv_obj_align(backBtnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(backBtn, &globalStyle, 0);

    // delete button
    lv_obj_t *deleteBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(deleteBtn, 40, 40);
    lv_obj_align(deleteBtn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(deleteBtn, onDeleteBtnPressed, LV_EVENT_CLICKED, this); // todo change function
    lv_obj_t *deleteBtnLabel = lv_label_create(deleteBtn);
    lv_label_set_text(deleteBtnLabel, "Del");
    lv_obj_align(deleteBtnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(deleteBtn, &globalStyle, 0);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.onDisplayFunc = [this, usernameLabel, passwordLabel, websiteLabel]() {
        lv_label_set_text(websiteLabel, this->selectedPassword.website.c_str());
        String usernameLabelString = "Username: " + this->selectedPassword.username;
        lv_label_set_text(usernameLabel, usernameLabelString.c_str());
        String passwordLabelString = "Password: " + this->selectedPassword.password;
        lv_label_set_text(passwordLabel, passwordLabelString.c_str());
    };
    return screen;
}