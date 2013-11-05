#include <stdio.h>
#include <stdlib.h>
//#include <ncurses.h>
//#include <panel.h>

#include "userifc_c/curseshello_controller.h"
#include "userifc_c/curseshello_model.h"
#include "userifc_c/curseshello_view.h"

struct cursescontroller *curses_ctrlr = NULL;

static void on_key_unmapped(struct cursescontroller *ctrlr, char ch) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "commands");
    
    mvwprintw(panel_window(pan_in), 1, 1, "Error! Un-mapped key: %s. Retrying.",
        unctrl(ch));
    wrefresh(panel_window(pan_in));
    flash();
    napms(2000);
}

static void on_key_enter(struct cursescontroller *ctrlr) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "commands");
    
    //top_panel(panel_window(pan_in));
    echo();
    char data[256];
    mvwgetstr(panel_window(pan_in), 1, 1, data);
    int cur_y, cur_x, max_y, max_x;
    getyx(panel_window(pan_out), cur_y, cur_x);
    getmaxyx(panel_window(pan_out), max_y, max_x);
    if ((max_y - 3) < cur_y) {
        wclear(panel_window(pan_out));
        box(panel_window(pan_out), '|', '-');
    }
    getyx(panel_window(pan_out), cur_y, cur_x);
    subject_notifyObservers(ctrlr->model1, data, (GHFunc)observer_update);
    noecho();
}

struct cursescontroller* cursescontroller_init(const char* greetfilename,
        const char* rsrc_path) {
    curses_ctrlr = malloc(sizeof(struct cursescontroller));
    if (NULL == curses_ctrlr) {
        perror("malloc cursescontroller");
        return NULL;
    }
    const char greetpath[128];
    snprintf((char*)greetpath, sizeof(greetpath) - 1, "%s/%s", rsrc_path,
        greetfilename);
    WINDOW *win = initscr();
    curses_ctrlr->model1 = construct_subject(greetpath, cursessubject_setup);
    curses_ctrlr->view1 = construct_observer(win, cursesobserver_setup);

    subject_attachObserver(curses_ctrlr->model1, curses_ctrlr->view1);
    //wrefresh(curses_ctrlr->view1->extra1);

    return curses_ctrlr;
}

void cursescontroller_cleanup(void) {
    g_hash_table_remove_all(curses_ctrlr->view1->extra0);
    destruct_subject(curses_ctrlr->model1);
    destruct_observer(curses_ctrlr->view1);
    if (NULL != curses_ctrlr) {
        free(curses_ctrlr);
        curses_ctrlr = NULL;
    }
}

int cursescontroller_step_virtualscr(struct cursescontroller *ctrlr) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(ctrlr->view1->extra0,
        "commands");
    
    int isRunning = 1;
    werase(panel_window(pan_in));
    box(panel_window(pan_in), '|', '-');
    hide_panel(pan_in);
    char ch = wgetch(panel_window(pan_cmds));
    
    if (CURSESKEY_ENTER == (int)ch) {
        on_key_enter(ctrlr);
    } else if (CURSESKEY_ESC == (int)ch) {
        isRunning = 0;
    } else if (CURSESKEY_RUN != (int)ch) {
        on_key_unmapped(ctrlr, ch);
    }
    
    wrefresh(panel_window(pan_out));
    wrefresh(panel_window(pan_in));
    wrefresh(panel_window(pan_cmds));
    
    return isRunning;
}

void cursescontroller_run(struct cursescontroller *ctrlr) {
    noecho();
    wrefresh(ctrlr->view1->extra1);
    
    while (cursescontroller_step_virtualscr(ctrlr)) {
        //update_panels();
        doupdate();
    }
}
