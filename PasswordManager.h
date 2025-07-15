#pragma once
#include <SPI.h>
#include <Arduino.h>

class PasswordManager{
public:
  static void setMasterPassword(String password);
  //static type getDecryptedPasswordJson();
  static void encryptAndSavePasswordJson();
  static bool checkForMasterPassword();
  static bool checkMasterPassword(String password);
private:
  static String generateHashedKey(String password, unsigned char salt[16]); 
};
