# Shell in C
Three things that a shell does in its lifetime:
- Initialize itself through configuration files.
- Read and execute commands through stdin.
- Run shutdown commands, free memory, terminate.

The implementation of this shell is simple, and won't have any configuration files and shutdown command. So, a simple loop and terminate.

## What our loop does?
- Read the command.
- Parse the command into program string and arguments.
- Execute.

### Reading a line
- To handle any length of user command, we need to dynamically allocate the buffer/block.
- The function defined can be implemented with the `stdio.h` `get_line()` function too. 
- But, we have implemented it following the steps:
    - Read a character, if it is EOF or null char, return the buffer.
    - Else, put the character to the buffer at its position.
    - Increment the position, and check if the buffer can accomodate the next
       incoming character.
    - If yes, do nothing, else reallocate and rerun the loop.

```NOTE: We use a new_buffer to reallocate because if reallocation fails, it returns NULL but the original buffer is still allocated. So if we overwrite, there is a memory leak. ```

### Parsing the line
- We've simplified parsing of line to separate commands and arguments based simply on spaces or tabs. We've not allowed usage of double quotes or backslash escaping.
- Very similar implementation to readine the line with dynamic allocation.
- Used `strtok()` from string.h

### Starting Processes
- There are mainly two ways to start a process in Unix.
- First is: when its kernel is loaded during computer boot, one process called Init is started, which runs for the entire length of time the computer is on. This process is responsible for loading all other processes. But, we aren't making Init. So:
- We use `fork()` system call. When we call this function, it creates a duplicate of the process. It returns 0 to the child process, and returns the process ID (PID) of the child to the parent process. So, this is the second way, the only way for the process to start is by existing in on another already existing process, duplicating itself. And, we use the `exec()` system call, which replaces the current running program and replace the current running program with a new one. This means, when we call `exec()`, the OS stops the current process, loads up the new program and run in its place. A process never returns from the `exec()` unless there is an error.
- This way, using the `fork()` call, we duplicate the parent, and it can keep running and doing its thing, and using `exec()`, we replace the child with a new program. Using `wait()`, we can further keep tabs on the child processes.
- In our implementation, we have used `execvp()` version of `exec()`, which expects a program name, and an array (vector) `v` in the name, and `p` means that instead of providing full path for the program, we give the program name and the OS runs the program from the path. 
- We use perror to print errors that OS decide. When a system call fails, the OS doesn't just return -1, it also sets a global variable called `errno`. e.g., `errno = 2` means  "File not found". `perror` automatically follows a standard format: `[String]: [Error Message]`. So in our case, `"Shell: Err message"`
- In our parent processes' do while loop, `WUNTRACED` is an option flag. It tells `waitpid` to return not just if the child dies, but also if the child stops (for example if user presses `Ctrl + Z`.)
- We are doing the `do while` loop because even if the `waitpid` waits, because in UNIX, a process can change state multiple times. For eg., it could be stopped (paused) and then later resumed or killed. The loop ensures that the parent only stops waiting when the child is truly finished.

### Builtins
- Most of the commands that shells execute are programs but not all of them. Some are built-in the shell.
- For example, if we want to change a directory, we need to use function `chdir()`. But current directory is property of a process. So, if we wrote a program called `cd` that changed directory, it would just change its own current directory, and then terminate. Its parent process's current directory wouldn't be changed. So, the shell process itself needs to execute `chdir()` so that when it launches child processes, they will inherit that directory too.
- Similar is for program called `exit`. These types of commands need to be built into the shell. Also shells are configured using shell configuration scripts like `~/.bashrc`. Those type of scripts use commands that change the operation of the shell. These commands could only change the operation of the shell if they were implemented within the shell process itself. So, these are builtins, the commands taht we add to the shell itself. In our shell, we've added `cd`, `exit`, and `help`.
- So, we've first created the builtinis prototypes and then created an array of strings, which stores the available builtins.
- Then, we create a function pointer array that points to the addresses of the built-ins functions.
- Then, we implement each of the builtins.

## Final putting together
- We have a main shell loop that runs until the return status is 0.
- We have a `shell_execute` function, that executes either one of the builtins or starts any program with its arguments.
- Then, finally, we free up the allocated memory.

### Some builtins I would like to add/self-implement:

- `History`: `history` everytime a user enters a command, save it to a list (an array of strings). This built-in would print that list
- `Helpful Status`: `status`: prints how many processes you've spawned so far or the PID of the current shell.

