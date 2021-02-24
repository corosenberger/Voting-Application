#ifndef _CRYPTO_CLIENT_H
#define _CRYPTO_CLIENT_H

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include "Garbage.h"
#include "List.h"
#include "String.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef String* (*CryptoFunc) (String*,BIO*);

String* encryptFromFile(String* plain, const char* filename);
String* encryptFromString(String* plain, String* key);
String* decryptFromFile(String* plain, const char* filename);
String* decryptFromString(String* plain, String* key);

#ifdef __cplusplus
}
#endif

#endif