#ifndef _MAIN_CLIENT_H
#define _MAIN_CLIENT_H

#include <gtk/gtk.h>
#include <pthread.h>
#include "Garbage.h"
#include "List.h"
#include "String.h"
#include "Build.h"
#include "Crypto.h"
#include "Client.h"

#if defined(_WIN32)
#define SIGUSED SIGFPE
#elif defined(__unix__)
#define SIGUSED SIGUSR1
#endif

typedef void* (*ThreadFunc) (void*);

typedef struct ThreadInput {
    void* input;
    ThreadFunc toRun;
    ThreadFunc successFunc;
    ThreadFunc failureFunc
} ThreadInput;

#define PUBLIC_KEY_FILENAME "cliPublicKey.pem"
#define PRIVATE_KEY_FILENAME "cliPrivateKey.pem"
#define SUCCESS_RESPONSE "Success"
#define FAILURE_RESPONSE "Failure"

#endif