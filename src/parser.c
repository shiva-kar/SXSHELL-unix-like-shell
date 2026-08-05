#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parse_arguments(char *buffer) {
    int argsCount = 10;
    char **args = malloc(sizeof(char *) * argsCount);
    if (args == NULL) {
      perror("Malloc failed: on perser.c line 10.");
      exit(EXIT_FAILURE);
    }
    args[0] = strtok(buffer, " ");
    for (int i = 1; args[i-1] != NULL; i++) {
        if (i >= argsCount) {
            argsCount *= 2;
            char **temp = realloc(args, sizeof(char *) * argsCount);
            if (temp == NULL) {
                free(args);
                perror("Realloc failed");
                exit(EXIT_FAILURE);
            }
            args = temp;
        }
        args[i] = strtok(NULL, " ");
    }
    return args;
}

void free_arguments(char **args) {
  if (args != NULL) {
      free(args);
  }
}
