#include "../include/process.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int execute_program(const char *exec_path, char *const argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("Error: fork() failed on process.c line 11.");
      return -1;
    }
    if (pid == 0) {
      if(execv(exec_path, argv) == -1){
      perror("Error: execv() failed on process.c line 18.");
      exit(EXIT_FAILURE);
      }
    }
    if (pid > 0) {
      int status;
      waitpid(pid, &status, 0);
    }
    return 0;
}
