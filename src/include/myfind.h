#ifndef MYFIND_H
#define MYFIND_H


// myfind options
typedef struct option
{
   char *name;
    int print;
    char types;
    char *newer;
    char *exec;
    char *execdir;
    int perm;
    int delete;
    int delete_files;
    int perm_mode;
    struct option *next;
} opt;


void myfind(char **path, opt *args);
opt *parsing_arguments(int len, char **spath, char **parms);

#endif 
