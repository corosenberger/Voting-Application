#include "Main.h"

GtkBuilder* builder;
volatile int currWindow;

void* threadNextPhase(ThreadInput* tinput) {
    pthread_t runThread; pthread_create(&runThread, NULL, tinput->toRun, tinput->input);
    setVisibleTab(3);
    String* crypt; pthread_join(runThread,&crypt);

    if(!crypt) {noResponse(NULL); return NULL;}

    String* reply = decryptFromFile(crypt,PRIVATE_KEY_FILENAME);

    printf("%s\n",reply);

    if(!strcmp(reply->chars,FAILURE_RESPONSE)) tinput->failureFunc(reply);
    else if(!strcmp(reply->chars,SUCCESS_RESPONSE)) nextWindow();
    else {printf("%s\n",reply->chars); noResponse(NULL);}

    return NULL;
}

void createThread(void* input, ThreadFunc toRun, ThreadFunc failureFunc) {
    ThreadInput* tinput = callocg(sizeof(ThreadInput));
    tinput->input = input;
    tinput->toRun = toRun;
    tinput->failureFunc = failureFunc;
    pthread_t thread; pthread_create(&thread, NULL, threadNextPhase, tinput);
}

void nextWindow() {
    switch (currWindow) {
    case -1: currWindow = 1; break;
    case 0: currWindow = 2; break;
    case 1: currWindow = 2; break;
    case 2: currWindow = 6; break;
    default: currWindow = 4; break;
    }
    setVisibleTab(currWindow);
}

void* createFailure(void* None) {
    setVisibleTab(currWindow);

    GtkEntry* email = gtk_builder_get_object(builder,"createEmailEntry");
    GtkEntry* pass1 = gtk_builder_get_object(builder,"createPassEntry1");
    GtkEntry* pass2 = gtk_builder_get_object(builder,"createPassEntry2");
    gtk_entry_set_text(email,""); gtk_entry_set_text(pass1,""); gtk_entry_set_text(pass2,"");
    gtk_entry_set_placeholder_text(email,"Please enter a valid email address");
    gtk_entry_set_placeholder_text(pass1,"Must have at least 10 character, 1 number, 1 letter");
    gtk_entry_set_placeholder_text(pass2,"Must have at least 10 character, 1 number, 1 letter");
}

void* createFailurePass(void* None) {
    GtkEntry* pass1 = gtk_builder_get_object(builder,"createPassEntry1");
    GtkEntry* pass2 = gtk_builder_get_object(builder,"createPassEntry2");
    gtk_entry_set_text(pass1,""); gtk_entry_set_text(pass2,"");
    gtk_entry_set_placeholder_text(pass1,"Passwords must match");
    gtk_entry_set_placeholder_text(pass2,"Passwords must match");
}

void* signupFailure(void* None) {
    setVisibleTab(++currWindow);

    GtkEntry* voterid = gtk_builder_get_object(builder,"voteridEntry");
    gtk_entry_set_text(voterid,"");
    gtk_entry_set_placeholder_text(voterid,"Please enter a valid voter id");
}

void* loginFailure(void* None) {
    setVisibleTab(currWindow);

    GtkEntry* email = gtk_builder_get_object(builder,"logEmailEntry");
    GtkEntry* pass = gtk_builder_get_object(builder,"logPassEntry");
    gtk_entry_set_text(email,""); gtk_entry_set_text(pass,"");
    gtk_entry_set_placeholder_text(email,"Please enter a valid email address");
    gtk_entry_set_placeholder_text(pass,"Please enter a valid password");
}

void* noResponse(void* None) {
    printf("rip\n");
    currWindow = 4;
    setVisibleTab(4);
    return NULL;
}

String* threadSubmitVote(List* groups) {
    String* vote = buildVote(groups);
    stringextend(vote,"vote");
    String* crypt = encryptFromFile(vote,PUBLIC_KEY_FILENAME);
    return serverConnect(crypt);
}

void* threadLogout(String* info) {
    threadSignupLogin(info);
    guiQuit();
}

String* threadSignupLogin(String* info) {
    String* crypt = encryptFromFile(info,PUBLIC_KEY_FILENAME);
    return serverConnect(crypt);
}

static void submitVote(GtkWidget* widget, List* groups) {
    createThread(groups,threadSubmitVote,noResponse);    
}

static void resetSelection(GtkWidget* widget, List* groups) {
    for(int i = 1; i < groups->size; i+=2) {
        for(GSList* curr = listget(groups,i); curr != NULL; curr = curr->next) {
            GtkRadioButton* data = (GtkRadioButton*)curr->data;
            gtk_toggle_button_set_active(data,0);
        }
    }

    gtk_toggle_button_set_active(gtk_builder_get_object(builder,"presidentialGroup"),1);
    gtk_toggle_button_set_active(gtk_builder_get_object(builder,"gubernatorialGroup"),1);
    gtk_toggle_button_set_active(gtk_builder_get_object(builder,"mayoralGroup"),1);
}

static void logout() {
    createThread(createStringArr("logout"),threadLogout,noResponse);
}

static void createAccount(GtkWidget* widget) {
    GtkEntry* email = gtk_builder_get_object(builder,"createEmailEntry");
    GtkEntry* pass1 = gtk_builder_get_object(builder,"createPassEntry1");
    GtkEntry* pass2 = gtk_builder_get_object(builder,"createPassEntry2");
    if(strcmp(gtk_entry_get_text(pass1),gtk_entry_get_text(pass2))) {
        createFailurePass(NULL);
    } else {
        String* info = createStringArr(gtk_entry_get_text(email));
        stringappend(info,'\n'); stringextend(info,gtk_entry_get_text(pass1));
        stringextend(info,"\ncreate");
        createThread(info,threadSignupLogin,createFailure);
    }
}

static void signup(GtkWidget* widget) {
    currWindow--;
    GtkEntry* voterid = gtk_builder_get_object(builder,"voteridEntry");
    String* idstr = createStringArr(gtk_entry_get_text(voterid));
    stringextend(idstr,"\nsignup");
    createThread(idstr,threadSignupLogin,signupFailure);
}

static void login(GtkWidget* widget) {
    GtkEntry* email = gtk_builder_get_object(builder,"logEmailEntry");
    GtkEntry* pass = gtk_builder_get_object(builder,"logPassEntry");
    String* info = createStringArr(gtk_entry_get_text(email));
    stringappend(info,'\n'); stringextend(info,gtk_entry_get_text(pass));
    stringextend(info,"\nlogin");
    createThread(info,threadSignupLogin,loginFailure);
}

static void guiQuit() {
    freegAll();
    gtk_main_quit();
}

void setVisibleTab(int pagenumber) {
    GtkNotebook* notebook;
    GtkWidget* page;

    notebook = gtk_builder_get_object(builder,"notebook");
    for(int i = 0; i < NUM_TABS; i++) gtk_widget_hide(gtk_notebook_get_nth_page(notebook, i));
    page = gtk_notebook_get_nth_page(notebook, pagenumber);
    gtk_widget_show(page); gtk_notebook_set_current_page(notebook, pagenumber);
}

int main(int argc, char* argv[]) {
    GObject* window;
    GObject* button;
    GError* error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, GUI_FILE, &error);
    if(!builder) {freegAll(); return -1;}

    currWindow = 0;
    setVisibleTab(currWindow);

    List* groups = createList();

    listappend(groups, 'p', "Presidential:");
    button = gtk_builder_get_object(builder,"presidentialGroup");
    listappend(groups, 'p', gtk_radio_button_get_group(button));

    listappend(groups, 'p', "Gubernatoria:");
    button = gtk_builder_get_object(builder,"gubernatorialGroup");
    listappend(groups, 'p', gtk_radio_button_get_group(button));

    listappend(groups, 'p', "Mayoral:");
    button = gtk_builder_get_object(builder,"mayoralGroup");
    listappend(groups, 'p', gtk_radio_button_get_group(button));




    window = gtk_builder_get_object(builder, "window");
    g_signal_connect(window, "destroy", G_CALLBACK(guiQuit), NULL);

    button = gtk_builder_get_object(builder, "signupButton");
    g_signal_connect(button, "clicked", G_CALLBACK(signup), groups);

    button = gtk_builder_get_object(builder, "loginButton");
    g_signal_connect(button, "clicked", G_CALLBACK(login), groups);

    button = gtk_builder_get_object(builder, "createButton");
    g_signal_connect(button, "clicked", G_CALLBACK(createAccount), groups);

    button = gtk_builder_get_object(builder, "voteButton");
    g_signal_connect(button, "clicked", G_CALLBACK(submitVote), groups);

    button = gtk_builder_get_object(builder, "resetButton");
    g_signal_connect(button, "clicked", G_CALLBACK(resetSelection), groups);

    button = gtk_builder_get_object(builder, "quitButton");
    g_signal_connect(button, "clicked", G_CALLBACK(logout), NULL);

    gtk_main();

    return 0;
}