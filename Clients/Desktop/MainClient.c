#include <gtk/gtk.h>
#include "Garbage.h"
#include "List.h"
#include "String.h"
#include "Build.h"
#include "Crypto.h"

#define PUBLIC_KEY_FILENAME "cliPublicKey.pem"
#define PRIVATE_KEY_FILENAME "cliPrivateKey.pem"

static void submitVote(GtkWidget* widget, List* groups) {
    String* vote = buildVote(groups);
    String* crypt = encryptFromFile(vote,PUBLIC_KEY_FILENAME);
    printf("%s\n", decryptFromFile(crypt,PRIVATE_KEY_FILENAME)->chars);
}

static void guiQuit() {
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
    gtk_builder_add_from_file(builder, "builder.ui", &error);
    if(!builder) {freegAll(); return -1;}

    List* groups = createList();

    listappend(groups, 'p', "Presidential: ");
    button = gtk_builder_get_object(builder,"presidentialGroup");
    listappend(groups, 'p', gtk_radio_button_get_group(button));

    listappend(groups, 'p', "Gubernatoria: ");
    button = gtk_builder_get_object(builder,"gubernatorialGroup");
    listappend(groups, 'p', gtk_radio_button_get_group(button));

    listappend(groups, 'p', "Mayoral: ");
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