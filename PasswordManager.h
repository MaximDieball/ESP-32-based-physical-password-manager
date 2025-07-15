#pragma once
#include <SPI.h>

class PasswordManager{
public:
  void setMasterPassword(String password);
  type getDecryptedPasswordJson();
  void encryptAndSavePasswordJson();
  bool checkMasterPassword(String password);
}
