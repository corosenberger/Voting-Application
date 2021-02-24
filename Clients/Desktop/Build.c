#include "Build.h"

String* buildVote(List* groups) {
    String* vote = createString();
    for(int i = 0; i < groups->size; i++) {
        stringextend(vote,listget(groups,i++));
        for(GSList* curr = listget(groups,i); curr != NULL; curr = curr->next) {
            GtkRadioButton* data = (GtkRadioButton*)curr->data;
            if(gtk_toggle_button_get_active(data)){
                stringextend(vote,gtk_button_get_label(data));
                break;
            }
        }
        stringappend(vote,'\n');
    }
    return vote;
}