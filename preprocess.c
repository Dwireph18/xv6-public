#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_DEFINITIONS 100

void preprocess(FILE *file, char *vars[], char *vals[], int count) {
    char line[MAX_LINE_LEN];

    while (fgets(line, MAX_LINE_LEN, file)) {
        char buffer[MAX_LINE_LEN]; // Buffer to store the modified line
        int bufferIndex = 0;
        char *pos = line; // Pointer to traverse the line

        while (*pos) { // While we haven't reached the end of the line
            int found = 0;

            for (int i = 0; i < count; i++) {
                // Check if the current position matches a variable
                if (strncmp(pos, vars[i], strlen(vars[i])) == 0) {
                    // If found, copy part of the line before the variable
                    strncpy(buffer + bufferIndex, line, pos - line);
                    bufferIndex += pos - line;

                    // Append the value
                    strcpy(buffer + bufferIndex, vals[i]);
                    bufferIndex += strlen(vals[i]);

                    // Move past the variable in the original line
                    pos += strlen(vars[i]);
                    found = 1;
                    break; // Exit the loop after replacement
                }
            }

            if (!found) {
                // If no variable was found, copy the current character
                buffer[bufferIndex++] = *pos++;
            }
        }

        // Null-terminate the buffer
        buffer[bufferIndex] = '\0';

        // Print the modified line
        printf("%s", buffer);
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
            char *eq = strchr(argv[i], '=');
            if (eq) {
                *eq = '\0';
                vars[count] = argv[i] + 2; // Variable name
                vals[count] = eq + 1; // Value
                count++;
            }
        }
    }

    // Perform variable substitution
    preprocess(file, vars, vals, count);
    fclose(file);
    return 0;
}

