#include "../include/executable.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

char *find_executable(const char *command) {
  char *path_env = getenv("PATH");
  if (!path_env)
    return NULL;

  char *path_copy = strdup(path_env);
  if (!path_copy)
    return NULL;

  const char *delimiter = ":";

  char *dir = strtok(path_copy, delimiter);
  char full_path[1024];

  while (dir != NULL) {

    snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

    if (access(full_path, X_OK) == 0) {
      char *result = strdup(full_path);
      free(path_copy);
      return result;
    }

    dir = strtok(NULL, delimiter);
  }

  free(path_copy);
  return NULL;
}
