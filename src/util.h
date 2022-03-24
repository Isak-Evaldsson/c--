#ifndef UTIL_HEADER
#define UTIL_HEADER

#define PRINT 1 // enable/disable prints.

#if PRINT
#define pr(...)                       \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (0)
#else
#define pr(...) /* no effect at all */
#endif

/** Utility function for printing formated error messages.
    @param type used to specify which type of error it is, e.g. Lexer error
    @param fmt provides error details, fromated string that works like printf 
*/
void error(char *type, const char *fmt, ...);

/**
 * @brief Allcaties a new string and copies the contents of the supplied string.
 * 
 * @param string the string to copy
 * @return char* 
 */
char *copy_str(char *string);
#endif