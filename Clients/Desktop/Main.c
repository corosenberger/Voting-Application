#include "Main.h"

pthread_t voteThread;

void* testFunc(String* reply) {
    if(reply) printf("%s\n",reply->chars);
    else printf("rip\n");
    return NULL;
}

String* threadSubmitVote(List* groups) {
    String* vote = buildVote(groups);
    String* crypt = encryptFromFile(vote,PUBLIC_KEY_FILENAME);
    return serverConnect(crypt);
}

void* threadNextPhase(ThreadInput* tinput) {
    pthread_t runThread; pthread_create(&runThread, NULL, tinput->toRun, tinput->input);
    String* reply; pthread_join(runThread,&reply);

    if(!reply || !strcmp(reply->chars,FAILURE_RESPONSE)) tinput->failureFunc(reply);
    else if(!strcmp(reply->chars,SUCCESS_RESPONSE)) tinput->successFunc(reply);

    return NULL;
}

static void submitVote(GtkWidget* widget, List* groups) {
    gtk_widget_set_sensitive(widget, FALSE);

    ThreadInput* tinput = callocg(sizeof(ThreadInput));
    tinput->input = groups;
    tinput->toRun = threadSubmitVote;
    tinput->successFunc = testFunc;
    tinput->failureFunc = testFunc;

    pthread_create(&voteThread, NULL, threadNextPhase, tinput);
}

static void guiQuit() {
    if(voteThread) pthread_cancel(voteThread);
    freegAll();
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    GtkBuilder* builder;
    GObject* window;
    GObject* button;
    GError* error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "voting.ui", &error);
    if(!builder) {freegAll(); return -1;}

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

    button = gtk_builder_get_object(builder, "voteButton");
    g_signal_connect(button, "clicked", G_CALLBACK(submitVote), groups);

    button = gtk_builder_get_object(builder, "quitButton");
    g_signal_connect(button, "clicked", G_CALLBACK(guiQuit), NULL);

    gtk_main();

    return 0;
}