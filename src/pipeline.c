#include "../include/pipeline.h"
#include "../include/builtins.h"
#include "../include/executable.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

bool has_pipe(char **argv) {
  for (int i = 0; argv[i] != NULL; i++) {
    if (strcmp(argv[i], "|") == 0) {
      return true;
    }
  }
  return false;
}

void execute_pipeline(char **argv) {

  // Split the array into two commands and create pipe
  int pipe_index = 0;
  for (int i = 0; argv[i] != NULL; i++) {
    if (strcmp(argv[i], "|") == 0) {
      pipe_index = i;
    }
  }
  argv[pipe_index] = NULL;
  char **right_argv = &argv[pipe_index + 1];

  int pipefd[2];
  if (pipe(pipefd) != 0) {
    perror("Error: Pipe failed at pipeline.c line no 41");
  }

  // Fork the left child
  pid_t pid = fork();
  if (pid == 0) {
    // Stdout Overwrite
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
    if (handle_builtin(argv) == 1) {
      exit(0);
    } else {
      char *executable = find_executable(argv[0]);
      int status = execv(executable, argv);
      if (status == -1) {
        exit(1);
      }
    }
  }
  // Fork the right child
  pid = fork();
  if (pid == 0) {
    // Stdout Overwrite
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
    if (handle_builtin(right_argv) == 1) {
      exit(0);
    } else {
      char *executable = find_executable(*right_argv);
      int status = execv(executable, right_argv);
      if (status == -1) {
        exit(1);
      }
    }
  }

  // Parent Process Cleanup
  close(pipefd[0]);
  close(pipefd[1]);
  wait(NULL);
  wait(NULL);
}
