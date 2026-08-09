#include "../include/builtins.h"
#include "../include/executable.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/redirection.h"
#include "../include/pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *buffer = NULL;
  size_t size = 0;
  ssize_t len;
  setbuf(stdout, NULL);
  while (true) {
    printf("$ ");
    if ((len = getline(&buffer, &size, stdin)) != -1) {
      buffer[strcspn(buffer, "\n")] = '\0';

      // Parse the input into tokens
      char **argv = parse_arguments(buffer);

      // Detect Redirection
      int saved_stdout = -1;
      int saved_stderr = -1;
      hijack_redirections(argv, &saved_stdout, &saved_stderr);

      // If the user just pressed Enter (empty command) just ignore it :)
      if (argv != NULL && argv[0] != NULL) {
        if (has_pipe(argv)) {
          execute_pipeline(argv);
        } else {
          // Check if argv[0] is a builtin
          int status = handle_builtin(argv); // Pass argv instead of buffer!
          if (status == -1) {
            free_arguments(argv);
            break;
          } else if (status == 0) {
            // If not a built-in then run as external program
            char *exec_path = find_executable(argv[0]);
            if (exec_path != NULL) {
              execute_program(exec_path, argv);
              free(exec_path);
            } else {
              printf("%s: command not found\n", argv[0]);
            }
          }
        }
      }
      // Safely frees the arguments
      free_arguments(argv);

      // Restore Hijack
      restore_redirections(saved_stdout, saved_stderr);
    }
  }
  free(buffer);
  return 0;
}
