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
  void setMasterPassword(String password);;
  bool checkForMasterPassword();
  bool checkMasterPassword(String password);
  String generatePassword();
  void loadPasswordData();
  void savePasswordData();
  std::vector<Password> passwordList;
private:
  uint8_t masterKey[32];
  String generateHashedKey(String password, unsigned char salt[16]); 
  static const String passwordChars;
  String aesDecrypt(const uint8_t key[32], const String& b64);
  String aesEncrypt(const uint8_t key[32], const String& plaintext);
};
