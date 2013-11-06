#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>

//static Tcl_Interp *tcl_interp;

int tk_AppInit(Tcl_Interp *interp);

static void str_append(char **dest, const char *src) {
    char *old_dest = *dest, *result;
    
    result = (char*)calloc(strlen(*dest) + strlen(src) + 1, sizeof(char));
    strncpy(result, old_dest, 1 + strlen(old_dest));
    strncat(result, src, strlen(src));
    free(old_dest);
    *dest = result;
}

int tk_AppInit(Tcl_Interp *interp) {
    if (Tcl_Init(interp) != TCL_OK) {
        fprintf(stderr, "Error: Initializing Tcl!\n"); 
        return TCL_ERROR;
    }
    if (Tk_Init(interp) != TCL_OK) {
        fprintf(stderr, "Error: Initializing Tk!\n");
        return TCL_ERROR;
    }
    return TCL_OK;
}

int main(int argc, char **argv) {
    const char *rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") :
        "resources";
    char uiform[128];
    snprintf((char*)uiform, sizeof(uiform) - 1, "%s/%s", rsrc_path,
        2 > argc ? "tcltk/helloForm-tk.tcl" : argv[1]);
    char *defaultArgs[] = {"--", uiform};
    char *tkArgs[2 > argc ? 3 : argc];
    memcpy(tkArgs, defaultArgs, 2 * sizeof(char*));
    
    if (2 > argc)
        tkArgs[2] = "-name=helloForm-tk";
    else
        memcpy(&tkArgs[2], &argv[2], (argc-2) * sizeof(char*));
    
    /*Tcl_Interp *interp = Tcl_CreateInterp();
    tk_AppInit(interp);
    char *str_setArgv = (char*)calloc(1, sizeof(char)), buf[128], *sep = " ";
    int len_tkArgs = sizeof(tkArgs) / sizeof(tkArgs[0]);
    str_append(&str_setArgv, "set argv {");
    for (int i = 0; (len_tkArgs - 2) > i; ++i) {
        snprintf(buf, sizeof(buf) / sizeof(buf[0]),
            "%s%s", (0 == i) ? "" : sep, tkArgs[2+i]);
        str_append(&str_setArgv, buf);
    }
    str_append(&str_setArgv, "}");
    Tcl_Eval(interp, str_setArgv);
    //Tcl_EvalFile(interp, tkArgs[1]);
    snprintf(buf, sizeof(buf) / sizeof(buf[0]), "source %s", tkArgs[1]);
    Tcl_Eval(interp, buf);
    Tcl_Eval(interp, "if {![winfo exists .frame1]} {lib_main}");
    Tk_MainLoop();*/
    Tk_Main(sizeof(tkArgs) / sizeof(tkArgs[0]), tkArgs, tk_AppInit);
    
    exit(EXIT_SUCCESS);
} // end main

// cc hellotcltkdemo.c -o build/main-hellotcltkdemo `pkg-config --cflags --libs tcl tk x11`
// build/main-hellotcltkdemo
