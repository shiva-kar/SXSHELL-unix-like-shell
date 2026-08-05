#include "../include/builtins.h"
#include "../include/executable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH(x) sizeof(x) / sizeof(x[0])

char *builtins[4] = {
    "type",
    "quit",
    "exit",
    "echo",
};

int handle_builtin(char *buffer) {
    if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
        return -1;
    } else if (strncmp(buffer, "type ", 5) == 0) {
        char *target = buffer + 5;
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
    } else if (strncmp(buffer, "echo ", 5) == 0) {
        fprintf(stdout, "%s\n", buffer + 5);
        return 1;
    }
    
    return 0;
}
