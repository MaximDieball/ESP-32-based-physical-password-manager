#pragma once
#include <SPI.h>
#include <Arduino.h>

class PasswordManager{
public:
  static void setMasterPassword(String password);
  //static type getDecryptedPasswordJson();
  static void encryptAndSavePasswordJson();
  static bool checkMasterPassword(String password);
};
