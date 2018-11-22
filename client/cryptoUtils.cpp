#include "cryptoUtils.h"

int CryptoUtils::AESEncrypt(const char *text, char *cipher, const char *password, const char *IV)
{
    strcpy(cipher, text);
    return 0;
}

int CryptoUtils::AESDecrypt(const char *cipher, char *text, const char *password, const char *IV)
{
    strcpy(text, cipher);
    return 0;
}

int CryptoUtils::AESEncrypt(const char *text, char *cipher, const char *password)
{
    return AESEncrypt(text, cipher, password, IV);
}

int CryptoUtils::AESDecrypt(const char *cipher, char *text, const char *password)
{
    return AESDecrypt(cipher, text, password, IV);
}