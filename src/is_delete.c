#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


int delete_file(const char *pathname) {
    int result = 0;
    DIR *dir = opendir(pathname);
    struct dirent *entry;
    char filepath[4096];

    if (dir == NULL) {
        // It's a file or a symlink
        result = remove(pathname);
        if (result != 0) {
            fprintf(stderr, "Failed to delete file %s: %s\n", pathname, strerror(errno));
        }
    } else {
        // It's a directory
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snprintf(filepath, sizeof(filepath), "%s/%s", pathname, entry->d_name);
                result = delete_file(filepath);
                if (result != 0) {
                    break;
                }
            }
        }
        closedir(dir);

        if (result == 0) {
            result = rmdir(pathname);
            if (result != 0) {
                fprintf(stderr, "Failed to delete directory %s: %s\n", pathname, strerror(errno));
            }
        }
    }

    return result;
}


void delete_file_and_print_error(const char *pathname) {
    int result = delete_file(pathname);
    if (result != 0) {
        fprintf(stderr, "Failed to delete %s: %s\n", pathname, strerror(errno));
    }
}

