#include <sys/types.h> // pid_t
#include <unistd.h> // pid_t, fork, exec
#include <sys/wait.h> // WIFEXITED, Status Macros

#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>

#define SHELL_LINE_BUFFER_SIZE 1024
#define SHELL_TOKENS_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\n\a\r"

void shell_loop();
char *shell_read_line();
char **shell_tokenize_command(char*);

int main(int argc, char **argv) {
    // Initialize through configuration files if any

    shell_loop();

    // Perform cleanup.
    // Terminate
    return EXIT_SUCCESS;
}

void shell_loop() {
    char *line;
    char **args;
    int loop = 0;
    do {
        printf(">");
        // Get line
        char *line = shell_read_line();
        if (!line) {
            break;
        }
        printf("You entered: %s", line);
        
        // Split line

        char **args = shell_tokenize_command(line);
        if (args) {
            for (int i = 0; args[i] != NULL; i++) {
                printf("(%d) (%s) ", i + 1, args[i]);
            }
        }

        free(line);
        free(args);
        // Execute
        // See status
        // Free line
        // Free args
        loop++;
        
    } while (loop < 5);   
}

/* Function to read the user command*/
/* This function can be implemented with the stdio.h getline() function too. */
char *shell_read_line(void) {
    int character = 0; // for getchar()
    int buffer_size = SHELL_LINE_BUFFER_SIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffer_size); // our buffer

    if (!buffer) {
        fprintf(stderr, "Shell: Allocation Error!");
        exit(EXIT_FAILURE);
    }

    /*
    1. Read a character, if it is EOF or null char, return the buffer.
    2. Else, put the character to the buffer at its position.
    3. Increment the position, and check if the buffer can accomodate the next
       incoming character.
    4. If yes, do nothing, else reallocate and rerun the loop.
    **NOTE: We use a new_buffer to reallocate because if reallocation fails, it
            returns NULL but the original buffer is still allocated. So if we 
            overwrite, there is a memory leak.
    */
    while (1) {
        character = getchar();
        // EOF is an integer
        if (character == EOF || character == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        else {
            buffer[position] = character;
            position++;
            if (position >= buffer_size) {
                buffer_size += SHELL_LINE_BUFFER_SIZE;
                char *new_buffer = realloc(buffer, buffer_size);
                if (!new_buffer){
                    free(buffer);
                    fprintf(stderr, "Shell: Allocation Error!");
                    exit(EXIT_FAILURE);
                }
                buffer = new_buffer;
            }
        }
    }
}

/*
Function to tokenize the read line/command.
Takes in the read string.
Tokenizes and returns array of pointers to the tokenized tokens.
*/
char **shell_tokenize_command(char* command) {
    int position = 0, buffer_size = SHELL_TOKENS_BUFFER_SIZE;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Shell: Allocation Error!");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, TOKEN_DELIMITERS);

    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= buffer_size) {
            buffer_size += SHELL_TOKENS_BUFFER_SIZE;
            char ** new_tokens = realloc(tokens, buffer_size * sizeof(char*));

            if (!new_tokens) {
                free(tokens);
                fprintf(stderr, "Shell: Allocation Error!");
                exit(EXIT_FAILURE);
            }

            tokens = new_tokens;
        }

        token = strtok(NULL, TOKEN_DELIMITERS);      
    }
    tokens[position] = NULL;
    return tokens;
}

/*
Shell launch function
*/
int shell_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();

    // Child process
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            /*
            Child never returns, so must be error because if execvp is successful
            the process has been transformed to a different program
            */
            perror("Shell");
            exit(EXIT_FAILURE);
        }
    }
    // If fork() failed
    else if (pid < 0)
    {
        perror("Shell");
    }

    // Parent process
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
