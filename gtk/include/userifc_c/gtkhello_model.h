#ifndef GTKHELLO_MODEL_H
#define GTKHELLO_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "userifc_c/subject.h"

struct subject* gtksubject_setup(struct subject *subj, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // GTKHELLO_MODEL_H
