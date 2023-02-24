#include "include/is_dir.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/is_file.h"
#include "include/myfind.h"

void is_dir(char *dir_path, opt *parms, struct stat *hdr)
{   
    struct dirent *sdir;

    DIR * dir = opendir(dir_path);
    if (dir == NULL)
    {
        fprintf(stderr, "myfind: %s %s\n", dir_path,strerror(errno));
        return;
    }

    while ((sdir = readdir(dir)) != NULL)
    {
        if (!((strcmp(sdir->d_name, ".")) == 0
            || (strcmp(sdir->d_name, "..")) == 0))
        {
            size_t len_path = strlen(dir_path) + strlen(sdir->d_name);

          //  char *path = malloc(sizeof(char) * len_path);

            char path[len_path];

            if (dir_path[strlen(dir_path) - 1] == '/')
                sprintf(path, "%s%s", dir_path, sdir->d_name);
            else
                sprintf(path, "%s/%s", dir_path, sdir->d_name);

            int status = stat(path, hdr);
            if (status == -1)
            {
                fprintf(stderr, "myfind: %s: %s\n", path, strerror(errno));
             exit(1);
            }

            if (S_ISDIR(hdr->st_mode))
            {
                is_file(path, parms, hdr);
                is_dir(path, parms, hdr);
            }
            else
                is_file(path, parms, hdr);
                free(path);
        }
        else
            continue;
    }
    closedir(dir);
}
