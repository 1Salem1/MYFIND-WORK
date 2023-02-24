#ifndef DELETE_FILE_H
#define DELETE_FILE_H

/**
 * Deletes the file or directory at the given pathname.
 * 
 * Returns 0 on success, or a non-zero value on failure.
 */
int delete_file(const char *pathname);

/**
 * Deletes the file or directory at the given pathname and prints an error message
 * to stderr if the operation fails.
 */
void delete_file_and_print_error(const char *pathname);

#endif /* DELETE_FILE_H */
