#include "PasswordManager.h"
#include "SDManager.h"

#include <Arduino.h>

#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/base64.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include <ArduinoJson.h>
#include <vector>

void PasswordManager::setMasterPassword(String password){
  unsigned char salt[16];

  // setup rng
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;

  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  const char *personalization = "RandomString <3 PM1";

  int rng_error = mbedtls_ctr_drbg_seed( // result should be 0
      &ctr_drbg,                // DRBG context
      mbedtls_entropy_func,     // Entropy source function
      &entropy,                 // Entropy context
      (const unsigned char *)personalization,
      strlen(personalization)
  );
  if(rng_error != 0){
    Serial.println("rng setup failed");
    return;
  }

  // generate salt
  int salt_error = mbedtls_ctr_drbg_random(&ctr_drbg, salt, sizeof(salt)); // salt is stored in unsigned char salt[16] 
  if (salt_error != 0) {
    Serial.println("generating salt failed");
    return;
  }

  // store salt in salt.txt
  unsigned char salt_output[64];
  size_t salt_olen;
  mbedtls_base64_encode(salt_output, sizeof(salt_output), &salt_olen, salt, sizeof(salt));
  String base64Salt = String((char *)salt_output);
  SDManager::writeFile("/salt.txt", base64Salt);

  // derive key from password 
  // and hash key
  String base64Key = generateHashedKey(password, salt);
  SDManager::writeFile("/mpwHash.txt", base64Key);
}

// check if master password is set
bool PasswordManager::checkForMasterPassword(){
  Serial.println("checkForMasterPassword()");
  String masterPasswordHash = SDManager::readFile("/mpwHash.txt");
  Serial.printf("Hashed Password : %s - %s\n", masterPasswordHash.c_str(), (masterPasswordHash != "None") ? "true" : "false");
  return (masterPasswordHash != "None");
}

// compare password to master password
bool PasswordManager::checkMasterPassword(String password){
  // read salt from salt.txt
  unsigned char salt[16];
  size_t salt_len = 0; // length after decoding
  String base64Salt = SDManager::readFile("/salt.txt");

  int decode_error = mbedtls_base64_decode(
    salt, sizeof(salt), // salt is the output 
    &salt_len,  // decoded length output
    (const unsigned char*)base64Salt.c_str(), // base64 input
    base64Salt.length()
  );

  if(decode_error != 0 || salt_len != 16){
    Serial.printf("Salt decode failed:\n", -decode_error);
    return false;
  }

  String inputedPasswordKey = generateHashedKey(password, salt);
  String masterPasswordKey = SDManager::readFile("/mpwHash.txt");
  Serial.printf("input. ", inputedPasswordKey, "master: ", masterPasswordKey);
  if(masterPasswordKey == inputedPasswordKey){
    return true;  
  } else{
    return false;
  }
  
}


String PasswordManager::generateHashedKey(String password, unsigned char salt[16]){
  // derive key from password 
  // and hash key
  const unsigned char *pwd_data = (const unsigned char *)password.c_str(); // convert String to unsigned char*
  unsigned char key[32];  // output
  size_t password_len = password.length();
  size_t salt_len = sizeof(salt);
  unsigned int iterations = 10000;
  
  const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  mbedtls_md_context_t sha_ctx;
  mbedtls_md_init(&sha_ctx);

  if (!md_info) {
    Serial.println("Failed to get md_info");
    return ""; 
  }
  
  int setup_error = mbedtls_md_setup(&sha_ctx, md_info, 1); // 1 = HMAC
  if (setup_error != 0) {
    Serial.printf("md_setup failed:\n", -setup_error);
    return "";
  }

  int hash_error = mbedtls_pkcs5_pbkdf2_hmac(
      &sha_ctx,
      pwd_data, password_len,
      salt, salt_len,
      iterations,
      sizeof(key),
      key
  );
  if(hash_error != 0){
      Serial.print("hash setup failed");
      return "";
  }

  // convert key to base64 String
  unsigned char key_output[64];
  size_t key_olen;
  mbedtls_base64_encode(key_output, sizeof(key_output), &key_olen, key, sizeof(key));
  String base64Key = String((char *)key_output);
  return base64Key;
}

void PasswordManager::loadPasswordData(){
  StaticJsonDocument<512> rawPasswordData = SDManager::readJsonFile("/main.json"); 
  std::vector<Password> passwordList;
  if(!rawPasswordData.isNull() && rawPasswordData.size() > 0){
    JsonArray passwords = rawPasswordData["passwords"].as<JsonArray>();
    int passwordDataSize = passwords.size();
  
    // loop through data and create buttons
    for(int i = 0; i < passwordDataSize; i++){
      JsonObject entry = passwords[i];
      Password passwordStruct;
      passwordStruct.website = entry["website"].as<String>();
      passwordStruct.username = entry["username"].as<String>();
      passwordStruct.password = entry["password"].as<String>();
      passwordList.push_back(passwordStruct);
    }
  }
  this->passwordList = passwordList;
}
