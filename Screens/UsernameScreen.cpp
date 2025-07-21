#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling

void ScreenManager::displayUsername(){
    Serial.println("displayUsername");
    lv_obj_t *usernameLabel = this->currentScreen.updatableObjects["usernameLabel"];
    lv_obj_t *websiteLabel = this->currentScreen.updatableObjects["websiteLabel"];
    lv_label_set_text(usernameLabel, this->selectedPassword.username.c_str());
    lv_label_set_text(websiteLabel, this->selectedPassword.website.c_str());
}

Screen ScreenManager::createUsernameScreen() {
    lv_obj_t *lvScreen = lv_obj_create(NULL);
    
    // website label
    lv_obj_t *websiteLabel = lv_label_create(lvScreen);
    lv_label_set_text(websiteLabel, this->selectedPassword.website.c_str());
    lv_obj_align(websiteLabel, LV_ALIGN_CENTER, 0, -40);
    lv_obj_add_style(websiteLabel, &globalStyle, 0);
    
    // username label
    lv_obj_t *usernameLabel = lv_label_create(lvScreen);
    lv_label_set_text(usernameLabel, this->selectedPassword.username.c_str());
    lv_obj_align(usernameLabel, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_style(usernameLabel, &globalStyle, 0);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.mainTextarea = NULL;
    screen.onDisplayFunc = [this]() {
        this->displayUsername();
    };
  
    // enter function / called when pressing enter
    screen.enterFunc = []() {
        Serial.println("test");
    };

    // add labels to updatable object
    screen.updatableObjects["websiteLabel"] = websiteLabel;
    screen.updatableObjects["usernameLabel"] = usernameLabel;

    return screen;
}