#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

#include "intro_c/util.h"
#include "userifc_c/gtkhello_model.h"

// extra(s) usage: extra0 -> char hellopfx[64]; extra1 -> ???
//struct subject {GHashTable *observers; void *extra0; void *extra1;
//    void destroy_func(struct subject *subj);
//    void* notify_func(struct subject *subj, void *arg);
//};

//void gtksubject_destroy(struct subject *subj) {
//    ;
//}

void* gtksubject_notify(struct subject *subj, void *arg) {
    int buf_len = 64;
    char *buf = malloc((buf_len + 1) * sizeof(char));
    snprintf(buf, buf_len, "%s%s!", subj->extra0, (const char*)arg);
    return buf;
}

struct subject* gtksubject_setup(struct subject *subj, void *user_data) {
    FILE *fIn;
    if (NULL == (fIn = fopen((const char*)user_data, "r"))) {
        perror(user_data);
        return NULL;
    }
    int buf_len = 64;
    subj->extra0 = malloc((buf_len + 1) * sizeof(char));
    //get_line(subj->extra0, sizeof(subj->extra0) - 1, fIn, false);
    get_line(subj->extra0, buf_len, fIn, false);
    fclose(fIn);
    
    //subj->destroy_func = gtksubject_destroy;
    subj->notify_func = gtksubject_notify;
    return subj;
}
