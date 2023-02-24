#include "include/myfind.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "include/is_file.h"
#include "include/is_dir.h"


int filter_by_permission(const char *pathname, const struct stat *statbuf, const opt *op) {
    mode_t mode = statbuf->st_mode;
    int perm = strtol(op->perm, NULL, 8);

    if (op->perm[0] == '-') {
        // Match all bits in the mode
        return (mode & perm) == perm;
    } else if (op->perm[0] == '/') {
        // Match any bit in the mode
        return (mode & perm) != 0;
    } else {
        // Match exact mode
        return mode == perm;
    }
}