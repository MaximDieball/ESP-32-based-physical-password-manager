#pragma once
#include <SPI.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <lvgl.h>
#include <vector>

struct Password {
    //lv_obj_t* button = nullptr;
    //std::function<void()> buttonFunc;
    String website;
    String username;
    String password;
};


class PasswordManager{
public:
  static void setMasterPassword(String password);
  static void encryptAndSavePasswordJson();
  static bool checkForMasterPassword();
  static bool checkMasterPassword(String password);
  void loadPasswordData();
  void savePasswordData();
  std::vector<Password> passwordList;
private:
  static String generateHashedKey(String password, unsigned char salt[16]); 
};
