#ifndef DEMO_H
#define DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

void greeting(const char *greet_path, const char *name, char **greet_buf);

char delay_char(float microsecs);

#ifdef __cplusplus
}
#endif

#endif // DEMO_H
