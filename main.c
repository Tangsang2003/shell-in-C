#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

#define SHELL_LINE_BUFFER_SIZE 1024

void shell_loop();
char *shell_read_line();

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
        char *readline = shell_read_line();
        if (readline) {
            printf("You entered: %s\n", readline);
        }
        while (*readline != '\0') {
            printf("%c", *readline);
            readline++;
        }
        
        // Split line
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