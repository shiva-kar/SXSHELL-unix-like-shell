#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/builtins.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/executable.h"

int main(int argc, char *argv[]) {
  char *buffer = NULL;
  size_t size = 0;
  ssize_t len;
  setbuf(stdout, NULL);
  while (true) {
    printf("$ ");
    if ((len = getline(&buffer, &size, stdin)) != -1) {
      buffer[strcspn(buffer, "\n")] = '\0';

      int status = handle_builtin(buffer);
      if (status == -1) {
        break;
      } else if (status == 1) {
        continue;
      } else {
        // If not a built-in try to parse and run as an external program.
        char **argv_ext = parse_arguments(buffer);

        if (argv_ext != NULL && argv_ext[0] != NULL) {
            // Check if it exists in PATH
            char *exec_path = find_executable(argv_ext[0]);

            if (exec_path != NULL) {
                // If it exists, execute it
                execute_program(exec_path, argv_ext);
                free(exec_path);
            } else {
                printf("%s: command not found\n", argv_ext[0]);
            }

            // safely free up parsed arguments
            free_arguments(argv_ext);
        }
      }
    }
  }
  free(buffer);
  return 0;
}
