#include "PasswordManager.h"
#include "SDManager.h"

#include <Arduino.h>

#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/base64.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

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
  const unsigned char *pwd_data = (const unsigned char *)password.c_str(); // convert String to unsigned char*
  unsigned char key[32];  // output
  size_t password_len = password.length();
  size_t salt_len = sizeof(salt);
  unsigned int iterations = 10000;
  
  const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  mbedtls_md_context_t sha_ctx;
  mbedtls_md_init(&sha_ctx);

  if (!md_info) {
    Serial.println("Ffled to get md_info");
    return; 
  }
  
  int setup_error = mbedtls_md_setup(&sha_ctx, md_info, 1); // 1 = HMAC
  if (setup_error != 0) {
    Serial.printf("md_setup failed: -0x%04X\n", -setup_error);
    return;
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
      return;
  }
  
  // store key in mpwHash.txt
  unsigned char key_output[64];
  size_t key_olen;
  mbedtls_base64_encode(key_output, sizeof(key_output), &key_olen, key, sizeof(key));
  String base64Key = String((char *)key_output);
  SDManager::writeFile("/mpwHash.txt", base64Key);
}
