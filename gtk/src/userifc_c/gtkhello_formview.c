#include <stdio.h>
#include <stdlib.h>
//#include <gtk/gtk.h>

#include "userifc_c/gtkhello_view.h"

// extra(s) usage: extra0 -> GHashTable *widgets; extra1 -> GtkWindow *window1
//struct observer {void *data; void *extra0; void *extra1;
//    void destroy_func(struct observer *obs);
//    void update_func(struct observer *obs, void *arg);
//};

static void widgets_add(gpointer data, GHashTable *widgetsTbl) {
    //gtk_buildable_set_name(GTK_BUILDABLE(data), name);
    //g_hash_table_insert(widgetsTbl, name, GTK_WIDGET(data));
    g_hash_table_insert(widgetsTbl, g_strdup(gtk_buildable_get_name(
        GTK_BUILDABLE(data))), GTK_WIDGET(data));
}

//void gtkobserver_destroy(struct observer *obs) {
//    ;
//}

void gtkobserver_update(struct observer *obs, void *arg) {
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
        g_hash_table_lookup(obs->extra0, "textview1")));
    gtk_text_buffer_set_text(textbuf, (const char*)arg, -1);
}

struct observer* gtkobserver_setup(struct observer *obs, void *user_data) {
    //obs->extra0 = g_hash_table_new_full((GHashFunc)g_direct_hash,
    //    (GEqualFunc)g_direct_equal, (GDestroyNotify)g_free, NULL);
    obs->extra0 = g_hash_table_new_full((GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, NULL);

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    //GTK_MAJOR_VERSION depends on compile: gtk+-3.0 or gtk+-2.0
    const char uiform[128];
    snprintf((char*)uiform, sizeof(uiform) - 1, "%s/%s", (const char*)user_data,
        (2 == GTK_MAJOR_VERSION) ? "gtk/helloForm-gtk2.glade" :
        "gtk/helloForm-gtk3.glade");

    if (!gtk_builder_add_from_file(builder, uiform, &error)) {
        g_warning("%s", error->message);
        g_free(error);
        exit(EXIT_FAILURE);
    }

    g_slist_foreach(gtk_builder_get_objects(builder), widgets_add,
        obs->extra0);
    g_object_unref(G_OBJECT(builder)); // destroy builder, no longer needed
    
    obs->extra1 = GTK_WINDOW(g_hash_table_lookup(obs->extra0, "window1"));
    //obs->destroy_func = gtkobserver_destroy;
    obs->update_func = gtkobserver_update;
    return obs;
}
