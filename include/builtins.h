#ifndef BUILTINS_H
#define BUILTINS_H

extern char *builtins[4];

// handles all the system logic for builtins like type/pwd/exit/quit... etc.
int handle_builtin(char *buffer);

#endif // BUILTINS_H
