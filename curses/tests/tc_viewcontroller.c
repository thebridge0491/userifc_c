#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/util.h"
#include "userifc_c/curseshello_controller.h"

static int initNum1 = 0, initNum2 = 0;
static float delay_usecs = 3.0e6;
static struct cursescontroller *uicontroller = NULL;

static void refresh_ui(struct cursescontroller *ctrlr, float microsecs) {
    if (cursescontroller_step_virtualscr(ctrlr)) {
        //update_panels();
        doupdate();
    }
    usleep(microsecs);
}

static void setupTCase(void) {
    printf("###setup TCase###\n");
    initNum1 = 5;
    char *rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") : "resources";
    uicontroller = cursescontroller_init("greet.txt", rsrc_path);
    return;
}

static void teardownTCase(void) {
    cursescontroller_cleanup();
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

//START_TEST (test_key_enter_cb) {
void test_key_enter_cb(void) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "commands");
    
    ungetch(CURSESKEY_ENTER);
    
    top_panel(pan_in);
    //mvwprintw(panel_window(pan_in), 1, 1, "xxxxxxx%sxxxxxxx",
    //    " Push Enter key ");
    mvwaddstr(panel_window(pan_in), 1, 1, "xxxxxxx Push Enter key xxxxxxx");
    wrefresh(panel_window(pan_in));
    napms(2000);
    
    //refresh_ui(uicontroller, delay_usecs);
    ck_assert_msg(!panel_hidden(pan_in), "input panel not visible on ENTER key");
    const char *input = "John Doe";
    //mvwprintw(panel_window(pan_in), 1, 1, "%s", input);
    mvwaddstr(panel_window(pan_in), 1, 1, input);
    subject_notifyObservers(uicontroller->model1, input,
        (GHFunc)observer_update);
    int ovector[64];
    //pcre *re = compile_pcre(input, PCRE_CASELESS);//("(?i)John Doe", 0);
    pcre *re = compile_pcre(input, 0);//(input, 0);
    /*struct observer* valptr = *(struct observer**)g_list_first(
        g_hash_table_get_values(uicontroller->model1->observers));
    ck_assert_msg(0 < match_pcre(re, ovector, valptr->data),
        "'%s' not in view1->data(%s)", input, uicontroller->view1->data);*/
    ck_assert_msg(0 < match_pcre(re, ovector, uicontroller->view1->data),
        "'%s' not in view1->data(%s)", input, uicontroller->view1->data);
}
//} END_TEST

//START_TEST (test_key_run_cb) {
void test_key_run_cb(void) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "commands");
    
    ungetch(CURSESKEY_RUN);
    refresh_ui(uicontroller, delay_usecs);
    ck_assert_msg(!panel_hidden(pan_out), "output panel not visible on RUN key");
}
//} END_TEST

//START_TEST (test_key_esc_cb) {
void test_key_esc_cb(void) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "commands");
    
    ungetch(CURSESKEY_ESC);
    ck_assert_msg(!cursescontroller_step_virtualscr(uicontroller),
        "step_virtualscr not False on ESC key");
}
//} END_TEST

//START_TEST (test_key_unmapped_cb) {
void test_key_unmapped_cb(void) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(uicontroller->view1->extra0,
        "commands");
    
    ungetch((int)'Z');
    refresh_ui(uicontroller, delay_usecs);
    ck_assert_msg(!panel_hidden(pan_out), "output panel not visible on unmapped key");
}
//} END_TEST

START_TEST (test_callbacks) {
    test_key_enter_cb();
    test_key_run_cb();
    test_key_esc_cb();
    test_key_unmapped_cb();
} END_TEST


Suite* tc_viewcontroller_suite(void) {
    Suite *suite = suite_create("tc_viewcontroller");
    TCase *tc_viewcontroller = tcase_create("Tc_viewcontroller");
    tcase_set_tags(tc_viewcontroller, "Tag1");

    tcase_add_unchecked_fixture(tc_viewcontroller, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_viewcontroller, setupTestA, teardownTestA);

    TFun tests[] = {
        /*test_key_enter_cb, test_key_run_cb, test_key_esc_cb,
        test_key_unmapped_cb*/
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
