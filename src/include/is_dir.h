#ifndef IS_DIR_H
#define IS_DIR_H

#include "myfind.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void is_dir(char *dp_path, opt *parms, struct stat *hdr);

#endif /* ! IS_DIR_H */
