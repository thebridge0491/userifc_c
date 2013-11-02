#include <stdio.h>
#include <stdlib.h>
//#include <gtk/gtk.h>

#include "userifc_c/gtkhello_controller.h"
#include "userifc_c/gtkhello_model.h"
#include "userifc_c/gtkhello_view.h"

struct gtkcontroller *gtk_ctrlr = NULL;

G_MODULE_EXPORT void window1_destroy_cb(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

G_MODULE_EXPORT void button1_clicked_cb(GtkWidget *widget, gpointer data) {
    gtk_widget_show(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "textview1"));
    gtk_widget_show_all(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "dialog1"));
    gtk_entry_set_text(GTK_ENTRY(g_hash_table_lookup(
        gtk_ctrlr->view1->extra0, "entry1")), "");
}

G_MODULE_EXPORT void dialog1_destroy_cb(GtkWidget *widget, gpointer data) {
    //gtk_main_quit();
    gtk_widget_destroy(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "window1"));
}

G_MODULE_EXPORT void dialog1_response_cb(GtkWidget *widget, gpointer data) {
    gtk_widget_activate(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "entry1"));
    gtk_widget_hide(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "dialog1"));
}

G_MODULE_EXPORT void entry1_activate_cb(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "dialog1"));
    subject_notifyObservers(gtk_ctrlr->model1, gtk_entry_get_text(
        GTK_ENTRY(g_hash_table_lookup(gtk_ctrlr->view1->extra0, "entry1"))),
        (GHFunc)observer_update);
}

struct gtkcontroller* gtkcontroller_init(const char* greetfilename,
        const char* rsrc_path) {
    gtk_ctrlr = malloc(sizeof(struct gtkcontroller));
    if (NULL == gtk_ctrlr) {
        perror("malloc gtkcontroller");
        return NULL;
    }
    const char greetpath[128];
    snprintf((char*)greetpath, sizeof(greetpath) - 1, "%s/%s", rsrc_path,
        greetfilename);
    gtk_ctrlr->model1 = construct_subject(greetpath, gtksubject_setup);
    gtk_ctrlr->view1 = construct_observer(rsrc_path, gtkobserver_setup);

    g_signal_connect(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "window1"), "destroy", G_CALLBACK(window1_destroy_cb), NULL);
    g_signal_connect(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "dialog1"), "destroy", G_CALLBACK(dialog1_destroy_cb), NULL);
    g_signal_connect(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "button1"), "clicked", G_CALLBACK(button1_clicked_cb), NULL);
    g_signal_connect(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "dialog1"), "response", G_CALLBACK(dialog1_response_cb), NULL);
    g_signal_connect(g_hash_table_lookup(gtk_ctrlr->view1->extra0,
        "entry1"), "activate", G_CALLBACK(entry1_activate_cb), NULL);
    //gtk_builder_connect_signals(gtk_ctrlr->view1->builder, NULL);
    //g_object_unref(G_OBJECT(gtk_ctrlr->view1->builder)); // destroy builder, no longer needed

    subject_attachObserver(gtk_ctrlr->model1, gtk_ctrlr->view1);

    gtk_window_set_title(GTK_WINDOW(gtk_ctrlr->view1->extra1), "gtkhello");
    gtk_window_set_default_size(GTK_WINDOW(gtk_ctrlr->view1->extra1), 200,
        160);
    GtkDialog *dialog1 = GTK_DIALOG(g_hash_table_lookup(
        gtk_ctrlr->view1->extra0, "dialog1"));
    gtk_window_set_title(GTK_WINDOW(dialog1), "dialog1");
    gtk_window_set_default_size(GTK_WINDOW(dialog1), 160, 100);
    gtk_widget_show_all(GTK_WIDGET(gtk_ctrlr->view1->extra1));
    return gtk_ctrlr;
}

void gtkcontroller_cleanup(void) {
    g_hash_table_remove_all(gtk_ctrlr->view1->extra0);
    destruct_subject(gtk_ctrlr->model1);
    destruct_observer(gtk_ctrlr->view1);
    if (NULL != gtk_ctrlr) {
        free(gtk_ctrlr);
        gtk_ctrlr = NULL;
    }
}
