#ifndef PROCESS_H
#define PROCESS_H

// Executes an external program by creating a child process.

int execute_program(const char *exec_path, char *const argv[]);

#endif // PROCESS_H
