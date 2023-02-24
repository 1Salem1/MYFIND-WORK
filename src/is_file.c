#include "include/is_file.h"

#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "include/myfind.h"


/* 
La méthode is_file est appelée pour chaque fichier trouvé par myfind. 
Elle prend en entrée le chemin d'un fichier, une structure opt contenant les options spécifiées par l'utilisateur pour la recherche, 
et un pointeur vers la structure stat du fichier.
La méthode parcourt chaque option spécifiée et exécute la méthode correspondante  [match_name, match_types, newer]
 pour déterminer si le fichier correspond aux critères de recherche. Si le fichier correspond aux critères de recherche, 
 elle affiche le nom du fichier en fonction des options spécifiées. Si aucun critère de recherche ne correspond au fichier, 
 elle ne fait rien. Enfin, elle affiche le nom du fichier si l'option -print est spécifiée en entrée et que le fichier n'a pas été affiché auparavant.
 */

void is_file(char *path, opt *options, struct stat *header)
{
    int shouldPrint = 1;

    while (options != NULL)
    {
        if (options->name)
        {
            if (match_name(path, options) == 0)
                return;
        }
        if (options->print)
        {
            printf("%s\n", path);
            shouldPrint = 0;
        }
        if (options->types)
        {
            if (match_types(options, header) == 0)
                return;
        }
        if (options->newer)
        {
            if (newer(path, options, header) == 0)
                return;
        }
        options = options->next;
    }

    if (shouldPrint)
        printf("%s\n", path);
}






/*
Cette fonction vérifie d'abord si l'option newer est spécifiée. 
Si ce n'est pas le cas, elle retourne toujours vrai.
Si l'option newer est spécifiée, la fonction utilise la fonction stat pour récupérer les informations sur le fichier spécifié dans l'option newer. 
Si stat renvoie une erreur, la fonction affiche un message d'erreur et quitte le programme avec un code d'erreur.
Si stat réussit, la fonction compare les temps de modification des deux fichiers en utilisant la fonction difftime de la bibliothèque standard time.h.
Si le temps de modification du fichier path est plus récent que celui du fichier spécifié dans l'option newer, 
la fonction affiche le chemin du fichier path et retourne vrai. Sinon, elle retourne faux.
*/


int newer(char *path, opt *options, struct stat *header) {
    struct stat header2;
    int status;
    if (options->newer == NULL) {
        return 1;
    }
    status = stat(options->newer, &header2);
    if (status == -1) {
       fprintf(stderr, "myfind: error: cannot get information for file '%s': %s\n", options->newer, strerror(errno));
       return EXIT_FAILURE;
    }
    if (difftime(header->st_mtime, header2.st_mtime) > 0.0) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}





/* 
La fonction match_name prend en entrée un chemin de fichier path et un pointeur vers une structure opt contenant des options de recherche de fichiers.
La fonction extrait le nom de fichier à partir du chemin complet en utilisant la fonction basename de la bibliothèque standard libgen.h.
Ensuite, la fonction utilise la fonction fnmatch de la bibliothèque standard fnmatch.h 
pour comparer le nom de fichier extrait avec le motif de recherche spécifié dans l'option name de la structure opt.
*/


int match_name(char *path, opt *options)
{
    char *file = basename(path);
    int result = fnmatch(options->name, file, 0);
    if (result != 0)
        return 0;
    return 1;
}




/* 

La fonction match_types est utilisée pour vérifier 
si le type de fichier spécifié dans les options (parms) correspond au type de fichier du fichier dont 
les informations sont stockées dans la structure hdr.
La fonction utilise les informations de la

*/


int match_types(opt *parms, struct stat *hdr) {
    mode_t file_mode = hdr->st_mode;

    if (S_ISBLK(file_mode) && parms->types == 'b') {
        return 1;
    } else if (S_ISCHR(file_mode) && parms->types == 'c') {
        return 1;
    } else if (S_ISDIR(file_mode) && parms->types == 'd') {
        return 1;
    } else if (S_ISREG(file_mode) && parms->types == 'f') {
        return 1;
    } else if (S_ISFIFO(file_mode) && parms->types == 'p') {
        return 1;
    } else if (S_ISLNK(file_mode) && parms->types == 'l') {
        return 1;
    } else if (S_ISSOCK(file_mode) && parms->types == 's') {
        return 1;
    } else {
        return 0;
    }
}