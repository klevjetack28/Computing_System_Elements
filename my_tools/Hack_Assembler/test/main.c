// Standard Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Assembler Headers
#include "tokenizer.h"

struct cpu
{
    short registers[16];
};

int main(int argc, char *argv[])
{
    // Steps
    initializtion();
    // first_pass();
    // second_pass();

    // Getting path to file
    int length_path = strlen(argv[1]);
    char *file_path = (char *)malloc(length_path * sizeof(char));
    strcpy(file_path, argv[1]);

    // Checking file path is valid and loading it
    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("ERROR: Could not find file %s.", file_path);
    }

    // Defining variables for both passes
    char *line;
    size_t len = 0;
    ssize_t read;

    // First pass of the .asm file
    while ((read = getline(&line, &len, file)) != -1)
    {
        nextLine(line, len);
    }

    rewind(file);

    // Second pass of the .asm file
    while ((read = getline(&line, &len, file)) != -1)
    {
        nextLine(line, len);
    }


    return 0;
}
