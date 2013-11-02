#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static void widgets_add(gpointer data, GHashTable *widgetsTbl) {
    g_hash_table_insert(widgetsTbl, g_strdup(gtk_buildable_get_name(
        GTK_BUILDABLE(data))), GTK_WIDGET(data));
}

static void widgets_name_add(gpointer data, const gchar *name,
        GHashTable *widgetsTbl) {
    gtk_buildable_set_name(GTK_BUILDABLE(data), name);
    widgets_add(data, widgetsTbl);
}

struct view {GtkWindow *window1; GHashTable *widgets;
};

struct view *view1;

G_MODULE_EXPORT void window1_destroy_cb(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

G_MODULE_EXPORT void dialog1_destroy_cb(GtkWidget *widget, gpointer data) {
    //gtk_main_quit();
    gtk_widget_destroy((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "window1"));
}

G_MODULE_EXPORT void button1_clicked_cb(GtkWidget *widget, gpointer data) {
    gtk_widget_show((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "textview1"));
    gtk_widget_show_all((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "dialog1"));
    gtk_entry_set_text(GTK_ENTRY(g_hash_table_lookup(view1->widgets,
        "entry1")), "");
}

G_MODULE_EXPORT void dialog1_response_cb(GtkWidget *widget, gpointer data) {
    gtk_widget_activate((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "entry1"));
    gtk_widget_hide((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "dialog1"));
}

G_MODULE_EXPORT void entry1_activate_cb(GtkWidget *widget, gpointer data) {
    char buf[64];
    snprintf(buf, sizeof(buf) - 1, "%s%s.", "Hello, ", gtk_entry_get_text(
        GTK_ENTRY(g_hash_table_lookup(view1->widgets, "entry1"))));
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
        g_hash_table_lookup(view1->widgets, "textview1")));
    gtk_text_buffer_set_text(textbuf, buf, -1);
    gtk_widget_hide((GtkWidget*)g_hash_table_lookup(view1->widgets,
        "dialog1"));
}

struct view* view_init() {
    view1 = (struct view*)malloc(sizeof(struct view));
    if (NULL == view1) {
        perror("malloc view");
        return NULL;
    }
    view1->widgets = g_hash_table_new_full((GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, NULL);
    
    /*GtkWidget *window1, *frame1, *vbox1, *label1, *button1, *textview1,
        *entry1, *dialog1, *content_area;

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    frame1 = gtk_frame_new("frame1");
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label1 = gtk_label_new("label1");
    button1 = gtk_button_new_with_label("button1");
    textview1 = gtk_text_view_new();
    dialog1 = gtk_dialog_new();
    entry1 = gtk_entry_new();
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog1));

    gtk_box_pack_start(GTK_BOX(vbox1), label1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox1), textview1, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(frame1), vbox1);
    gtk_container_add(GTK_CONTAINER(window1), frame1);

    gtk_box_pack_start(GTK_BOX(content_area), entry1, TRUE, TRUE, 0);

    widgets_name_add(window1, "window1", view1->widgets);
    widgets_name_add(dialog1, "dialog1", view1->widgets);
    widgets_name_add(button1, "button1", view1->widgets);
    widgets_name_add(textview1, "textview1", view1->widgets);
    widgets_name_add(entry1, "entry1", view1->widgets);
    view1->window1 = GTK_WINDOW(window1);
    */
    const char *rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") :
        "resources";
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    //GTK_MAJOR_VERSION depends on compile: gtk+-3.0 or gtk+-2.0
    char uiform[128];
    snprintf((char*)uiform, sizeof(uiform) - 1, "%s/%s", rsrc_path,
        (2 == GTK_MAJOR_VERSION) ? "gtk/helloForm-gtk2.glade" :
        "gtk/helloForm-gtk3.glade");

    if (!gtk_builder_add_from_file(builder, uiform, &error)) {
        g_warning("%s", error->message);
        g_free(error);
        exit(EXIT_FAILURE);
    }

    g_slist_foreach(gtk_builder_get_objects(builder), (GFunc)widgets_add,
        view1->widgets);
    g_object_unref(G_OBJECT(builder)); // destroy builder, no longer needed
    view1->window1 = (GtkWindow*)g_hash_table_lookup(view1->widgets,
        "window1");
    GtkDialog *dialog1 = GTK_DIALOG(g_hash_table_lookup(view1->widgets,
        "dialog1"));
    
    g_signal_connect(g_hash_table_lookup(view1->widgets, "window1"),
        "destroy", G_CALLBACK(window1_destroy_cb), NULL);
    g_signal_connect(g_hash_table_lookup(view1->widgets, "dialog1"),
        "destroy", G_CALLBACK(dialog1_destroy_cb), NULL);
    g_signal_connect(g_hash_table_lookup(view1->widgets, "button1"),
        "clicked", G_CALLBACK(button1_clicked_cb), NULL);
    g_signal_connect(g_hash_table_lookup(view1->widgets, "dialog1"),
        "response", G_CALLBACK(dialog1_response_cb), NULL);
    g_signal_connect(g_hash_table_lookup(view1->widgets, "entry1"),
        "activate", G_CALLBACK(entry1_activate_cb), NULL);
    //gtk_builder_connect_signals(builder, NULL);
    //g_object_unref(G_OBJECT(builder)); // destroy builder, no longer needed

    gtk_window_set_title(GTK_WINDOW(view1->window1), "hellogtkdemo");
    gtk_window_set_default_size(GTK_WINDOW(view1->window1), 200, 160);
    gtk_window_set_title(GTK_WINDOW(dialog1), "dialog1");
    gtk_window_set_default_size(GTK_WINDOW(dialog1), 160, 100);
    gtk_widget_show_all(GTK_WIDGET(view1->window1));
    
    return view1;
}

void view_cleanup(void) {
    g_hash_table_remove_all(view1->widgets);
}

int main(int argc, char **argv) {
    //gtk_init(&argc, &argv);
    gtk_init(NULL, NULL);
    
    char pretextBuf[256];
    snprintf(pretextBuf, sizeof(pretextBuf) - 1,
        "(GCC %d.%d) Gtk+ %d.%d GUI\n", __GNUC__, __GNUC_MINOR__,
        GTK_MAJOR_VERSION, GTK_MINOR_VERSION);
    struct view *gui = view_init();
    GtkTextBuffer *txtBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
        g_hash_table_lookup(gui->widgets, "textview1")));
    gtk_text_buffer_set_text(txtBuf, pretextBuf, -1);
    //gtk_widget_show_all(GTK_WIDGET(gui->window1));
    gtk_main();

    view_cleanup();
} // end main


// cc hellogtkdemo.c -o build/main-hellogtkdemo `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`
// build/main-hellogtkdemo
