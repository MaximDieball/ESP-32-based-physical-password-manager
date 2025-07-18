#pragma once
#include <SPI.h>
#include <Arduino.h>
#include <ArduinoJson.h>


class PasswordManager{
public:
  static void setMasterPassword(String password);
  static void encryptAndSavePasswordJson();
  static bool checkForMasterPassword();
  static bool checkMasterPassword(String password);
  void loadPasswordData();
  StaticJsonDocument<512> getPasswordData();
private:
  StaticJsonDocument<512> passwordData;
  static String generateHashedKey(String password, unsigned char salt[16]); 
};
