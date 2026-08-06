#ifndef REDIRECTION_H_
#define REDIRECTION_H_

// Check for ">" to Hijack STDOUT_FILENO
int hijack_stdout(char **argv);

// Restore Hijack
void restore_stdout(int stdout);

#endif // REDIRECTION_H_
