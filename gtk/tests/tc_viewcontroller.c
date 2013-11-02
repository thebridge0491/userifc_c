#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/util.h"
#include "userifc_c/gtkhello_controller.h"

static int initNum1 = 0, initNum2 = 0;
static float delay_usecs = 3.0e6;
static struct gtkcontroller *uicontroller = NULL;

static void refresh_ui(float microsecs) {
    while (gtk_events_pending())
        gtk_main_iteration_do(FALSE);   // blocking = FALSE
        //gtk_main_iteration();
    usleep(microsecs);
}

static void setupTCase(void) {
    printf("###setup TCase###\n");
    initNum1 = 5;
    char *rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") : "resources";
    //gtk_init(&argc, &argv);
    gtk_init(NULL, NULL);
    uicontroller = gtkcontroller_init("greet.txt", rsrc_path);
    return;
}

static void teardownTCase(void) {
    gtkcontroller_cleanup();
    initNum1 = 0;
    printf("###teardown TCase###\n\n");
    return;
}

static void setupTestA(void) {
    printf("setup Test...\n");
    initNum2 = -1;
    return;
}

static void teardownTestA(void) {
    initNum2 = 0;
    printf("...teardown Test\n");
    return;
}

//START_TEST (test_button1_clicked_cb) {
void test_button1_clicked_cb(void) {
    gtk_button_clicked(g_hash_table_lookup(uicontroller->view1->extra0,
        "button1"));
    refresh_ui(delay_usecs);
    GtkDialog *dialog1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "dialog1");
    GtkTextView *textview1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "textview1");
    ck_assert_msg((gtk_widget_get_visible(GTK_WIDGET(dialog1)) &&
        gtk_widget_get_visible(GTK_WIDGET(textview1))),
        "dialog1 and textview1 not visible");
    GtkEntry *entry1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "entry1");
    ck_assert_str_eq(gtk_entry_get_text(entry1), "");
}
//} END_TEST

//START_TEST (test_dialog1_response_cb) {
void test_dialog1_response_cb(void) {
    GtkDialog *dialog1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "dialog1");
    gtk_widget_show(GTK_WIDGET(dialog1));
    gtk_dialog_response(dialog1, 1);
    refresh_ui(delay_usecs);
    ck_assert_msg(!gtk_widget_get_visible(GTK_WIDGET(dialog1)),
        "dialog1 visible");
}
//} END_TEST

//START_TEST (test_entry1_activate_cb) {
void test_entry1_activate_cb(void) {
    GtkDialog *dialog1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "dialog1");
    GtkEntry *entry1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "entry1");
    gtk_widget_show(GTK_WIDGET(dialog1));
    gtk_entry_set_text(entry1, "John Doe");

    gtk_widget_activate(GTK_WIDGET(entry1));
    refresh_ui(delay_usecs);
    GtkTextView *textview1 = g_hash_table_lookup(uicontroller->view1->extra0,
        "textview1");
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(textview1);
    GtkTextIter iter_start, iter_end;
    gtk_text_buffer_get_iter_at_offset(textbuf, &iter_start, 0);
    gtk_text_buffer_get_iter_at_offset(textbuf, &iter_end, -1);
    /*struct observer* valptr = *(struct observer**)g_list_first(
        g_hash_table_get_values(uicontroller->model1->observers));
    ck_assert_str_eq(gtk_text_buffer_get_slice(textbuf, &iter_start,
        &iter_end, FALSE), valptr->data);*/
    ck_assert_str_eq(gtk_text_buffer_get_slice(textbuf, &iter_start,
        &iter_end, FALSE), uicontroller->view1->data);
    ck_assert_msg(!gtk_widget_get_visible(GTK_WIDGET(dialog1)),
        "dialog1 visible");
}
//} END_TEST

START_TEST (test_callbacks) {
    test_button1_clicked_cb();
    test_dialog1_response_cb();
    test_entry1_activate_cb();
} END_TEST


Suite* tc_viewcontroller_suite(void) {
    Suite *suite = suite_create("tc_viewcontroller");
    TCase *tc_viewcontroller = tcase_create("Tc_viewcontroller");
    tcase_set_tags(tc_viewcontroller, "Tag1");

    tcase_add_unchecked_fixture(tc_viewcontroller, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_viewcontroller, setupTestA, teardownTestA);

    TFun tests[] = {
        /*test_button1_clicked_cb, test_dialog1_response_cb,
        test_entry1_activate_cb*/
        test_callbacks
    };
    int arrLen = sizeof(tests) / sizeof(tests[0]);

    /* Add the tests to the tcase */
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_viewcontroller, tests[i]);
    //tcase_add_test(tc_new, test_method);
    tcase_set_timeout(tc_viewcontroller, 30.0); // increase default timeout: 4.0

    /* Add tcase to suite */
    suite_add_tcase(suite, tc_viewcontroller);
    return suite;
}
