#ifndef IS_FILE_H
#define IS_FILE_H

#include "myfind.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>







// It checks whether the file at the given path exists and is a regular file.
void is_file(char *file_path, opt *options, struct stat *header);
/* If it is a regular file and its modification time is more recent than the modification time of the file pointed to by the stat struct,
 the function returns 1. Otherwise, it returns 0. */
int newer(char *file_path, opt *options, struct stat *header);
 // It checks whether the file matches the name pattern specified in the options.
int match_name(char *path, opt *options);
// It checks whether the file type matches the type specified in the options.
int match_types(opt *options, struct stat *header);

#endif
