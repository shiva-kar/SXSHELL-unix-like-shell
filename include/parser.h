#ifndef PARSER_H
#define PARSER_H

// parses the string/char* into tokens
char **parse_arguments(char *buffer);

// safely frees the arguments/tokens returned by parse_arguments().
void free_arguments(char **args);

#endif // PARSER_H
