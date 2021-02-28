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

typedef void* (*ThreadFunc) (void*);

typedef struct ThreadInput {
    void* input;
    ThreadFunc toRun;
    ThreadFunc failureFunc;
} ThreadInput;


void* threadNextPhase(ThreadInput* tinput);
void createThread(void* input, ThreadFunc toRun, ThreadFunc failureFunc);
void nextWindow();
void* createFailure(void* None);
void* createFailurePass(void* None);
void* signupFailure(void* None);
void* loginFailure(void* None);
void* noResponse(void* None);
String* threadSubmitVote(List* groups);
void* threadLogout(String* info);
String* threadSignupLogin(String* info);
static void submitVote(GtkWidget* widget, List* groups);
static void resetSelection(GtkWidget* widget, List* groups);
static void logout();
static void createAccount(GtkWidget* widget);
static void signup(GtkWidget* widget);
static void login(GtkWidget* widget);
static void guiQuit();
void setVisibleTab(int pagenumber);
int main(int argc, char* argv[]);

#define GUI_FILE "client.ui"
#define NUM_TABS 7
#define PUBLIC_KEY_FILENAME "cliPublicKey.pem"
#define PRIVATE_KEY_FILENAME "cliPrivateKey.pem"
#define SUCCESS_RESPONSE "Success"
#define FAILURE_RESPONSE "Failure"

#endif