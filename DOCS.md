# Building a Unix Shell from Scratch

I built this shell from the ground up in C to deeply understand POSIX system calls, process management, memory allocation, and file descriptor manipulation. 

If you are looking to learn how a shell actually works under the hood (or if you want to build your own), this document breaks down exactly how my engine operates.

## 1. The REPL (Read, Eval, Print Loop)
Every shell starts with an infinite loop that waits for user input. In [`src/main.c`](file:///d:/My%20Projects/unix-like-shell/src/main.c), I use the standard `getline()` function to read a raw string of text from the keyboard.

```c
while (true) {
    printf("$ ");
    if ((len = getline(&buffer, &size, stdin)) != -1) {
        // Strip the newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // 1. Parse
        // 2. Redirect
        // 3. Execute
    }
}
```

## 2. The Parser: Quotes and Escapes
You can't just split a command by spaces using `strtok()`. If a user types `echo "hello world"`, the space inside the quotes must be preserved. 

To solve this, I built a custom state-machine parser in [`src/parser.c`](file:///d:/My%20Projects/unix-like-shell/src/parser.c). The parser reads the string character-by-character and flips boolean flags (like `in_single_quotes` or `in_double_quotes`) on and off. When it hits a space, it only splits the string into a new argument if all the quote flags are currently off.

## 3. Built-ins vs External Commands
When the user types a command, the shell must decide how to run it.

First, I check if it is a built-in command in [`src/builtins.c`](file:///d:/My%20Projects/unix-like-shell/src/builtins.c). Built-ins (like `cd`, `exit`, or `pwd`) must be executed directly by the main shell process. For example, if a child process runs `cd`, it only changes the directory for the child, not the parent shell!

If it is not a built-in, I search the system `$PATH` environment variable in [`src/executable.c`](file:///d:/My%20Projects/unix-like-shell/src/executable.c) to find the absolute path to the binary (like `/usr/bin/ls`). I then fork a child process and execute the binary using `execv()`.

## 4. Redirection: Hijacking File Descriptors
Redirection (`>`, `>>`, `2>`) allows you to write command output to a file instead of the screen. I handle this in [`src/redirection.c`](file:///d:/My%20Projects/unix-like-shell/src/redirection.c).

The process requires hijacking the standard output (File Descriptor 1).
1. Save a backup of the real terminal output using `dup()`.
2. Open the target text file using `open()`.
3. Hijack standard output using `dup2()`, forcing FD 1 to point to the open file.

```c
// Save a backup of the real stdout
*stdout_backup = dup(STDOUT_FILENO);

// Open the file (Write-only, Create if missing, Append if exists)
int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

// Hijack stdout! Force FD 1 to point to the file instead of the terminal.
dup2(fd, STDOUT_FILENO);
close(fd); // Close the temporary fd since it's now cloned into FD 1
```
Once the command finishes, the main loop calls `restore_redirections()` to put the terminal back using the backup we saved.

## 5. Pipelines: Concurrent Processes
Pipelines (`|`) are the most complex architectural component, located in [`src/pipeline.c`](file:///d:/My%20Projects/unix-like-shell/src/pipeline.c). Instead of writing to a static text file, I hook the standard output of one running program directly into the standard input of another concurrently running program.

To do this, I use the `pipe()` system call to create a read-end and a write-end in memory. I then call `fork()` twice to create two child processes. 

* The **Left Child** uses `dup2()` to connect its `STDOUT_FILENO` to the pipe's write-end. 
* The **Right Child** uses `dup2()` to connect its `STDIN_FILENO` to the pipe's read-end.

```c
// Left Child (Writer)
dup2(pipefd[1], STDOUT_FILENO);
close(pipefd[0]);
close(pipefd[1]);
execv(left_executable, left_argv);

// Right Child (Reader)
dup2(pipefd[0], STDIN_FILENO);
close(pipefd[0]);
close(pipefd[1]);
execv(right_executable, right_argv);
```

**The Deadlock Trap:** The most critical part of this architecture is the parent cleanup. If the parent shell does not explicitly close its copies of the pipe file descriptors before calling `wait()`, the child processes will never receive an EOF (End Of File) signal, resulting in a permanent pipeline freeze.
