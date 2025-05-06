// Standard Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Assembler Headers
#include "hashmap/hashmap.h"

int16_t aInstruction(char* num)
{
    return atoi(num);
 }

int16_t cInstruction(hashmap_t* instructions, char* comp, char* dest, char* jump)
{
    int16_t instruction = 0;

    instruction |= 0b111 << 3; // The first bit is a 1 and the next 2 bits are not used.

    char* instrC = "c";
    strcat(instrC, comp);
    instruction |= search(instructions, instrC) << 7; // 7 bits 1 bit is a next 6 specify the instruction.

    char* instrD = "d";
    strcat(instrD, dest);
    instruction |= search(instructions, instrD) << 3; // 3 bits to specify the destination

    char* instrJ = "j";
    strcat(instrJ, jump);
    instruction |= search(instructions, instrJ) << 3; // 3 bits to specify the jump instruction

    printf("%d", instruction);
    return instruction;
}

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

void initializeInstr(hashmap_t* instructions)
{
    insert(instructions, "c0", 0b0101010);
    insert(instructions, "c1", 0b0111111);
    insert(instructions, "c-1", 0b0111010);
    insert(instructions, "cD", 0b0001100);
    insert(instructions, "cA", 0b0110000);
    insert(instructions, "c!D", 0b0001101);
    insert(instructions, "c!A", 0b0001111);
    insert(instructions, "c-D", 0b0110011);
    insert(instructions, "c-A", 0b0011111);
    insert(instructions, "cD+1", 0b0110111);
    insert(instructions, "cA+1", 0b0001110);
    insert(instructions, "cD-1", 0b0110010);
    insert(instructions, "cA-1", 0b0000010);
    insert(instructions, "cD+A", 0b0000010);
    insert(instructions, "cD-A", 0b0010011);
    insert(instructions, "cA-D", 0b0000111);
    insert(instructions, "cD&A", 0b0000000);
    insert(instructions, "cD|A", 0b0010101);
    insert(instructions, "cM", 0b1110000);
    insert(instructions, "c!M", 0b1110001);
    insert(instructions, "c-M", 0b1110011);
    insert(instructions, "cM+1", 0b1110111);
    insert(instructions, "cM-1", 0b1110010);
    insert(instructions, "cD+M", 0b1000010);
    insert(instructions, "cD-M", 0b1010011);
    insert(instructions, "cM-D", 0b1000111);
    insert(instructions, "cD&M", 0b1000000);
    insert(instructions, "cD|M", 0b1010101);
    insert(instructions, "dnull", 0b000);
    insert(instructions, "dM", 0b001);
    insert(instructions, "dD", 0b010);
    insert(instructions, "dDM", 0b011);
    insert(instructions, "dA", 0b100);
    insert(instructions, "dAM", 0b101);
    insert(instructions, "dAD", 0b110);
    insert(instructions, "dADM", 0b111);
    insert(instructions, "jnull", 0b000);
    insert(instructions, "jJGT", 0b001);
    insert(instructions, "jJEQ", 0b010);
    insert(instructions, "jJGE", 0b011);
    insert(instructions, "jJLT", 0b100);
    insert(instructions, "jJNE", 0b101);
    insert(instructions, "jJLE", 0b110);
    insert(instructions, "jJMP", 0b111);
}

void basic_assembler(hashmap_t* instructions, hashmap_t* map, char* file_path, int len_path)
{
    printf("Starting assembler...\n");

    FILE* file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("ERROR: Could not open file {%s}.\n", file_path);
        return;
    }

    // Get the name of the file - "asm" text
    char machine_path[len_path];
    strcpy(machine_path, file_path);
    machine_path[len_path - 3] = '\0';
    
    // Concat the machine code path with the "hack" file extension
    char* hackf_ext = "hack";
    strcat(machine_path, hackf_ext);
    printf("Machine path: %s\n", machine_path);

    FILE* machine_file = fopen(machine_path, "w");
    if (machine_file == NULL)
    {
        printf("ERROR: Could not create file {%s}.", machine_path);
        return;
    }

    int lineCount = 0;
    
    char* line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        char startLine = line[0];

        switch (startLine)
        {
            case '@': { 
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
                char num[len - 1];
                int i = 0;
                while ((startLine = *++line) != '\0')
                {
                    num[i++] = startLine;
                }
                num[len - 1] = '\0';
                
                aInstruction(num);

                lineCount++;        
                break;
            }
            case 'A':
            case 'D':
            case 'M': {
                bool jmpInstr = false;
                char comp[5], dest[5], jump[5]; // Parsed Symbols

                char token[5]; // Temp symbol
                int i = 0;
                while((startLine = *line++) != '\0')
                {
                    // Append next char to token
                    token[i++] = startLine;

                    // A dest instruction
                    if (startLine == '=')
                    {
                        jmpInstr = false;
                        strcpy(dest, token);
                        dest[4] = '\0';
                        i = 0;
                    }

                    // A jump instruction
                    else if (startLine == ';')
                    {
                        jmpInstr = true;
                        strcpy(comp, token);
                        comp[4] = '\0';
                        i = 0;
                    }
                }
                // If jump instruction rvalue = jump
                if (jmpInstr)
                {
                    strcpy(jump, token);
                    jump[4] = '\0';
                }
                // Else rvalue = comp
                else 
                {
                    strcpy(comp, token);
                    comp[4] = '\0';
                }

                cInstruction(map, comp, dest, jump);

                lineCount++;
                break;
            }
        }
    }
        fclose(file);
        fclose(machine_file);
}


int main(int argc, char *argv[])
{
    int len_path = strlen(argv[1]);
    char* file_path = (char*)malloc(len_path * sizeof(char));
    strcpy(file_path, argv[1]);

    hashmap_t* map;
    //initializtion(map);

    hashmap_t* instr;
    initializeInstr(instr);

    basic_assembler(instr, map, file_path, len_path);
    // second_pass();

    return 0;
}
