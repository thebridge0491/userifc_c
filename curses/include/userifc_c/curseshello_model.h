#ifndef CURSESHELLO_MODEL_H
#define CURSESHELLO_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "userifc_c/subject.h"

struct subject* cursessubject_setup(struct subject *subj, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // CURSESHELLO_MODEL_H
