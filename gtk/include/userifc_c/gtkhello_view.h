#ifndef GTKHELLO_VIEW_H
#define GTKHELLO_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

#include "userifc_c/observer.h"

struct observer* gtkobserver_setup(struct observer *obs, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // GTKHELLO_VIEW_H
