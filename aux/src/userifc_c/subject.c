#include "userifc_c/subject.h"

// extra(s) usage: extra0 -> ???; extra1 -> ???
//struct subject {GHashTable *observers; void *extra0; void *extra1;
//    void destroy_func(struct subject *subj);
//    void* notify_func(struct subject *subj, void *arg);
//};

//struct subject* subject_setup(struct subject *subj, void *user_data) {
//    // ...
//    subj->destroy_func = ???; //void destroy_func(struct subject*);
//    subj->notify_func = ???; //void* notify_func(struct subject*, void*);
//    
//    return subj;
//}

struct subject* construct_subject(void *user_data,
        struct subject* (*setup_func)(struct subject*, void*)) {
    struct subject *subj = malloc(sizeof(struct subject));
    if (NULL == subj) {
        perror("malloc subj");
        return NULL;
    }
    //subj->observers = g_hash_table_new_full((GHashFunc)g_direct_hash,
    //    (GEqualFunc)g_direct_equal, (GDestroyNotify)g_free, NULL);
    subj->observers = g_hash_table_new_full((GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, NULL);
    subj->destroy_func = NULL;
    subj->notify_func = NULL;
    
    if (NULL != setup_func)
        subj = setup_func(subj, user_data);
    
    return subj;
}

void destruct_subject(struct subject *subj) {
    if (NULL != subj->destroy_func)
        subj->destroy_func(subj);
    if (NULL != subj) {
        free(subj);
        subj = NULL;
    }
}

void subject_attachObserver(struct subject *subj, struct observer *obs) {
    //g_hash_table_replace(subj->observers, obs, obs);
    g_hash_table_add(subj->observers, obs);
}

void subject_detachObserver(struct subject *subj, struct observer *obs) {
    g_hash_table_remove(subj->observers, obs);
}

void subject_notifyObservers(struct subject *subj, void *arg, GHFunc obsfunc) {
    void *result = arg;
    if (NULL != subj->notify_func)
        result = subj->notify_func(subj, arg);
    g_hash_table_foreach(subj->observers, (GHFunc)obsfunc, result);
}
