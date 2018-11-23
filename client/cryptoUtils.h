#ifndef _CRYPTO_UTILS_H
#define _CRYPTO_UTILS_H

#include <cstring>
#include <iostream>
// #include "log.h"

class CryptoUtils
{
  public:
    const std::string IV = "0123456701234567";
    std::string AESEncrypt(std::string text, std::string password, std::string IV);
    std::string AESDecrypt(std::string cipher, std::string password, std::string IV);
    std::string SHA256(std::string text);
};

#endif