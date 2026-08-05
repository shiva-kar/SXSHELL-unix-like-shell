#include "../include/builtins.h"
#include "../include/executable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH(x) sizeof(x) / sizeof(x[0])

static char *builtins[] = {"type", "quit", "exit", "echo", "pwd", "cd"};

int handle_builtin(char **argv) {
  // argv[0] is guaranteed to exist here because it is checked in main.c
  if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) {
    return -1;
  }

  // Command: type
  else if (strcmp(argv[0], "type") == 0) {
    // Safeguard if user just typed "type"
    if (argv[1] == NULL)
      return 1;

    char *target = argv[1];
    bool is_found = false;

    for (int i = 0; i < LENGTH(builtins); i++) {
      if (strcmp(target, builtins[i]) == 0) {
        fprintf(stdout, "%s is a shell builtin\n", target);
        is_found = true;
        break;
      }
    }

    if (!is_found) {
      char *exec_path = find_executable(target);
      if (exec_path != NULL) {
        fprintf(stdout, "%s is %s\n", target, exec_path);
        free(exec_path);
        is_found = true;
      }
    }

    if (!is_found) {
      fprintf(stdout, "%s: not found\n", target);
    }
    return 1;
  }

  // Command: echo
  else if (strcmp(argv[0], "echo") == 0) {
    // Loop through all arguments and print them separated by a space
    for (int i = 1; argv[i] != NULL; i++) {
      fprintf(stdout, "%s", argv[i]);
      if (argv[i + 1] != NULL) {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
    return 1;
  }

  // Command: pwd
  else if (strcmp(argv[0], "pwd") == 0) {
    char *cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
      fprintf(stdout, "%s\n", cwd);
      // free the memory allocated by getcwd
      free(cwd);
    } else {
      perror("getcwd() error");
      return -1;
    }
    return 1;
  }

  else if (strcmp(argv[0], "cd") == 0) {
    if (argv[1] != NULL) {
        const char *path = argv[1];

        // Handles the '~' shortcut for the HOME directory
        if (strcmp(path, "~") == 0) {
            path = getenv("HOME");
            if (path == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
        }

        // Attempt to change directory
        if (chdir(path) != 0) {
            fprintf(stdout, "cd: %s: No such file or directory\n", argv[1]);
        }
    } else {
        // Safeguard/shortcut if they just type "cd" with no arguments just go HOME
        const char *home = getenv("HOME");
        if (home != NULL) {
            chdir(home);
        }
    }
    return 1;
}

  // Not a built-in
  return 0;
}
