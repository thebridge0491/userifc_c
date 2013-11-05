#ifndef CURSESHELLO_CONTROLLER_H
#define CURSESHELLO_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ncurses.h>
#include <panel.h>

#include "userifc_c/curseshello_model.h"
#include "userifc_c/curseshello_view.h"

struct cursescontroller {struct subject *model1; struct observer *view1;
};

struct cursescontroller* cursescontroller_init(const char* greetfilename,
    const char* rsrc_path);

void cursescontroller_cleanup(void);

int cursescontroller_step_virtualscr(struct cursescontroller *ctrlr);

void cursescontroller_run(struct cursescontroller *ctrlr);

#ifdef __cplusplus
}
#endif

#endif // CURSESHELLO_CONTROLLER_H
