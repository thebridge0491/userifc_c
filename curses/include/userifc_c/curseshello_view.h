#ifndef CURSESHELLO_VIEW_H
#define CURSESHELLO_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include <ncurses.h>
#include <panel.h>

#include "userifc_c/observer.h"

typedef enum {    // usage: CURSESKEY_ENTER
    CURSESKEY_ENTER = (int)'E' - 64,  // Ctrl+E -- Enter (KEY_ENTER)
    CURSESKEY_ESC = (int)'X' - 64,    // Ctrl+X -- Exit  (KEY_EXIT)
    CURSESKEY_RUN = (int)'R' - 64     // Ctrl+R -- Run   (???)
} Keys;

struct observer* cursesobserver_setup(struct observer *obs, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // CURSESHELLO_VIEW_H
