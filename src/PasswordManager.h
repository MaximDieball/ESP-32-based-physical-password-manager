#pragma once
#include <SPI.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <lvgl.h>
#include <vector>

struct Password {
    String prevWebsite; // used to find and delete old password entry if website edited 
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
  static String generatePassword();
  void loadPasswordData();
  void savePasswordData();
  std::vector<Password> passwordList;
private:
  static String generateHashedKey(String password, unsigned char salt[16]); 
  static const String passwordChars;
};
