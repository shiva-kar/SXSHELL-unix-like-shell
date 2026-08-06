#include "../include/redirection.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void hijack_redirections(char **argv, int *stdout, int *stderr) {
  char *output_file = NULL;
  int cut_index = -1;

  for (int i = 0; argv[i] != NULL; i++) {

    // Stdout Overwrite
    if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "1>") == 0) {
      output_file = argv[i + 1];
      if (cut_index == -1)
        cut_index = i;

      if (output_file != NULL) {
        if (*stdout == -1)
          *stdout = dup(STDOUT_FILENO); // Safe backup check
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
    }
    // Stderr Overwrite
    else if (strcmp(argv[i], "2>") == 0) {
      output_file = argv[i + 1];
      if (cut_index == -1)
        cut_index = i;

      if (output_file != NULL) {
        if (*stderr == -1)
          *stderr = dup(STDERR_FILENO); // Safe backup check
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDERR_FILENO);
        close(fd);
      }
    }
    // Stdout Append
    else if (strcmp(argv[i], ">>") == 0 || strcmp(argv[i], "1>>") == 0) {
      output_file = argv[i + 1];
      if (cut_index == -1)
        cut_index = i;

      if (output_file != NULL) {
        if (*stdout == -1)
          *stdout = dup(STDOUT_FILENO); // Safe backup check
        int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
    }
    // Stderr Append
    else if (strcmp(argv[i], "2>>") == 0) {
      output_file = argv[i + 1];
      if (cut_index == -1)
        cut_index = i;

      if (output_file != NULL) {
        if (*stderr == -1)
          *stderr = dup(STDERR_FILENO); // Safe backup check
        int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd, STDERR_FILENO);
        close(fd);
      }
    }
  }

  // Hide redirections from the command
  if (cut_index != -1) {
    argv[cut_index] = NULL;
  }
}

void restore_redirections(int stdout, int stderr) {
  if (stdout != -1) {
    dup2(stdout, STDOUT_FILENO);
    close(stdout);
  }
  if (stderr != -1) {
    dup2(stderr, STDERR_FILENO);
    close(stderr);
  }
}
