#include "PasswordManager.h"
#include "SDManager.h"

#include <Arduino.h>

#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/base64.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/gcm.h"

#include <ArduinoJson.h>
#include <vector>
#include <esp_system.h>

const String PasswordManager::passwordChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

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

  // compare inputed password(hashed key) and actuall password(hashed key)
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
  unsigned int iterations = 7000;
  
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

  // store key
  memcpy(this->masterKey, key, 32); // save raw key in memory to decrypt password later

  // hash key 
  uint8_t hash[32];
  const mbedtls_md_info_t* md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  if (!md) return "";
  if (mbedtls_md(md, key, 32, hash) != 0) return "";

  // clean up memory / for safety
  memset(key, 0, sizeof(key));
  mbedtls_md_free(&sha_ctx);

  unsigned char b64[64];
  size_t olen = 0;
  if (mbedtls_base64_encode(b64, sizeof(b64), &olen, hash, sizeof(hash)) != 0) return "";
  b64[olen] = '\0'; // add \0 as end marker
  String base64Key = String((char*)b64);
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
      // decrypt password
      passwordStruct.password = this->aesDecrypt(this->masterKey, passwordStruct.password);
      passwordList.push_back(passwordStruct);
    }
  }
  this->passwordList = passwordList;
}

void PasswordManager::savePasswordData(){
  StaticJsonDocument<512> rawPasswordData;
  JsonArray passwords = rawPasswordData.createNestedArray("passwords");
  std::vector<Password> passwordList = this->passwordList;
  int passwordDataSize = passwordList.size();

  for(int i = 0; i < passwordDataSize; i++){
    JsonObject jsonEntry = passwords.createNestedObject();
    jsonEntry["website"] = passwordList[i].website;
    jsonEntry["username"] = passwordList[i].username;
    jsonEntry["password"] = this->aesEncrypt(this->masterKey, passwordList[i].password); // encrypt password
  } 

  SDManager::writeJsonFile("/main.json", rawPasswordData);
}

String PasswordManager::generatePassword(){
  String password = "";
  srand(time(0)); // seed rand
  for(int i = 0; i<3; i++){
    for(int j = 0; j<5; j++){
      int len = passwordChars.length();
      int random_index =  esp_random() % len;
      char random_char = passwordChars.charAt(random_index);
      password += random_char;
    }
    if(i < 2){
      password += "-";
    }
  }
  return password;
}


// encrypting a passed String and returning the encrypted String
// format : IV / TAG / ENCRYPTED STRING
// IV = Initialization Vector / for unique results even if string and key are the same
// TAG = Authentication Tag 
String PasswordManager::aesEncrypt(const uint8_t key[32], const String& plaintext) {
  const size_t IV_LEN  = 12, TAG_LEN = 16;  // standart sizes

  uint8_t iv[IV_LEN];
  esp_fill_random(iv, IV_LEN);  // create/fill Initialization Vector 

  const uint8_t* pt = (const uint8_t*)plaintext.c_str();  // convert plain text to bytes
  size_t pt_len = plaintext.length();

  // reserve bytes for ciphertext buffer (same size as plaintext)
  uint8_t* ct = (uint8_t*)malloc(pt_len);
  if (!ct) return "";

  // setting GCM ctx + set key / standard code / gpt code
  mbedtls_gcm_context gcm; 
  mbedtls_gcm_init(&gcm);
  if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, 256) != 0) {
    mbedtls_gcm_free(&gcm); free(ct); return "";
  }

  // Encrypt
  uint8_t tag[TAG_LEN];
  int rc = mbedtls_gcm_crypt_and_tag(
    &gcm, MBEDTLS_GCM_ENCRYPT,
    pt_len, iv, IV_LEN,
    nullptr, 0,
    pt, ct,
    TAG_LEN, tag
  );

  mbedtls_gcm_free(&gcm); // free memory for safety
  if (rc != 0) { free(ct); return ""; }

  // save result as : IV / TAG / CIPHERTEXT
  size_t result_len = IV_LEN + TAG_LEN + pt_len;
  uint8_t* result = (uint8_t*)malloc(result_len);
  if (!result) { free(ct); return ""; }

  memcpy(result, iv, IV_LEN);
  memcpy(result + IV_LEN, tag, TAG_LEN);
  memcpy(result + IV_LEN + TAG_LEN, ct, pt_len);
  free(ct);

  // base64 encode result
  size_t b64_capacity = ((result_len + 2) / 3) * 4 + 1; // calculating the capacity needed to store the result / gpt code
  unsigned char* b64 = (unsigned char*)malloc(b64_capacity);
  size_t out_len = 0;
  rc = mbedtls_base64_encode(b64, b64_capacity, &out_len, result, result_len);
  free(result);
  b64[out_len] = '\0';  // add \0 as end marker
  String out = String((char*)b64);
  free(b64);

  return out;
}


String PasswordManager::aesDecrypt(const uint8_t key[32], const String& b64) {
  const size_t IV_LEN = 12, TAG_LEN = 16; // standard sizes

  // Base64 decode result / format = IV / TAG / CIPHERTEXT
  size_t b64_len = b64.length();
  size_t buf_capacity = (b64_len * 3) / 4 + 4;     // calculates capacity / gpt code
  uint8_t* buf = (uint8_t*)malloc(buf_capacity);
  if (!buf) return ""; // ERROR
  size_t buf_len = 0;

  // save bytes in buf
  if (mbedtls_base64_decode(buf, buf_capacity, &buf_len, (const unsigned char*)b64.c_str(), b64_len) != 0) {
      free(buf); return ""; // ERROR
  }

  // format = iv / tag / ct
  // initial vector / authentication / cypher txt
  uint8_t* iv  = buf;                         // start of iv
  uint8_t* tag = buf + IV_LEN;                // start of tag
  uint8_t* ct  = buf + IV_LEN + TAG_LEN;      // start of ct
  size_t ct_len = buf_len - IV_LEN - TAG_LEN;

  if (buf_len < IV_LEN + TAG_LEN) { free(buf); return ""; } // check buffer length

  // decrypt and authenticate
  uint8_t* plainText = (uint8_t*)malloc(ct_len + 1); // + 1 for \0
  if (!plainText) { free(buf); return ""; } // ERROR

  // gcm context / set key
  mbedtls_gcm_context gcm; 
  mbedtls_gcm_init(&gcm);
  if (mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, 256) != 0) {
    mbedtls_gcm_free(&gcm); free(plainText); free(buf); return ""; // ERROR
  }

  // authentication
  int rc = mbedtls_gcm_auth_decrypt(
    &gcm, ct_len,
    iv, IV_LEN,
    nullptr, 0,
    tag, TAG_LEN,
    ct, plainText);
  mbedtls_gcm_free(&gcm);
  if (rc != 0) { free(plainText); free(buf); return ""; }  // ERROR / authentication error

  // return string
  plainText[ct_len] = '\0'; // add \0 as end marker
  String plainTextString = String((char*)plainText);

  // free memory
  free(plainText);
  free(buf);
  return plainTextString;
}


