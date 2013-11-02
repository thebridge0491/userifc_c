#ifndef GTKHELLO_CONTROLLER_H
#define GTKHELLO_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

#include "userifc_c/gtkhello_model.h"
#include "userifc_c/gtkhello_view.h"

struct gtkcontroller {struct subject *model1; struct observer *view1;
};

G_MODULE_EXPORT void window1_destroy_cb(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void button1_clicked_cb(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void dialog1_destroy_cb(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void dialog1_response_cb(GtkWidget *widget, gpointer data);

G_MODULE_EXPORT void entry1_activate_cb(GtkWidget *widget, gpointer data);

struct gtkcontroller* gtkcontroller_init(const char* greetfilename,
    const char* rsrc_path);

void gtkcontroller_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif // GTKHELLO_CONTROLLER_H
