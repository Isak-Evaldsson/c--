#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/** Utility function for printing formated error messages.
    @param type used to specify which type of error it is, e.g. Lexer error
    @param fmt provides error details, fromated string that works like printf 
*/
void error(char *type, const char *fmt, ...)
{
    va_list ap;
    char buf[BUFSIZ];

    // Creates formated string in buf
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);

    // Prints buffer and exits with error
    fprintf(stderr, "%s: %s\n", type, buf);
    exit(1);
}

/**
 * @brief Allcaties a new string and copies the contents of the supplied string.
 * 
 * @param string the string to copy
 * @return char* 
 */
char *copy_str(char *string)
{
    char *t = calloc(sizeof(char), strlen(string) + 1);
    strcpy(t, string);
    return t;
}