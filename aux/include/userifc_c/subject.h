#ifndef SUBJECT_H
#define SUBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>

#include "userifc_c/observer.h"

struct subject {GHashTable *observers; void *extra0; void *extra1;
    void (*destroy_func)(struct subject *subj);
    void* (*notify_func)(struct subject *subj, void *arg);
};

//struct subject* subject_setup(struct subject *subj, void *user_data);

struct subject* construct_subject(void *user_data,
    struct subject* (*setup_func)(struct subject*, void*));

void destruct_subject(struct subject *subj);

void subject_attachObserver(struct subject *subj, struct observer *obs);

void subject_detachObserver(struct subject *subj, struct observer *obs);

void subject_notifyObservers(struct subject *subj, void *arg, GHFunc obsfunc);

#ifdef __cplusplus
}
#endif

#endif // SUBJECT_H
