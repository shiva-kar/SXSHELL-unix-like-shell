#include "../include/parser.h"
#include "../include/shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parse_arguments(char *buffer) {
  char **args = malloc(sizeof(char *) * MAX_ARGS);
  if (args == NULL) {
    perror("Malloc failed for args");
    exit(EXIT_FAILURE);
  }
  int args_count = 0;
  bool is_single_quotes = false;

  char *current_word = malloc(strlen(buffer) + 1);
  int word_len = 0;
  for (int i = 0; buffer[i] != '\0'; i++) {
    char c = buffer[i];
    if (c == '\'') {
      is_single_quotes = !is_single_quotes;
      continue; // don't include the quote char in the token
    }
    if (c == ' ') {
      if (!is_single_quotes) {
        if (word_len > 0) {
          current_word[word_len] = '\0';
          args[args_count++] = strdup(current_word);
          word_len = 0;
        }
        continue; // ignore extra spaces outside quotes
      }
    }

    current_word[word_len++] = c;
  }
  // save trailing token if string ended without a space
  if (word_len > 0) {
    current_word[word_len] = '\0';
    args[args_count++] = strdup(current_word);
  }
  free(current_word);
  args[args_count] = NULL;

  return args;
}
void free_arguments(char **args) {
  if (args != NULL) {
    // strdup allocates new memory for each string so it frees them individually
    for (int i = 0; args[i] != NULL; i++) {
      free(args[i]);
    }
    free(args);
  }
}
