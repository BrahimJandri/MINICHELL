# MINICHELL

<div align="center">
  <img src="minishelle.png" alt="Minishell Logo" width="400"/>
</div>

MINICHELL is a lightweight Unix shell implementation written in C. This project recreates the functionality of a bash-like command-line interpreter, providing essential shell features including command execution, built-in commands, I/O redirection, pipes, signal handling, and environment variable support.

This educational project demonstrates the internals of shell parsing, process control, and command execution in a compact and readable codebase.

---

## Table of Contents

- [Features](#features)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Build](#build)
- [Usage](#usage)
- [Examples](#examples)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)
- [Contributing](#contributing)
- [Authors](#authors)

---

## Features

### Built-in Commands
- `cd` - Change directory with relative or absolute path
- `echo` - Display text with `-n` option support
- `pwd` - Print current working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display all environment variables
- `exit` - Exit the shell with optional exit status

### Shell Features
- Execute external commands using PATH resolution
- Input redirection (`<`)
- Output redirection (`>` and `>>`)
- Here-documents (`<<`)
- Pipes (`|`) for command chaining
- Environment variable expansion (`$VAR`)
- Quote handling (single `'` and double `"`)
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)

---

## Dependencies

### Required
- **POSIX-compliant system** (Linux, macOS, or WSL on Windows)
- **GCC** or compatible C compiler (C99 or later)
- **GNU Make**
- **readline library** - for line editing and command history

### Installing readline

**On Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libreadline-dev
```

**On macOS:**
```bash
brew install readline
```

**On Fedora/RHEL:**
```bash
sudo dnf install readline-devel
```

---

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/BrahimJandri/MINICHELL.git
   cd MINICHELL
   ```

2. Ensure readline is installed (see [Dependencies](#dependencies))

3. Build the project:
   ```bash
   make
   ```

---

## Build

### Build Commands

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Rebuild the entire project
```

The build process will:
1. Compile the Libft library
2. Compile all source files in `Parsing/`, `Execution/`, and root directory
3. Link everything into the `minishell` executable

---

## Usage

Start the shell:
```bash
./minishell
```

You'll see a prompt where you can enter commands:

```bash
$ ls -la
$ echo "Hello, World!"
$ cd /tmp
$ pwd
$ export MY_VAR=hello
$ echo $MY_VAR
$ cat file.txt | grep "pattern" > output.txt
```

Exit the shell:
```bash
$ exit
```
or press `Ctrl+D`

---

## Examples

### Basic Commands
```bash
$ echo Hello World
Hello World

$ pwd
/home/user/MINICHELL

$ cd ..
$ pwd
/home/user
```

### Redirections
```bash
# Output redirection
$ echo "Hello" > file.txt
$ cat file.txt
Hello

# Append redirection
$ echo "World" >> file.txt
$ cat file.txt
Hello
World

# Input redirection
$ cat < file.txt
Hello
World
```

### Pipes
```bash
$ ls -l | grep minishell
-rwxr-xr-x 1 user user 45678 Oct 27 10:00 minishell

$ cat file.txt | wc -l
2
```

### Here-documents
```bash
$ cat << EOF
> First line
> Second line
> EOF
First line
Second line
```

### Environment Variables
```bash
$ export NAME=John
$ echo "Hello $NAME"
Hello John

$ env | grep NAME
NAME=John

$ unset NAME
$ echo "Hello $NAME"
Hello
```

---

## Project Structure

```
MINICHELL/
├── Parsing/              # Lexical analysis and parsing
│   ├── lexer.c           # Tokenization
│   ├── ft_nodes.c        # AST node creation
│   ├── ft_parsing.c      # Parser implementation
│   ├── ft_expander.c     # Variable expansion
│   ├── ft_tokenizer.c    # Token handling
│   ├── shell_loop.c      # Main shell loop
│   └── ...               # Helper files
├── Execution/            # Command execution
│   ├── ft_execution.c    # Main execution logic
│   ├── ft_heredoc.c      # Here-document handling
│   ├── redirection.c     # I/O redirection
│   ├── signals.c         # Signal handling
│   ├── builtins/         # Built-in commands
│   │   ├── ft_cd.c
│   │   ├── ft_echo.c
│   │   ├── ft_env.c
│   │   ├── ft_exit.c
│   │   ├── ft_export.c
│   │   ├── ft_pwd.c
│   │   └── ft_unset.c
│   └── ...               # Helper files
├── Libft/                # Custom C library
│   └── ...               # String and memory functions
├── include/              # Header files
│   ├── minishell.h       # Main header
│   ├── brahim.h          # Additional definitions
│   └── rachid.h          # Additional definitions
├── minishell.c           # Main entry point
├── Makefile              # Build configuration
├── readline.supp         # Valgrind suppression file
└── README.md             # This file
```

---

## Implementation Details

### Parsing Pipeline
1. **Lexical Analysis**: Input is tokenized into words, operators, and special characters
2. **Token Classification**: Tokens are classified (commands, arguments, redirections, pipes)
3. **Variable Expansion**: Environment variables are expanded (e.g., `$PATH`)
4. **AST Construction**: Tokens are organized into an abstract syntax tree
5. **Validation**: Syntax is validated before execution

### Execution Flow
1. **Command Resolution**: Built-in vs external command determination
2. **Process Management**: `fork()` for external commands
3. **Redirection Setup**: File descriptors are configured before execution
4. **Pipeline Handling**: Multiple commands connected via pipes
5. **Signal Management**: Proper handling of SIGINT, SIGQUIT, and SIGTERM

### Memory Management
- All allocated memory is properly freed
- Valgrind-clean (with readline suppressions)
- No memory leaks in normal operation

---

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please ensure your code:
- Compiles without warnings with `-Wall -Werror -Wextra`
- Follows the existing code style
- Is properly documented

---

## Authors

- **Brahim Jandri** - [@BrahimJandri](https://github.com/BrahimJandri)
- Additional contributors listed in the commit history

---

<div align="center">
  Made with ❤️ for learning shell internals
</div>
