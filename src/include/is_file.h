#ifndef IS_FILE_H
#define IS_FILE_H

#include "myfind.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void is_file(char *fp_path, opt *p, struct stat *hdr);
int newer(char *fp_path, opt *p, struct stat *hdr);
int match_name(char *path, opt *options);
int match_types(opt *options, struct stat *header);

#endif 
