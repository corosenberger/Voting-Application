#include "Main.h"

pthread_t voteThread;
String* reply;

void reportServerResponse(int num) {
    pthread_join(voteThread, NULL);
    if(reply) printf("%s\n",reply->chars);
    else printf("rip\n");
} 

void* threadSubmitVote(List* groups) {
    signal(SIGUSED,reportServerResponse);

    String* vote = buildVote(groups);
    String* crypt = encryptFromFile(vote,PUBLIC_KEY_FILENAME);
    reply = serverConnect(crypt);

    raise(SIGUSED);
    return NULL;
}

static void submitVote(GtkWidget* widget, List* groups) {
    gtk_widget_set_sensitive(widget, FALSE);
    pthread_create(&voteThread, NULL, threadSubmitVote, groups);
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