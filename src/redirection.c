#include "../include/redirection.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int hijack_stdout(char **argv){
  char *output_file = NULL;
      for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "1>") == 0) {
          // The next argument is the file.
          output_file = argv[i + 1];
          // Hides the redirection from the actual command by cutting the array
          // short.
          argv[i] = NULL;
          break;
        }
      }
      int saved_stdout = -1;
      if (output_file != NULL) {
        // Save a backup of the real stdout
        saved_stdout = dup(STDOUT_FILENO);

        // Open the file (Write-only, Create if missing, Truncate/Overwrite if exists, Read/Write permissions)
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        // Hijack stdout! Force FD 1 to point to your new file instead of the terminal.
        dup2(fd, STDOUT_FILENO);

        // Close the temporary fd since it's now safely cloned into FD 1
        close(fd);
      }
      return saved_stdout;
}

void restore_stdout(int stdout){
  if (stdout != -1) {
    // Restore the backup over FD 1
    dup2(stdout, STDOUT_FILENO);
    // Close the backup
    close(stdout);
  }
}
