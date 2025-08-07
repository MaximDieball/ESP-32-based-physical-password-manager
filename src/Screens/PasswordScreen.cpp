#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling
extern lv_style_t titel;

static void onBackBtnPressed(lv_event_t *e){
  auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
  self->queueScreen(PWM_SCR);
}

static void onUsernameBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    lv_obj_t* usernameLabel = self->currentScreen.updatableObjects["usernameLabel"];
    const char* usernameLabelText = lv_label_get_text(usernameLabel);
    const char* username = self->selectedPassword.username.c_str();
    if(strcmp(usernameLabelText, username) == 0){
        lv_label_set_text(usernameLabel, "Show Username <o>");
    } else{
        String username = self->shortenString(self->selectedPassword.username, 17); // shorten string
        lv_label_set_text(usernameLabel, username.c_str());
    }
}

static void onPasswordBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    lv_obj_t* passwordLabel = self->currentScreen.updatableObjects["passwordLabel"];
    const char* passwordLabelText = lv_label_get_text(passwordLabel);
    const char* password = self->selectedPassword.password.c_str();
    if(strcmp(passwordLabelText, password) == 0){
        lv_label_set_text(passwordLabel, "Show password <o>");
    } else{
        String password = self->shortenString(self->selectedPassword.password, 17);  // shorten string
        lv_label_set_text(passwordLabel, password.c_str());
    }
}

static void onEditBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->queueScreen(EDIT_PASSWORD_SCREEN);
}

static void onTypeUsernameBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    String username = self->selectedPassword.username;
    self->hidManager.typeString(username);
}

static void onTypePasswordBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    String password = self->selectedPassword.password;
    self->hidManager.typeString(password);
}

static void ontypeBothBtnPressed(lv_event_t *e){
    auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    String password = self->selectedPassword.password;
    String username = self->selectedPassword.username;
    self->hidManager.typeString(username);
    self->hidManager.typeTab();
    self->hidManager.typeString(password);
}



void displayInfo(ScreenManager* self){

    lv_obj_t *usernameLabel = self->currentScreen.updatableObjects["usernameLabel"];
    lv_obj_t *websiteLabel = self->currentScreen.updatableObjects["websiteLabel"];
    lv_obj_t *passwordLabel = self->currentScreen.updatableObjects["passwordLabel"];
    
    String website = self->shortenString(self->selectedPassword.website, 12); // shorten string
    lv_label_set_text(websiteLabel, website.c_str());
    lv_label_set_text(usernameLabel, "Show Username <o>");
    lv_label_set_text(passwordLabel, "Show Password <o>");
}

Screen ScreenManager::createPasswordScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set style
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_add_style(lvScreen, &globalStyle, 0);
    
    // website label
    lv_obj_t *websiteLabel = lv_label_create(lvScreen);
    String website = this->shortenString(this->selectedPassword.website, 12);
    lv_label_set_text(websiteLabel, website.c_str());
    lv_obj_align(websiteLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_add_style(websiteLabel, &titel, 0);
    
    // username label/button
    lv_obj_t *usernameBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(usernameBtn, 175, 40);
    lv_obj_align(usernameBtn, LV_ALIGN_CENTER, -22, -30);
    lv_obj_add_event_cb(usernameBtn, onUsernameBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *usernameLabel = lv_label_create(usernameBtn);
    lv_label_set_text(usernameLabel, "Show Username <o>");
    lv_obj_align(usernameLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(usernameBtn, &globalStyle, 0);

    // Password label/button
    lv_obj_t *passwordBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(passwordBtn, 175, 40);
    lv_obj_align(passwordBtn, LV_ALIGN_CENTER, -22, 20);
    lv_obj_add_event_cb(passwordBtn, onPasswordBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *passwordLabel = lv_label_create(passwordBtn);
    lv_label_set_text(passwordLabel, "Show Password <o>");
    lv_obj_align(passwordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(passwordBtn, &globalStyle, 0);
    
    // type username button
    lv_obj_t *typeUsernameBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(typeUsernameBtn, 40, 40);
    lv_obj_align(typeUsernameBtn, LV_ALIGN_CENTER, 90, -30);
    lv_obj_add_event_cb(typeUsernameBtn, onTypeUsernameBtnPressed, LV_EVENT_CLICKED, this); 
    lv_obj_t *typeUsernameLabel = lv_label_create(typeUsernameBtn);
    lv_label_set_text(typeUsernameLabel, "Type");
    lv_obj_align(typeUsernameLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(typeUsernameBtn, &globalStyle, 0);

    // type password button
    lv_obj_t *typePasswordBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(typePasswordBtn, 40, 40);
    lv_obj_align(typePasswordBtn, LV_ALIGN_CENTER, 90, 20);
    lv_obj_add_event_cb(typePasswordBtn, onTypePasswordBtnPressed, LV_EVENT_CLICKED, this); // todo change function
    lv_obj_t *typePasswordLabel = lv_label_create(typePasswordBtn);
    lv_label_set_text(typePasswordLabel, "Type");
    lv_obj_align(typePasswordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(typePasswordBtn, &globalStyle, 0);
    
    // type both button
    lv_obj_t *typeBothBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(typeBothBtn, 100, 40);
    lv_obj_align(typeBothBtn, LV_ALIGN_CENTER, 0, 70);
    lv_obj_add_event_cb(typeBothBtn, ontypeBothBtnPressed, LV_EVENT_CLICKED, this); // todo change function
    lv_obj_t *typeBothLabel = lv_label_create(typeBothBtn);
    lv_label_set_text(typeBothLabel, "Type Both");
    lv_obj_align(typeBothLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(typeBothBtn, &globalStyle, 0);
    
    // back button
    lv_obj_t *backBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(backBtn, 40, 40);
    lv_obj_align(backBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(backBtn, onBackBtnPressed, LV_EVENT_CLICKED, this);
    lv_obj_t *backBtnLabel = lv_label_create(backBtn);
    lv_label_set_text(backBtnLabel, "<-");
    lv_obj_align(backBtnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(backBtn, &globalStyle, 0);

    // edit button
    lv_obj_t *editBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(editBtn, 40, 40);
    lv_obj_align(editBtn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(editBtn, onEditBtnPressed, LV_EVENT_CLICKED, this); // todo change function
    lv_obj_t *editBtnLabel = lv_label_create(editBtn);
    lv_label_set_text(editBtnLabel, "Edit");
    lv_obj_align(editBtnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(editBtn, &globalStyle, 0);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.mainTextarea = NULL;
    screen.onDisplayFunc = [this]() {
        displayInfo(this);
    };
  
    // enter function / called when pressing enter
    screen.enterFunc = []() {
        Serial.println("test");
    };

    // add labels to updatable object
    screen.updatableObjects["websiteLabel"] = websiteLabel;
    screen.updatableObjects["usernameLabel"] = usernameLabel;
    screen.updatableObjects["passwordLabel"] = passwordLabel;

    return screen;
}