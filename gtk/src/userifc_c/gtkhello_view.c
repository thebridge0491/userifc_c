#include <stdio.h>
#include <stdlib.h>
//#include <gtk/gtk.h>

#include "userifc_c/gtkhello_view.h"

// extra(s) usage: extra0 -> GHashTable *widgets; extra1 -> GtkWindow *window1
//struct observer {void *data; void *extra0; void *extra1;
//    void destroy_func(struct observer *obs);
//    void update_func(struct observer *obs, void *arg);
//};

static void widgets_add(gpointer data, gpointer name, GHashTable *widgetsTbl) {
    gtk_buildable_set_name(GTK_BUILDABLE(data), name);
    g_hash_table_insert(widgetsTbl, name, GTK_WIDGET(data));
    //g_hash_table_insert(widgetsTbl, g_strdup(gtk_buildable_get_name(
    //    GTK_BUILDABLE(data))), GTK_WIDGET(data));
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

    GtkWidget *window1, *frame1, *vBox1, *label1, *button1, *textview1, *entry1,
        *dialog1, *content_area;

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    frame1 = gtk_frame_new("frame1");
    vBox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label1 = gtk_label_new("label1");
    button1 = gtk_button_new_with_label("button1");
    textview1 = gtk_text_view_new();
    dialog1 = gtk_dialog_new();
    entry1 = gtk_entry_new();
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog1));

    gtk_box_pack_start(GTK_BOX(vBox1), label1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vBox1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vBox1), textview1, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(frame1), vBox1);
    gtk_container_add(GTK_CONTAINER(window1), frame1);

    gtk_box_pack_start(GTK_BOX(content_area), entry1, TRUE, TRUE, 0);

    widgets_add(window1, "window1", obs->extra0);
    widgets_add(dialog1, "dialog1", obs->extra0);
    widgets_add(button1, "button1", obs->extra0);
    widgets_add(textview1, "textview1", obs->extra0);
    widgets_add(entry1, "entry1", obs->extra0);
    
    obs->extra1 = GTK_WINDOW(g_hash_table_lookup(obs->extra0, "window1"));
    //obs->destroy_func = gtkobserver_destroy;
    obs->update_func = gtkobserver_update;
    
    return obs;
}
