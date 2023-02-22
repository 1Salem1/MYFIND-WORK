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


int newer(char *path, opt *options, struct stat *header)
{
    struct stat header2;

    int status = stat(options->newer, &header2);

    if (status == -1)
    {
        fprintf(stderr, "myfind: %s: %s\n", options->newer, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        if (header->st_mtime > header2.st_mtime)
        {
            printf("%s\n", path);
            return 0;
        }
    }
    return 0;
}

int match_name(char *path, opt *options)
{
    char *file = basename(path);
    int result = fnmatch(options->name, file, 0);
    if (result != 0)
        return 0;
    return 1;
}

int match_types(opt *parms, struct stat *hdr)
{
    char letter;

    if (S_ISBLK(hdr->st_mode))
        letter = 'b';
    else if (S_ISCHR(hdr->st_mode))
        letter = 'c';
    else if (S_ISDIR(hdr->st_mode))
        letter = 'd';
    else if (S_ISREG(hdr->st_mode)) 
        letter = 'f';
   // else if (S_ISLNK(hdr->st_mode))
      //  letter = 'l';
    else if (S_ISFIFO(hdr->st_mode))
        letter = 'p';
   // else if (S_ISSOCK(hdr->st_mode))
     //   letter = 's';
    else
        letter = '?';

    if (letter != parms->types)
        return 0;
    return 1;
}
