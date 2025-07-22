#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utilities.h"

extern lv_style_t globalStyle;  // global styling

// selected password / passed to next screen 
Password passwordStruct;
std::vector<Password> passwordList;

void onBackBtnPressed(lv_event_t *e){
  auto self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
  self->queueScreen(HOME_SCR);
}

// saving the password Struct for each button and ScreenManager so both can get passed to the button function in one obj
struct PasswordButtonContext {
  ScreenManager* self;
  Password password;
};
PasswordButtonContext* buttonContexts[5] = { nullptr };

void onPasswordBtnClicked(lv_event_t *e) {
  // get context from user data
  auto ctx = static_cast<PasswordButtonContext*>(lv_event_get_user_data(e));
  Password password = ctx->password;
  auto self = ctx->self;
  // set selected Password and queue username screen
  self->selectedPassword = password;
  self->queueScreen(USERNAME_SCREEN);
}

void ScreenManager::displayPasswords(int i){
  std::vector<Password> passwordData = this->passwordManager.passwordList;
  int passwordDataSize = passwordData.size();

  if (passwordDataSize == 0) {
    Serial.println("ERROR: passwordDataSize == 0");
    return;
  }

  int passwordIndices[5];
  passwordIndices[0] = i-2;
  passwordIndices[1] = i-1;
  passwordIndices[2] = i;
  passwordIndices[3] = i+1;
  passwordIndices[4] = i+2;
  // Wrap indices
  for(int i = 0; i < 5; i++){
    while(passwordIndices[i] > passwordDataSize-1){
      passwordIndices[i] = passwordIndices[i] - passwordDataSize;
    }
    while(passwordIndices[i] < 0){
      passwordIndices[i] = passwordIndices[i] + passwordDataSize;
    }
  }
  
  for(int i = 0; i < 5; i++){
    Password entry = passwordData[passwordIndices[i]];  // get password data as Password Struct
    lv_obj_t *websiteLabel = lv_obj_get_child(passwordButtons[i], 0); // get label as child from button

    // change label
    lv_label_set_text(websiteLabel, entry.website.c_str());
    lv_obj_add_style(websiteLabel, &globalStyle, 0);

    // create context / store the password for each button in array (with this/self so both can get passed to the function as one obj)
    PasswordButtonContext* ctx = new PasswordButtonContext{ this, entry };
    lv_obj_add_event_cb(passwordButtons[i], onPasswordBtnClicked, LV_EVENT_CLICKED, ctx);
    
  }

}

Screen ScreenManager::createPasswordManagerScreen() {
  // base screen / not scrollabel
  lv_obj_t *lvScreen = lv_obj_create(NULL);
  lv_obj_clear_flag(lvScreen, LV_OBJ_FLAG_SCROLLABLE);

  // get password json data
  std::vector<Password> passwordData = this->passwordManager.passwordList;
  int passwordDataSize = passwordData.size();

  // hard coded geometry for each button
  int heights[5]    = {20, 30, 40, 30, 20};
  int y_positions[5] = {-80+10,-45+10,0+10,45+10,80+10};

  // create 5 password button
  for(int i = 0; i < 5; i++){
    lv_obj_t *btn = lv_btn_create(lvScreen);

    // geometry
    int width = 200 - 20 * abs(i - 2);
    int height = heights[i];
    lv_obj_set_size(btn, width, height);

    // set alignment with hardcoded geometry
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, y_positions[i]);

    // website label
    lv_obj_t *websiteLabel = lv_label_create(btn);
    lv_label_set_text(websiteLabel, "NONE");
    lv_obj_center(websiteLabel);
    lv_obj_add_style(websiteLabel, &globalStyle, 0);

    // bulletpoint label
    lv_obj_t *bulletpointLabel = lv_label_create(btn);
    lv_label_set_text(bulletpointLabel, "<>");
    lv_obj_align(bulletpointLabel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_style(bulletpointLabel, &globalStyle, 0);

    // arrow label
    lv_obj_t *arrowLabel = lv_label_create(btn);
    lv_label_set_text(arrowLabel, "->");
    lv_obj_align(arrowLabel, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_style(arrowLabel, &globalStyle, 0);

    this->passwordButtons[i] = btn;
  }

  displayPasswords(this->selectedPasswordIndex);

  // back button
  lv_obj_t *backBtn = lv_btn_create(lvScreen);
  lv_obj_set_size(backBtn, 40, 40);
  lv_obj_align(backBtn, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_obj_add_event_cb(backBtn, onBackBtnPressed, LV_EVENT_CLICKED, this);
  lv_obj_t *backBtnLabel = lv_label_create(backBtn);
  lv_label_set_text(backBtnLabel, "<-");
  lv_obj_align(backBtnLabel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_style(backBtnLabel, &globalStyle, 0);
  lv_obj_add_style(backBtn, &globalStyle, 0);

  // add password button
  lv_obj_t *addPwButton = lv_btn_create(lvScreen);
  lv_obj_set_size(addPwButton, 40, 40);
  lv_obj_align(addPwButton, LV_ALIGN_TOP_RIGHT, -10, 10);
  lv_obj_add_event_cb(addPwButton, onBackBtnPressed, LV_EVENT_CLICKED, this); // TODO 
  lv_obj_t *addPwLabel = lv_label_create(addPwButton);
  lv_label_set_text(addPwLabel, "+");
  lv_obj_align(addPwLabel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_style(addPwLabel, &globalStyle, 0);
  lv_obj_add_style(backBtn, &globalStyle, 0);

  // screen stuct
  Screen screen;
  screen.lvScreen = lvScreen;
  screen.mainTextarea = NULL;
  screen.onDisplayFunc = [this]() {
    this->displayPasswords(0);
  };
  screen.swipeFunc = [this](String direction) {
    int &selectedPasswordIndex = this->selectedPasswordIndex;

    if(direction == "UP"){
      selectedPasswordIndex = selectedPasswordIndex - 1;
      this->displayPasswords(selectedPasswordIndex);
    } else if(direction == "DOWN"){
      selectedPasswordIndex = selectedPasswordIndex + 1;
      this->displayPasswords(selectedPasswordIndex);
    }

  };
  
  // enter function / called when pressing enter
  screen.enterFunc = []() {
    Serial.println("test");
  };

  return screen;
}