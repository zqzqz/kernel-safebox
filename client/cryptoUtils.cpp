#include "cryptoUtils.h"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

std::string CryptoUtils::AESEncrypt(std::string text, std::string key, std::string iv)
{
    std::string cipher;
    std::string cIV;
    if (iv.length() == 0)
    {
        cIV = IV;
    }
    else
    {
        cIV = iv;
    }

    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte *)key.c_str(), key.length(), (byte *)cIV.c_str());
    CryptoPP::StringSource encryptor(text, true,
        new CryptoPP::StreamTransformationFilter(encryption,
            new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(cipher),
                    false // do not append a newline
    )));
    return cipher;
}

std::string CryptoUtils::AESDecrypt(std::string cipher, std::string key, std::string iv)
{

    std::string text;
        std::string cIV;
    if (iv.length() == 0)
    {
        cIV = IV;
    }
    else
    {
        cIV = iv;
    }

    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte *)key.c_str(), key.length(), (byte *)cIV.c_str());

    CryptoPP::StringSource decryptor(cipher, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StreamTransformationFilter(decryption,
                new CryptoPP::StringSink(text)
    )));
    return text;
}

std::string CryptoUtils::SHA256(std::string text)
{
    CryptoPP::SHA256 hash;
    byte digest[CryptoPP::SHA256::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte *)text.c_str(), text.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return output;
}