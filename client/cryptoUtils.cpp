#include "cryptoUtils.h"

#define AES_BLOCK_SIZE 64

int CryptoUtils::AESEncrypt(const char *text, char *cipher, const char *password, const char *iv)
{
    char *thisIV = new char[AES_BLOCK_SIZE];
    if (iv) {
        strncpy(thisIV, iv, AES_BLOCK_SIZE);
    } else {
        strncpy(thisIV, this->IV, AES_BLOCK_SIZE);
    }
    strcpy(cipher, text);
    return 0;
}

int CryptoUtils::AESDecrypt(const char *cipher, char *text, const char *password, const char *iv)
{
    char *thisIV = new char[AES_BLOCK_SIZE];
    if (iv) {
        strncpy(thisIV, iv, AES_BLOCK_SIZE);
    } else {
        strncpy(thisIV, this->IV, AES_BLOCK_SIZE);
    }
    strcpy(text, cipher);
    return 0;
}

int CryptoUtils::base64Encrypt(const char *text, char *cipher) {
    strcpy(cipher, text);
    return 0;
}

int CryptoUtils::base64Decrypt(const char *cipher, char *text) {
    strcpy(text, cipher);
    return 0;
}

int CryptoUtils::AES_base64_encrypt(const char *text, char *cipher, const char *password, const char *iv) {
    char * midCipher = new char[strlen(text)];
    int res = 0;
    res = AESEncrypt(text, midCipher, password, iv);
    if (res != 0) {
        return res;
    }
    res = base64Encrypt(midCipher, cipher);
    return res;
}

int CryptoUtils::AES_base64_decrypt(const char *cipher, char *text, const char *password, const char *iv) {
    char * midCipher = new char[strlen(text)];
    int res = 0;
    res = AESDecrypt(cipher, midCipher, password, iv);
    if (res != 0) {
        return res;
    }
    res = base64Decrypt(midCipher, text);
    return res;
}