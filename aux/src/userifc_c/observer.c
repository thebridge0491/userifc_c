#include <stdio.h>
#include <stdlib.h>

#include "userifc_c/observer.h"

// extra(s) usage: extra0 -> ???; extra1 -> ???
//struct observer {void *data; void *extra0; void *extra1;
//    void destroy_func(struct observer *obs);
//    void update_func(struct observer *obs, void *arg);
//};

//struct observer* observer_setup(struct observer *obs, void *user_data) {
//    // ...
//    obs->destroy_func = ???; //void destroy_func(struct observer*);
//    obs->update_func = ???; //void update_func(struct observer*, void*);
//    
//    return obs;
//}

struct observer* construct_observer(void *user_data,
        struct observer* (*setup_func)(struct observer*, void*)) {
    struct observer *obs = malloc(sizeof(struct observer));
    if (NULL == obs) {
        perror("malloc obs");
        return NULL;
    }
    obs->data = malloc((128 + 1) * sizeof(char));
    
    obs->destroy_func = NULL;
    obs->update_func = NULL;
    
    if (NULL != setup_func)
        obs = setup_func(obs, user_data);
    
    return obs;
}

void destruct_observer(struct observer *obs) {
    if (NULL != obs->destroy_func)
        obs->destroy_func(obs);
    if (NULL != obs) {
        free(obs);
        obs = NULL;
    }
}

void observer_update(struct observer *key, struct observer *val, void *arg) {
    //memcpy(key->data, arg, 1 + strlen(arg));
    strncpy(key->data, arg, 1 + strlen(arg));
    if (NULL != key->update_func)
        key->update_func(key, arg);
}
