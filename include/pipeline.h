#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdbool.h>

// Returns true if a pipe operator "|" is found anywhere in the arguments
bool has_pipe(char **argv);

// Executes a dual-command pipeline
void execute_pipeline(char **argv);

#endif // PIPELINE_H
