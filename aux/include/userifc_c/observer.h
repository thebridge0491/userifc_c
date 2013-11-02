#ifndef OBSERVER_H
#define OBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

struct observer {void *data; void *extra0; void *extra1;
    void (*destroy_func)(struct observer *obs);
    void (*update_func)(struct observer *obs, void *arg);
};

//struct observer* observer_setup(struct observer *obs, void *user_data);

struct observer* construct_observer(void *user_data,
    struct observer* (*setup_func)(struct observer*, void*));

void destruct_observer(struct observer* obs);

void observer_update(struct observer *key, struct observer *val, void *arg);

#ifdef __cplusplus
}
#endif

#endif // OBSERVER_H
