#ifndef _CRYPTO_UTILS_H
#define _CRYPTO_UTILS_H

#include <cstring>

class CryptoUtils
{
  public:
    const char *IV = "123456";
    int AESEncrypt(const char *text, char *cipher, const char *password, const char *IV);
    int AESDecrypt(const char *cipher, char *text, const char *password, const char *IV);
    int AESEncrypt(const char *text, char *cipher, const char *password);
    int AESDecrypt(const char *cipher, char *text, const char *password);
};

#endif