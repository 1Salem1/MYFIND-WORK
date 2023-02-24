#include "include/myfind.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <sys/stat.h>
#include "include/is_file.h"
#include "include/is_dir.h"
#include <stddef.h>

typedef enum {
    MODE_EXACT,
    MODE_ANY,
    MODE_NONE
} perm_mode_t;

void myfind(char **paths, opt *options)
{
    struct stat header;
    int i = 0;

    if (paths[i] == NULL)
    {
        paths[i] = ".";
        paths[i + 1] = NULL;
    }
    while (paths[i] != NULL)
    {
        int status = stat(paths[i], &header);
        if (status == -1){

            // output :  myfind: /path/to/missing/file: No such file or directory 
            perror("myfind");
            fprintf(stderr, ": %s: %s\n", paths[i], strerror(errno));
        }
        else 
        {
            is_file(paths[i], options, &header);
            if (S_ISDIR(header.st_mode))
                is_dir(paths[i], options, &header);
        }

        // Check if -delete option is present
        if (options != NULL && options->delete)
        {
            if (remove(paths[i]) == 0)
            {
                // Successful deletion
                printf("%s: deleted\n", paths[i]);
            }
            else
            {
                // Deletion failed
                perror("myfind");
                fprintf(stderr, ": cannot delete '%s': %s\n", paths[i], strerror(errno));
            }
        }

        i++;
    }
}








/* 
La fonction commence par vérifier si l'argument est nul. Si c'est le cas, 
elle affiche un message d'erreur sur la sortie d'erreur standard et termine 
le programme avec un code de retour indiquant une erreur.
*/
static void parse_name_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -name: requires additional arguments\n");
       exit(1);
    }
    op->name = arg;
    op->next = malloc(sizeof(opt));
} 



static void parse_execdir_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -execdir: requires additional arguments\n");
        exit(1);
    }
    op->execdir = arg;
    op->next = malloc(sizeof(opt));
}



/* Parse the -print option */
static void parse_print_option(opt *op)
{
    op->print = 1;
    op->next = malloc(sizeof(opt));
}

/* 
cette fonction analyse l'argument de l'option "-type" 
et crée une nouvelle structure "opt" pour stocker les informations correspondantes, 
tout en effectuant des vérifications pour s'assurer que l'argument est valide.
 */

static void parse_type_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -type: requires additional arguments\n");
       exit(1);
    }
    if (strlen(arg) > 1) {
        fprintf(stderr, "myfind: -type: this option takes one letter\n");
       exit(1);
    }
    char type_of = *arg;
    if (type_of != 'b' && type_of != 'c' && type_of != 'd' && type_of != 'f' && type_of != 'l' && type_of != 'p' && type_of != 's') {
        fprintf(stderr, "myfind: -type: %c: unknown letter\n", type_of);
       exit(1);
    }
    op->types = type_of;
    op->next = malloc(sizeof(opt));
}

/* Parse the -newer option */
static void parse_newer_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -newer: requires additional arguments\n");
       exit(1);
    }
    op->newer = arg;
    op->next = malloc(sizeof(opt));
}






















static void parse_exec_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -exec: requires additional arguments\n");
       exit(1);
    }

    // Check if command contains {}
    if (strstr(arg, "{}") == NULL) {
        fprintf(stderr, "myfind: -exec: command does not contain {}\n");
       exit(1);
    }

  
    strncpy(op->exec, arg, 5);
    op->next = malloc(sizeof(opt));
}




 void parse_delete_option(opt *op) {
    op->delete_files = 1;
}









static void parse_perm_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -perm: requires additional arguments\n");
        exit(EXIT_FAILURE);
    }

    perm_mode_t mode = strtol(arg, NULL, 8);
    if (mode == 0 && strcmp(arg, "0") != 0) {
        fprintf(stderr, "myfind: -perm: invalid mode '%s'\n", arg);
        exit(EXIT_FAILURE);
    }

    if (op->perm_mode == MODE_NONE) {
        op->perm_mode = MODE_EXACT;
        op->perm = mode;
    } else if (op->perm_mode == MODE_EXACT) {
        op->perm &= mode;
    } else if (op->perm_mode == MODE_ANY) {
        op->perm |= mode;
    } else {
        fprintf(stderr, "myfind: -perm: invalid mode '%s'\n", arg);
        exit(EXIT_FAILURE);
    }
}














static void parse_group_option(opt *op, char *arg)
{
    struct passwd *pw;
    struct group *gr;
    char *endptr;
    uid_t uid;
    gid_t gid;

    if (*arg == '+') {  // user 
        arg++;
        pw = getpwnam(arg);
        if (pw == NULL) {
            fprintf(stderr, "myfind: Unknown user '%s'\n", arg);
            exit(EXIT_FAILURE);
        }
        uid = pw->pw_uid;
        op->check_perm = true;
        op->perm_user = uid;
    } else if (*arg == '-') {  // group 
        arg++;
        gr = getgrnam(arg);
        if (gr == NULL) {
            fprintf(stderr, "myfind: Unknown group '%s'\n", arg);
            exit(EXIT_FAILURE);
        }
        gid = gr->gr_gid;
        op->check_perm = true;
        op->perm_group = gid;
    } else {
        // Invalid argument format
        fprintf(stderr, "myfind: Invalid argument '%s' for -perm option\n", arg);
        exit(EXIT_FAILURE);
    }
}






















/* 
Cette fonction est généralement utilisée dans une boucle pour analyser toutes les options passées au programme.
 */


static opt *parse_option(opt *op, char *optname, char *arg)
{
    if (strcmp(optname, "-name") == 0) {
        parse_name_option(op, arg);
    } else if (strcmp(optname, "-print") == 0) {
        parse_print_option(op);
    } else if (strcmp(optname, "-type") == 0) {
        parse_type_option(op, arg);
    } else if (strcmp(optname, "-newer") == 0) {
        parse_newer_option(op, arg);
    } else if (strcmp(optname, "-exec") == 0) {
        parse_exec_option(op, arg);
    } else if (strcmp(optname, "-execdir") == 0) {
        parse_execdir_option(op, arg);
    } else if (strcmp(optname, "-delete") == 0) {
        parse_delete_option(op);
    } else if (strcmp(optname, "-perm") == 0) {
       parse_perm_option(op, arg);
    } else if (strcmp(optname, "-user") == 0) {
        parse_user_option(op, arg);
    } else if (strcmp(optname, "-group") == 0) {
        parse_group_option(op, arg);
    } else {
        fprintf(stderr, "myfind: %s: Unknown option\nUsage: ./myfind [starting-point ...] [expressions] ...", optname);
        exit(EXIT_FAILURE);
    }
    return op->next;
}


















/* Parse the command line arguments */
opt *parsing_arguments(int argc, char **argv, char **spath)
{
    opt *op = malloc(sizeof(opt));
    opt *res = op;

    int i = 1;
    while (i < argc) {
        if (argv[i][0] == '-') {
            op = parse_option(op, argv[i], argv[i+1]);
            i++;
        } else {
            spath[0] = argv[i];
            spath++;
        }
        i++;
    }
    spath[0] = NULL;
    return res;
}




















