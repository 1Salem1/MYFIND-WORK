#ifndef MYFIND_H
#define MYFIND_H

typedef struct option
{
    char *name;
    int print;
    char types;
    char *newer;
    struct option *next;
}opt;

void myfind(char **path, opt *args);
opt *Parsing_args(int len, char **spath, char **parms);

#endif /* ! MYFIND_H */
