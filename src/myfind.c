#include "include/myfind.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "include/is_file.h"
#include "include/is_dir.h"




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
        return EXIT_FAILURE;
    }
    op->name = arg;
    op->next = malloc(sizeof(opt));
} 



static void parse_execdir_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -execdir: requires additional arguments\n");
         return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }
    if (strlen(arg) > 1) {
        fprintf(stderr, "myfind: -type: this option takes one letter\n");
        return EXIT_FAILURE;
    }
    char type_of = *arg;
    if (type_of != 'b' && type_of != 'c' && type_of != 'd' && type_of != 'f' && type_of != 'l' && type_of != 'p' && type_of != 's') {
        fprintf(stderr, "myfind: -type: %c: unknown letter\n", type_of);
        return EXIT_FAILURE;
    }
    op->types = type_of;
    op->next = malloc(sizeof(opt));
}

/* Parse the -newer option */
static void parse_newer_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -newer: requires additional arguments\n");
        return EXIT_FAILURE;
    }
    op->newer = arg;
    op->next = malloc(sizeof(opt));
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
        op->delete = 1;
         op->action = &delete_file_and_print_error;
    } else if (strcmp(optname, "-perm") == 0) {
        op->perm = arg;
        op->filter = &filter_by_permission;
    } else {
        fprintf(stderr, "myfind: %s: Unknown option\nUsage: ./myfind [starting-point ...] [expressions] ...", optname);
        exit(EXIT_FAILURE);
    }
    return op->next;
}









static void parse_exec_option(opt *op, char *arg)
{
    if (arg == NULL) {
        fprintf(stderr, "myfind: -exec: requires additional arguments\n");
        return EXIT_FAILURE;
    }

    // Check if command contains {}
    if (strstr(arg, "{}") == NULL) {
        fprintf(stderr, "myfind: -exec: command does not contain {}\n");
        return EXIT_FAILURE;
    }

  
    strncpy(op->exec, arg, 5);
    op->next = malloc(sizeof(opt));
}











/* Parse the command line arguments */
static opt *parse_arguments(int argc, char **argv, char **spath)
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


















