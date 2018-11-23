#ifndef _CRYPTO_UTILS_H
#define _CRYPTO_UTILS_H

#include <cstring>
#include "log.h"

class CryptoUtils
{
  public:
    const char *IV = "123456";
    int AESEncrypt(const char *text, char *cipher, const char *password, const char *IV);
    int AESDecrypt(const char *cipher, char *text, const char *password, const char *IV);
    int base64Encrypt(const char *text, char *cipher);
    int base64Decrypt(const char *cipher, char *text);
    int AES_base64_encrypt(const char *text, char *cipher, const char *password, const char *IV);
    int AES_base64_decrypt(const char *cipher, char *text, const char *password, const char *IV);
};

#endif