#include "Crypto.h"

int cryptoinit;

void initCrypto() {
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    ERR_load_crypto_strings();
    cryptoinit = 1;
}

String* encryptText(String* plain, BIO* file) {
    RSA* rsa = RSA_new(); PEM_read_bio_RSA_PUBKEY(file, &rsa, NULL, NULL);
    int size = RSA_size(rsa);

    char* chars = callocg(size);
    RSA_public_encrypt(plain->size, plain->chars, chars, rsa, RSA_PKCS1_OAEP_PADDING);
    String* crypt = createStringArrLen(chars,size);

    freeg(chars);
    BIO_free(file);
    RSA_free(rsa);
    return crypt;
}

String* decryptText(String* crypt, BIO* file) {
    RSA* rsa = RSA_new(); PEM_read_bio_RSAPrivateKey(file, &rsa, NULL, NULL);
    int size = RSA_size(rsa);

    char* chars = callocg(size);
    RSA_private_decrypt(crypt->size, crypt->chars, chars, rsa, RSA_PKCS1_OAEP_PADDING);
    String* plain = createStringArrLen(chars,size);

    freeg(chars);
    BIO_free(file);
    RSA_free(rsa);
    return plain;
}

String* cryptoFromFile(String* plain, const char* filename, CryptoFunc cryptoFunc) {
    if(!cryptoinit) initCrypto();
    BIO* file = BIO_new_file(filename, "rb");
    return cryptoFunc(plain,file);
}

String* cryptoFromString(String* plain, String* key, CryptoFunc cryptoFunc) {
    if(!cryptoinit) initCrypto();
    BIO* file = BIO_new(BIO_s_mem());
    BIO_puts(file, key->chars);
    return cryptoFunc(plain,file);
}

String* encryptFromFile(String* plain, const char* filename) {return cryptoFromFile(plain,filename,encryptText);}
String* encryptFromString(String* plain, String* key) {return cryptoFromString(plain,key,encryptText);}
String* decryptFromFile(String* plain, const char* filename) {return cryptoFromFile(plain,filename,decryptText);}
String* decryptFromString(String* plain, String* key) {return cryptoFromString(plain,key,decryptText);}