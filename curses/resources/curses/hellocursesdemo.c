#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <ncurses.h>
#include <panel.h>

typedef enum {    // usage: CURSESKEY_ENTER
    CURSESKEY_ENTER = (int)'E' - 64,  // Ctrl+E -- Enter (KEY_ENTER)
    CURSESKEY_ESC = (int)'X' - 64,    // Ctrl+X -- Exit  (KEY_EXIT)
    CURSESKEY_RUN = (int)'R' - 64     // Ctrl+R -- Run   (???)
} Keys;

static void panels_add(gpointer data, const gchar *name,
        GHashTable *panelsTbl) {
    g_hash_table_insert(panelsTbl, (gpointer)name, data);
}

struct view {GHashTable *panels; WINDOW *stdscr;
};

void view_on_key_unmapped(struct view* view1, char ch) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(view1->panels, "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(view1->panels, "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(view1->panels, "commands");
    
    mvwprintw(panel_window(pan_in), 1, 1, "Error! Un-mapped key: %s. Retrying.",
        unctrl(ch));
    wrefresh(panel_window(pan_in));
    flash();
    napms(2000);
}

void view_on_key_enter(struct view* view1) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(view1->panels, "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(view1->panels, "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(view1->panels, "commands");
    
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
    mvwprintw(panel_window(pan_out), cur_y+1, 1, "Hello, %s.", data);
    noecho();
}

void view_cleanup(struct view* view1) {
    //g_hash_table_remove_all(view1->panels);
    nocbreak();
    keypad(view1->stdscr, 0);
    echo();
    endwin();
}

WINDOW* view_setup(struct view* view1) {
    //view1->stdscr = initscr();
    noecho();
    cbreak();
    keypad(view1->stdscr, 1);
    return view1->stdscr;
}

int view_step_virtualscr(struct view* view1) {
    PANEL *pan_out = (PANEL*)g_hash_table_lookup(view1->panels, "output");
    PANEL *pan_in = (PANEL*)g_hash_table_lookup(view1->panels, "input");
    PANEL *pan_cmds = (PANEL*)g_hash_table_lookup(view1->panels, "commands");
    
    int isRunning = 1;
    werase(panel_window(pan_in));
    box(panel_window(pan_in), '|', '-');
    hide_panel(pan_in);
    char ch = wgetch(panel_window(pan_cmds));
    
    if (CURSESKEY_ENTER == (int)ch) {
        view_on_key_enter(view1);
    } else if (CURSESKEY_ESC == (int)ch) {
        isRunning = 0;
    } else if (CURSESKEY_RUN != (int)ch) {
        view_on_key_unmapped(view1, ch);
    }
    
    wrefresh(panel_window(pan_out));
    wrefresh(panel_window(pan_in));
    wrefresh(panel_window(pan_cmds));
    
    return isRunning;
}

void view_run(struct view* view1) {
    noecho();
    wrefresh(view1->stdscr);
    
    while (view_step_virtualscr(view1)) {
        //update_panels();
        doupdate();
    }
}

struct view* view_init(WINDOW* screen) {
    struct view *view1 = (struct view*)malloc(sizeof(struct view));
    if (NULL == view1) {
        perror("malloc view");
        return NULL;
    }
    view1->panels = g_hash_table_new_full((GHashFunc)g_str_hash,
        (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, NULL);
    view1->stdscr = screen;
    
    view1->stdscr = view_setup(view1);
    //view1->stdscr->clear();
    int orig_hgt, orig_wid;
    getmaxyx(view1->stdscr, orig_hgt, orig_wid);
    
    PANEL *panel_output, *panel_input, *panel_commands;
    panel_output = new_panel(newwin(orig_hgt - 5, orig_wid - 2, 1, 1));
    panel_input = new_panel(newwin(3, orig_wid / 2, 7, 20));
    panel_commands = new_panel(newwin(4, orig_wid - 2, orig_hgt - 5, 1));
    
    panels_add(panel_output, "output", view1->panels);
    panels_add(panel_input, "input", view1->panels);
    panels_add(panel_commands, "commands", view1->panels);
    
    char textBuf[256];
    snprintf(textBuf, sizeof(textBuf) - 1, "'%-32s'", "hellocursesdemo.c");
    wattron(view1->stdscr, A_REVERSE);
    waddstr(view1->stdscr, textBuf);
    wattroff(view1->stdscr, A_REVERSE);
    
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
    wrefresh(view1->stdscr);
    return view1;
}

int main(int argc, char **argv) {
    WINDOW *win = initscr();
    
    char pretextBuf[256];
    snprintf(pretextBuf, sizeof(pretextBuf) - 1,
        "(GCC %d.%d) Ncurses %s TUI\n", __GNUC__, __GNUC_MINOR__, "???");
    struct view *gui = view_init(win);
    wattron(gui->stdscr, A_REVERSE);
    mvwaddstr(gui->stdscr, 1, 1, pretextBuf);
    wattroff(gui->stdscr, A_REVERSE);
    view_run(gui);
    
    view_cleanup(gui);
    
    exit(EXIT_SUCCESS);
} // end main


// cc hellocursesdemo.c -o build/main-hellocursesdemo `pkg-config --cflags --libs glib-2.0 ncurses panel`
// build/main-helloncursesdemo
