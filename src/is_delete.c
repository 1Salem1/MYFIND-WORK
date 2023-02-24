#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "include/myfind.h"

static int delete_file(const char *path) {
    struct stat st;
    if (lstat(path, &st) != 0) {
        fprintf(stderr, "myfind: %s: %s\n", path, strerror(errno));
        return 0;
    }
    if (S_ISDIR(st.st_mode)) {
        fprintf(stderr, "myfind: %s: cannot delete a directory\n", path);
        return 0;
    }
    if (unlink(path) != 0) {
        fprintf(stderr, "myfind: %s: %s\n", path, strerror(errno));
        return 0;
    }
    return 1;
}

int delete_files(char *dir_path, opt *parms, struct stat *hdr) {
    int result = 1;
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "myfind: %s: %s\n", dir_path, strerror(errno));
        return 0;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        size_t path_len = strlen(dir_path) + strlen(entry->d_name) + 2;
        char *path = malloc(path_len);
        snprintf(path, path_len, "%s/%s", dir_path, entry->d_name);
        int should_delete = 0;
        if (parms->name && fnmatch(parms->name, entry->d_name, 0) == 0) {
            should_delete = 1;
        }
        if (should_delete) {
            if (!delete_file(path)) {
                result = 0;
            }
        } else if (S_ISDIR(hdr->st_mode)) {
            if (!delete_files(path, parms, hdr)) {
                result = 0;
            }
        }
        free(path);
    }
    closedir(dir);
    if (parms->delete_files) {
        if (rmdir(dir_path) != 0) {
            fprintf(stderr, "myfind: %s: %s\n", dir_path, strerror(errno));
            result = 0;
        }
    }
    return result;
}
