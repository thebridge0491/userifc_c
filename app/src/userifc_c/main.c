/** @mainpage DocComment: 
 * Introduction, basic syntax/features */

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include <glib.h>
#include <log4c.h>

#include <yajl/yajl_tree.h>
#include <json-c/json.h>
#include <yaml.h>

#include "intro_c/util.h"
#include "person.h"
#include "demo.h"
#include "userifc_c/gtkhello_controller.h"

#define OVECCOUNT 30

struct opts_record {char name[32]; char ifc[16];
};

static log4c_category_t *root;

static void run_demo(char *rsrc_path, struct opts_record *opts) {
    time_t time_in = time(NULL);

    char path_buf[128], dateBuf[64];
    snprintf(path_buf, sizeof(path_buf) - 1, "%s/greet.txt", rsrc_path);
    char *greetBuf = NULL, *greet_path = path_buf;

    //regex_t regex;
    //compile_regex(&regex, "^quit$", REG_EXTENDED|REG_NEWLINE|REG_ICASE);
    int ovector[OVECCOUNT];
    pcre *re = compile_pcre("^quit$", PCRE_CASELESS);//("(?i)^quit$", 0);

    //int rc = match_regex(&regex, opts->name);
    int rc = match_pcre(re, ovector, opts->name);

    printf("%s match: %s to %s\n", (0 > rc) ? "Does not" : "Good", opts->name,
        "\"^quit$\"");

    greeting(greet_path, opts->name, &greetBuf);
    strftime(dateBuf, sizeof(dateBuf), "%c %z %Z", localtime(&time_in));
    printf("%s\n%s!\n", dateBuf, greetBuf);

    assert(NULL != re);
    //regfree(&regex);
    pcre_free(re);
}

static void run_demo_gtk(char *rsrc_path, struct opts_record *opts) {
    time_t time_in = time(NULL);

    char dateBuf[64], pretextBuf[256];

    //regex_t regex;
    //compile_regex(&regex, "^quit$", REG_EXTENDED|REG_NEWLINE|REG_ICASE);
    int ovector[OVECCOUNT];
    pcre *re = compile_pcre("^quit$", PCRE_CASELESS);//("(?i)^quit$", 0);

    //int rc = match_regex(&regex, opts->name);
    int rc = match_pcre(re, ovector, opts->name);

    strftime(dateBuf, sizeof(dateBuf), "%c %z %Z", localtime(&time_in));
    snprintf(pretextBuf, sizeof(pretextBuf) - 1,
        "(GCC %d.%d) Gtk+ %d.%d GUI\n%s match: %s to %s\n%s\n",
        __GNUC__, __GNUC_MINOR__, GTK_MAJOR_VERSION, GTK_MINOR_VERSION,
        (0 > rc) ? "Does not" : "Good", opts->name, "\"^quit$\"", dateBuf);

    //gtk_init(&argc, &argv);
    gtk_init(NULL, NULL);
    
    struct gtkcontroller *uicontroller = gtkcontroller_init("greet.txt",
        rsrc_path);
    GtkTextBuffer *txtBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
        g_hash_table_lookup(uicontroller->view1->extra0, "textview1")));
    gtk_text_buffer_set_text(txtBuf, pretextBuf, -1);
    //gtk_widget_show_all(GTK_WIDGET(uicontroller->view1->window1));
    gtk_main();

    gtkcontroller_cleanup();
    
    assert(NULL != re);
    //regfree(&regex);
    pcre_free(re);
}


static void print_usage(const char *str) {
    fprintf(stderr, "Usage: %s [-h][-u NAME][-i IFC]\n", str);
    exit(EXIT_FAILURE);
}

void parse_cmdopts(struct opts_record *opts, int argc, char **argv) {
    int8_t opt_ch;
    const char *opts_str = "u:hi:";
    log4c_category_log(root, LOG4C_PRIORITY_INFO, "parse_cmdopts()");
    //if (2 > argc)
    //    print_usage(argv[0]);
    while (-1 != (opt_ch = getopt(argc, argv, opts_str))) {
        switch (opt_ch) {
            case 'u': strncpy(opts->name, optarg, 1 + strlen(optarg));
                break;
            case 'i': strncpy(opts->ifc, optarg, 1 + strlen(optarg));
                break;
            default: print_usage(argv[0]);
        }
    }
    if (argc > optind)
        fprintf(stderr, "Unexpected argument after options\n");
}

static int chk_inidata(GKeyFile *cfg_ini, const char *buf_ini,
        GKeyFileFlags flags) {
    GError *error = NULL;

    if (!g_key_file_load_from_data(cfg_ini, buf_ini, strlen(buf_ini), flags,
            &error)) {
        g_error("%s", error->message);
        g_error_free(error);
        return -1;
    }
    return 0;
}

/** DocComment: <p>
 * Brief description. <p>
 * @param argc - number of command-line arguments
 * @param argv - array of command-line arguments */
int main(int argc, char **argv) {
    struct opts_record opts;
    FILE *fIn;
    char rc_buf[256],
        *rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") : "resources";
    
    log4c_init();
    // default precedence: ${LOG4C_RCPATH}/log4crc, ${HOME}/.log4crc, ./log4crc
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/log4crc.xml", rsrc_path);
    if (log4c_load(rc_buf))
        fprintf(stderr, "Failed loading log4c config file($RSRC_PATH=%s): %s\n",
            rsrc_path, rc_buf);
    root = log4c_category_get("root");
    
    strncpy(opts.name, "World", 1 + strlen("World"));
    strncpy(opts.ifc, "term", 1 + strlen("term"));
    parse_cmdopts(&opts, argc, argv);
    
    /*char buf_ini[1024], *row_ini[3] = {"????\n", "???", "???"};
    GKeyFile *cfg_ini = g_key_file_new();
    GError *error = NULL;
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.conf", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
        fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
            rsrc_path, rc_buf);
    else {
        fread(buf_ini, sizeof(buf_ini[0]), sizeof(buf_ini), fIn);
        
        if (chk_inidata(cfg_ini, buf_ini, G_KEY_FILE_KEEP_COMMENTS)) {
            g_key_file_free(cfg_ini);
        } else {
            row_ini[0] = inicfg_to_str(cfg_ini);
            row_ini[1] = g_key_file_get_string(cfg_ini, "default", "domain", 
                &error);
            row_ini[2] = g_key_file_get_string(cfg_ini, "user1", "name",
                &error);
        }
    }*/
    
    char buf_json[1024], *row_json[3] = {"????\n", "???", "???"};
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.json", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
        fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
            rsrc_path, rc_buf);
    else {
        fread(buf_json, sizeof(buf_json[0]), sizeof(buf_json), fIn);
        
        if (1) {
            json_object *jsonobj1 = NULL, *domainobj = NULL, *user1obj = NULL,
                *user1Nameobj = NULL;
            if (NULL == (jsonobj1 = json_tokener_parse(buf_json)))
                fprintf(stderr, "Failure: JSON object is NULL\n");
            else
                row_json[0] = buf_json;
            if (!json_object_object_get_ex(jsonobj1, "domain", &domainobj))
                fprintf(stderr, "Failure: path domain is missing\n");
            else
                row_json[1] = (char*)json_object_get_string(domainobj);
            if (!json_object_object_get_ex(jsonobj1, "user1", &user1obj))
                fprintf(stderr, "Failure: path user1:name is missing\n");
            else {
                json_object_object_get_ex(user1obj, "name", &user1Nameobj);
                row_json[2] = (char*)json_object_get_string(user1Nameobj);
            }
        } else {
            char errbuf[512];
            yajl_val jsonnode = NULL, v_domain = NULL, v_user1Name = NULL;
            const char *path0[] = {"domain", NULL},
                *path1[] = {"user1", "name", NULL};
                
            if (NULL == (jsonnode = yajl_tree_parse(buf_json, errbuf, 
                    sizeof(errbuf))))
                fprintf(stderr, "Failure: JSON node is NULL\n");
            else 
                row_json[0] = buf_json;
            if (NULL == (v_domain = yajl_tree_get(jsonnode, path0,
                    yajl_t_string)))
                fprintf(stderr, "Failure: path domain is missing\n");
            else
                row_json[1] = YAJL_GET_STRING(v_domain);
            if (NULL == (v_user1Name = yajl_tree_get(jsonnode, path1, 
                    yajl_t_string)))
                fprintf(stderr, "Failure: path user1:name is missing\n");
            else
                row_json[2] = YAJL_GET_STRING(v_user1Name);
            yajl_tree_free(jsonnode);
        }
    }
    
    /*char buf_yaml[1024], *row_yaml[3] = {"????\n", "???", "???"};
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.yaml", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
        fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
            rsrc_path, rc_buf);
    else {
        fread(buf_yaml, sizeof(buf_yaml[0]), sizeof(buf_yaml), fIn);
        yaml_parser_t yamlparser;
        yaml_token_t token_yaml;
        if (!yaml_parser_initialize(&yamlparser))
            fprintf(stderr, "Failure: Yaml parser initialization failed\n");
        else {
            yaml_parser_set_input_string(&yamlparser, buf_yaml,
                sizeof(buf_yaml) / sizeof(buf_yaml[0]));
            char *name_block = "default", *tk, *keyX, *valX, **data_ptr;
            char buf_tmp[256];
            int is_value = 0;
            GData *datalist = NULL;
            g_datalist_init(&datalist);
            
            do {
                yaml_parser_scan(&yamlparser, &token_yaml);
                
                switch(token_yaml.type) {
                    case YAML_KEY_TOKEN:
                        is_value = 0;
                    case YAML_VALUE_TOKEN:
                        is_value = 1;
                    case YAML_SCALAR_TOKEN:
                        tk = token_yaml.data.scalar.value;
                        if (0 == is_value) {
                            keyX = strdup(tk);
                        } else {
                            snprintf(buf_tmp, sizeof(buf_tmp), "%s:%s",
                                name_block, keyX);
                            g_datalist_set_data(datalist, strdup(buf_tmp),
                                strdup(tk));
                        }
                    case YAML_BLOCK_SEQUENCE_START_TOKEN:
                        tk = token_yaml.data.scalar.value;
                        name_block = strdup(tk);
                    default:
                        break;
                }
                if (NULL == token_yaml.type || YAML_STREAM_END_TOKEN != token_yaml.type)
                    yaml_token_delete(&token_yaml);
            } while (NULL != token_yaml.type && YAML_STREAM_END_TOKEN != token_yaml.type);
            yaml_token_delete(&token_yaml);
            
            row_yaml[0] = buf_yaml;
            row_yaml[1] = g_datalist_get_data(datalist, "default:domain");
            row_yaml[2] = g_datalist_get_data(datalist, "user1:name");
            
            yaml_parser_delete(&yamlparser);
        }
    }*/
    
    const char *arr_rows[][3] = { /*{row_ini[0], row_ini[1], row_ini[2]},*/
        {row_json[0], row_json[1], row_json[2]}/*,
        {row_yaml[0], row_yaml[1], row_yaml[2]}*/ };
    int len_arr_rows = sizeof(arr_rows) / sizeof(arr_rows[0]);
    
    //gchar *res_str = g_key_file_get_start_group(cfg_ini);
    //fprintf(stderr, "GKeyFile config start group: %s\n", res_str);
    //g_free(res_str);
    for (int i = 0; len_arr_rows > i; ++i) {
        printf("config: %s", arr_rows[i][0]);
        printf("domain: %s\n", arr_rows[i][1]);
        printf("user1Name: %s\n\n", arr_rows[i][2]);
    }
    
    /*if (NULL != error)
        g_error_free(error);
    g_key_file_free(cfg_ini);*/
    
    if (0 == strcmp(opts.ifc, "gtk"))
        run_demo_gtk(rsrc_path, &opts);
    else if (0 == strcmp(opts.ifc, "term"))
        run_demo(rsrc_path, &opts);
    else
        run_demo(rsrc_path, &opts);
    
    log4c_category_log(root, LOG4C_PRIORITY_DEBUG, "exiting main()");
    log4c_fini();
    exit(EXIT_SUCCESS);
} // end main

