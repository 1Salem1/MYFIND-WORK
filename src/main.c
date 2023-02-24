#include "include/myfind.h"

int main(int argc, char *argv[])
{
    char *list_arr[argc];

    opt *args = parsing_arguments(argc, list_arr, argv);

    myfind(list_arr, args);

    return 0;
}


