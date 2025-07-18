#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../utilities.h"

// selected password / passed to next screen 
Password passwordStruct;

Screen ScreenManager::createPasswordManagerScreen() {
  // base screen / not scrollabel
  lv_obj_t *lvScreen = lv_obj_create(NULL);
  lv_obj_clear_flag(lvScreen, LV_OBJ_FLAG_SCROLLABLE);



  // get password json data
  StaticJsonDocument<512> passwordData = passwordManager.getPasswordData();
  JsonArray passwords = passwordData["passwords"].as<JsonArray>();
  // loop through data and create buttons
  for(JsonObject entry : passwords){
    Password passwordStruct;
    passwordStruct.website = entry["website"].as<String>();
    passwordStruct.username = entry["username"].as<String>();
    passwordStruct.password = entry["password"].as<String>();

  }
  // screen stuct
  Screen screen;
  screen.lvScreen = lvScreen;
  screen.mainTextarea = NULL;
  
  // enter function / called when pressing enter
  screen.enterFunc = []() {
    Serial.println("test");
  };

  return screen;
}