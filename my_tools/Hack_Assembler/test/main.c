// Standard Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Assembler Headers
#include "hashmap/hashmap.h"
/*
void initialization(hashmap_t* map)
{
    int registers = 16;
    for (int i = 0; i < registers; i++)
    {
        char* registerName;
        sprintf(registerName, "R%d", i);
        insert(map, registerName, i);
    }
    
    insert(map, "SCREEN", 16384);
    insert(map, "KBD", 24576);

    return;
}
*/
void basic_assembler(hashmap_t* map, char* file_path, int len_path)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("ERROR: Could not open file {%s}.", file_path);
        return;
    }

    // Get the name of the file - "asm" text
    char* machine_path;
    strcpy(machine_path, file_path, len_path - 3);
    
    // Concat the machine code path with the "hack" file extension
    char* hackf_ext = "hack";
    strcat(machine_path, hackf_ext);

    FILE* machine_file = fopen(machine_path, "w");
    if (machine_file == NULL)
    {
        printf("ERROR: Could not create file {%s}.", machine_path);
        return;
    }

    int lineAsm = 0, lineHack = 0;
    
    char* line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        char startLine = line[0];

        switch (startLine)
        {
            case '/':
                // Confirm if start of line is comment
                if (line[1] != '/')
                {
                    // If not start of comment
                    printf("ERROR: Syntax not recognized. Like %d", lineAsm);
                    return;
                }
                break;
            case '@':
                /* Old parenthesis code will reuse
                size_t size = 0;
                while ((startLine = *line++) != ')')
                {
                    size++;
                }
                char* symbol;
                strcpy(symbol, size + 1, line);
                insert(map, symbol, lineHack + 1);
                */
                break;
            case '\n':
                break;
            default:
                fprintf("%s", line);
                lineCount++;
        }

    }
        fclose(file);
        fclose(tmp_file);
}


int main(int argc, char *argv[])
{
    int len_path = strlen(argv[1]);
    char* file_path = (char*)malloc(len_path * sizeof(char));
    strcpy(file_path, argv[1]);

    hashmap_t map;
    //initializtion(map);

    basic_assembler(map, file_path, int len_path);
    // second_pass();

    return 0;
}
