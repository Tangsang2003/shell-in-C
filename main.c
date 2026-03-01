#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

void shell_loop();
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
        // Split line
        // Execute
        // See status
        // Free line
        // Free args
        loop++;
        
    } while (loop < 5);   
}