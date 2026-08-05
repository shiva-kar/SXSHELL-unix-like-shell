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
  bool is_double_quotes = false;

  char *current_word = malloc(strlen(buffer) + 1);
  int word_len = 0;

  for (int i = 0; buffer[i] != '\0'; i++) {
    char c = buffer[i];

    // backslashes outside quotes
    if (c == '\\' && !is_single_quotes && !is_double_quotes) {
      char next_char = buffer[i + 1];
      if (next_char != '\0') {
        current_word[word_len++] = next_char;
        i++;
      }
      continue;
    }

    // backslashes inside double quotes
    if (c == '\\' && is_double_quotes) {
      char next_char = buffer[i + 1];
      if (next_char == '"' || next_char == '\\' || next_char == '$' ||
          next_char == '\n') {
        current_word[word_len++] = next_char;
        i++;
        continue;
      }
    }

    // double quotes
    if (c == '\"') {
      if (!is_single_quotes) {
        is_double_quotes = !is_double_quotes;
        continue;
      }
    }

    // single quotes
    if (c == '\'') {
      if (!is_double_quotes) {
        is_single_quotes = !is_single_quotes;
        continue;
      }
    }

    // spaces
    if (c == ' ') {
      if (!is_single_quotes && !is_double_quotes) {
        if (word_len > 0) {
          current_word[word_len] = '\0';
          args[args_count++] = strdup(current_word);
          word_len = 0;
        }
        continue;
      }
    }

    // normal characters
    current_word[word_len++] = c;
  }

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
    for (int i = 0; args[i] != NULL; i++) {
      free(args[i]);
    }
    free(args);
  }
}
