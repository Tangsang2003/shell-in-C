# Shell in C
This is an impementation of a simple shell built from scratch in C.

## Key Concepts
- Dynaimc buffer growth for arbitrary-length input.
- Safe `realloc`, to prevent memory leak on allocation failure.
- Tokenization of user input line with `strtok()` and dynamic token array.
- Process life cycle and process execution, usage of `fork()`, `exec()`, `waitpid()` system calls and macros.
- Shell built-ins (`cd`, `exit`, `help`).
- To see more explanations and details, see [Notes.md](./docs/Notes.md).

## Build & Run
```bash
make
./shell
```

## Limitations
- No quote  or backslash escaping in parsing
- Tokenization splits only on whitespace, no support for quoted arguments with spaces.
- Very few standard builtins
- No globbing
- No piping or redirection
