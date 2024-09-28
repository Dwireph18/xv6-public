#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For strdup and free

#define MAX_LINE_LEN 1024
#define MAX_DEFINITIONS 100
void preprocess(FILE *file, char *vars[], char *vals[], int count) {
    char line[MAX_LINE_LEN];

    while (fgets(line, MAX_LINE_LEN, file)) {
        char buffer[MAX_LINE_LEN]; // Buffer to store the modified line
        int bufferIndex = 0;
        char *pos = line; // Pointer to traverse the line
        char *lastPos = line; // Pointer to the last copied position
        int substitutionsMade = 0; // Flag to track substitutions

        while (*pos) { // While we haven't reached the end of the line
            int found = 0;

            // Check each variable definition
            for (int i = 0; i < count; i++) {
                // Check if the current position matches a variable
                if (strncmp(pos, vars[i], strlen(vars[i])) == 0) {
                    // Copy the part of the line before the variable (from lastPos to pos)
                    strncpy(buffer + bufferIndex, lastPos, pos - lastPos);
                    bufferIndex += pos - lastPos;

                    // Append the value corresponding to the variable
                    strcpy(buffer + bufferIndex, vals[i]);
                    bufferIndex += strlen(vals[i]);

                    // Move the pointer past the variable in the original line
                    pos += strlen(vars[i]);
                    lastPos = pos; // Update lastPos to the end of the variable match
                    found = 1;
                    substitutionsMade = 1; // Set flag to indicate a substitution
                    break; // Exit the loop after replacement
                }
            }

            // If no variable was found, move to the next character
            if (!found) {
                pos++;
            }
        }

        // Copy any remaining part of the line after the last variable
        strncpy(buffer + bufferIndex, lastPos, pos - lastPos);
        bufferIndex += pos - lastPos;

        // Null-terminate the buffer
        buffer[bufferIndex] = '\0';

        // Print the modified line only if substitutions were made
        if (substitutionsMade) {
            printf("%s\n", buffer);
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file> -Dvar1=value1 -Dvar2=value2...\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char *vars[MAX_DEFINITIONS];
    char *vals[MAX_DEFINITIONS];
    int count = 0;

    // Parse command-line arguments for -Dvar=value
    for (int i = 2; i < argc; i++) {
        if (strncmp(argv[i], "-D", 2) == 0) {
            char *arg_copy = strdup(argv[i]);
            char *eq = strchr(arg_copy, '=');
            if (eq) {
                *eq = '\0'; // Split variable and value
                vars[count] = strdup(arg_copy + 2); // Variable name
                vals[count] = strdup(eq + 1); // Value
                count++;
            }
            free(arg_copy); // Free the duplicated string
        }
    }

    // Perform variable substitution
    preprocess(file, vars, vals, count);
    
    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(vars[i]);
        free(vals[i]);
    }

    fclose(file);
    return 0;
}
