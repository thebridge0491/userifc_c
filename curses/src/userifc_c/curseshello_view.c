#include <stdio.h>
#include <stdlib.h>
//#include <ncurses.h>
//#include <panel.h>

#include "userifc_c/curseshello_view.h"

// extra(s) usage: extra0 -> GHashTable *panels; extra1 -> WINDOW *stdscr
//struct observer {void *data; void *extra0; void *extra1;
//    void destroy_func(struct observer *obs);
//    void update_func(struct observer *obs, void *arg);
//};

static void panels_add(gpointer data, gpointer name, GHashTable *widgetsTbl) {
    g_hash_table_insert(widgetsTbl, name, data);
}

static WINDOW* stdscr_setup(struct observer *obs) {
    //obs->extra1 = initscr();
    noecho();
    cbreak();
    keypad(obs->extra1, 1);
    return obs->extra1;
}

void cursesobserver_destroy(struct observer *obs) {
    //g_hash_table_remove_all(obs->extra0);
    nocbreak();
    keypad(obs->extra1, 0);
    echo();
    endwin();
}

void cursesobserver_update(struct observer *obs, void *arg) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(obs->extra0, "output");
    int cur_y, cur_x;
    getyx(panel_window(pan_out), cur_y, cur_x);
    mvwprintw(panel_window(pan_out), cur_y+1, 1, "%s", arg);
}

struct observer* cursesobserver_setup(struct observer *obs, void *user_data) {
    //obs->extra0 = g_hash_table_new_full((GHashFunc)g_direct_hash,
    //    (GEqualFunc)g_direct_equal, (GDestroyNotify)g_free, NULL);
    obs->extra0 = g_hash_table_new_full((GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal, (GDestroyNotify)NULL, NULL);
    obs->extra1 = user_data;
    
    obs->extra1 = stdscr_setup(obs);
    //obs->extra1->clear();
    int orig_hgt, orig_wid;
    getmaxyx((WINDOW*)obs->extra1, orig_hgt, orig_wid);
    
    PANEL *panel_output, *panel_input, *panel_commands;
    panel_output = new_panel(newwin(orig_hgt - 5, orig_wid - 2, 1, 1));
    panel_input = new_panel(newwin(3, orig_wid / 2, 7, 20));
    panel_commands = new_panel(newwin(4, orig_wid - 2, orig_hgt - 5, 1));
    
    panels_add(panel_output, "output", obs->extra0);
    panels_add(panel_input, "input", obs->extra0);
    panels_add(panel_commands, "commands", obs->extra0);
    
    char textBuf[256];
    snprintf(textBuf, sizeof(textBuf) - 1, "'%-32s'", "curseshello");
    wattron(obs->extra1, A_REVERSE);
    waddstr(obs->extra1, textBuf);
    wattroff(obs->extra1, A_REVERSE);
    
    werase(panel_window(panel_output));
    werase(panel_window(panel_input));
    werase(panel_window(panel_commands));
    box(panel_window(panel_output), '|', '-');
    box(panel_window(panel_input), '|', '-');
    box(panel_window(panel_commands), '|', '-');
    wattron(panel_window(panel_commands), A_STANDOUT);
    mvwaddch(panel_window(panel_commands), 1, 1, CURSESKEY_RUN);
    wattroff(panel_window(panel_commands), A_STANDOUT);
    wprintw(panel_window(panel_commands), "'%-11s'", " Run");

    wattron(panel_window(panel_commands), A_STANDOUT);
    waddch(panel_window(panel_commands), CURSESKEY_ENTER);
    wattroff(panel_window(panel_commands), A_STANDOUT);
    wprintw(panel_window(panel_commands), "'%-11s'", " Enter Name");

    wattron(panel_window(panel_commands), A_STANDOUT);
    mvwaddch(panel_window(panel_commands), 2, 1, CURSESKEY_ESC);
    wattroff(panel_window(panel_commands), A_STANDOUT);
    wprintw(panel_window(panel_commands), "'%-11s'", " Exit");
    wrefresh(obs->extra1);
    
    obs->destroy_func = cursesobserver_destroy;
    obs->update_func = cursesobserver_update;
    
    return obs;
}
