
![Logo](https://github.com/user-attachments/assets/430f729f-6951-4d38-89e3-511ebe4825b0)


# SXSHELL : a-unix-like-shell

A minimal Unix-like shell written from scratch in C.

I built this project to get hands-on experience with process management, memory allocation, and OS-level system calls (like `fork`, `execv`, `dup2`, and `open`). It handles everything from basic command execution to dual-command pipelines and file descriptor manipulation.

## Features

- **Base Shell Execution**:
  - REPL interface with prompt display and invalid command handling.
  - Resolves executable files in the `$PATH` environment variable.
  - Native implementation of fundamental built-ins (`exit`, `echo`, `type`).

- **Navigation**:
  - `pwd` to print current working directory.
  - `cd` with support for absolute paths, relative paths, and home directory expansion.

- **Advanced Quoting & Escaping**:
  - Single and double quotes support with whitespace preservation.
  - Backslash escaping both inside and outside of quotes.
  - Execution of quoted executables and arguments.

- **I/O Redirection**:
  - Redirect `stdout` (`>`) and `stderr` (`2>`) to files.
  - Append `stdout` (`>>`) and `stderr` (`2>>`) to files.

- **Pipelines**:
  - Dual-command pipelines.
  - Support for chaining built-in commands through pipes.

- **Planned / In Progress**:
  - History management and persistence (`history` builtin, arrow-key navigation, history file read/write).
  - Parameter expansion (`declare` builtin, `$VAR` and `${VAR}` expansion).

## Documentation & Learning Resource

If you are looking to build your own shell, I have started writing a comprehensive internal documentation guide. You can find it in [`DOCS.md`](DOCS.md).

This document breaks down my thought process, my architectural approach, and acts as a learning resource for understanding how POSIX system calls operate under the hood. As I add more functionality to this shell, I will continue to expand the documentation so it serves as a complete, step-by-step guide for systems engineering students.

## Build and Run

You'll need a C compiler (GCC/Clang), CMake, and the GNU Readline library.

```bash
# Clone the repository
git clone https://github.com/shiva-kar/SXSHELL-unix-like-shell.git
cd SXSHELL-unix-like-shell

# Generate build files and compile
cmake -B build
cmake --build build

# Start the shell
./build/shell
```

## Architecture Summary

- **Parser (`parser.c`)**: Reads raw input from `stdin` via `getline()` and tokenizes it using a character-by-character state machine, safely handling quote escapes and whitespaces.
- **Execution (`executable.c` / `builtins.c`)**: Checks if the command is a builtin. If not, it searches `$PATH`, forks a child process, and executes via `execv()`.
- **Process Management (`process.c`)**: Forks a child process, executes an external program with `execv()`, and waits for the child to finish with `waitpid()`.
- **Redirection (`redirection.c`)**: Scans tokens for redirection operators, creates backups of file descriptors with `dup()`, opens target files via `<fcntl.h>`, and overwrites active file descriptors with `dup2()`.
- **Pipelines (`pipeline.c`)**: Detects pipe operators, creates a pipe with `pipe()`, forks two child processes, and wires them together using `dup2()` on the pipe's read and write ends.
