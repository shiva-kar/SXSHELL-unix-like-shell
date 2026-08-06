#ifndef REDIRECTION_H_
#define REDIRECTION_H_

// Check for redirection characters ( ">" "2>" ">>" etc..) to Hijack redirection
// of one or both STDOUT and STDERR.
void hijack_redirections(char **argv, int *stdout, int *stderr);

// Restore Hijack redirection to original saved stdout and stderr.
void restore_redirections(int stdout, int stderr);

#endif // REDIRECTION_H_
