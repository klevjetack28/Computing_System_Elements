// Standard Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

// Assembler Headers
#include "../include/hashmap.h"

void remove_preceding_spaces(char* str) {
    int i = 0, j = 0;
    while (isspace(str[i])) {
        i++;
    }
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

bool isSymbol(char* str)
{
    bool symbol = true;

    if ((str[0] >= '0' && str[0] <= '9'))
    {
        return !symbol;
    }

    return symbol;
}

char* int_to_binary(uint16_t num)
{
    int bits = 16;
    char* binary = malloc(bits + 1);

    int n = num;
    for (int i = 0; i < bits; i++)
    {
        uint16_t bit = n % 2;
        binary[bits - i - 1] = '0' + bit;
        n /= 2;
    }
    binary[bits] = '\0';
    return binary;
}

void writeInstruction(FILE* machine_file, int16_t instruction)
{
    char* binary = int_to_binary(instruction);
    fprintf(machine_file, "%s\n", binary);
    free(binary);
}

int16_t aInstruction(char* num)
{
    return atoi(num);
 }

char* instructionCoder(char* instruction, char prefix)
{
    size_t len = strlen(instruction);
    char* out = malloc(len + 2);
    out[0] = prefix;
    int i;
    for (i = 1; i < len + 1; i++)
    {
        if ((instruction[i - 1] == '\0') || (instruction[i - 1] == '\n'))
        {
            break;
        }
        out[i] = instruction[i - 1];
    }
    out[i] = '\0';
    return out;
}



int16_t cInstruction(hashmap_t* instructions, char* comp, char* dest, char* jump)
{
    uint16_t instruction = 0;
    instruction |= 0b111 << 13; // The first bit is a 1 and the next 2 bits are not used.

    size_t compBits = search(instructions, comp);
    instruction |= compBits << 6; // 7 bits 1 bit is a next 6 specify the instruction.
    
    size_t destBits = search(instructions, dest);
    instruction |= destBits << 3; // 3 bits to specify the destination

    size_t jumpBits = search(instructions, jump);
    instruction |= jumpBits; // 3 bits to specify the jump instruction
    return instruction;
}

void initialization(hashmap_t** map)
{
    initializeHashMap(*map, 0, 8);
    int registers = 16;
    for (int i = 0; i < registers; i++)
    {
        char* registerName;
        sprintf(registerName, "R%d", i);
        insert(map, registerName, i);
    }
   
    insert(map, "SP", 256);
    insert(map, "SCREEN", 16384);
    insert(map, "KBD", 24576);
}

void initializeInstr(hashmap_t** instructions)
{
    initializeHashMap(*instructions, 0, 8);
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

void first_pass(hashmap_t** symbolTable, char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("ERROR: Count not open file {%s}.\n", file_path);
        return;
    }
    
    int lineCount = -1;
    char* str = NULL;
    size_t len;
    
    while(getline(&str, &len, file) != -1)
    {
        char* line = str;
        remove_preceding_spaces(line);
        char startLine = line[0];

        switch (startLine)
        {
            case '(': {
                line++;
                char* symbol = malloc(len);
                int size = 0;
                while ((startLine = *line++) != ')')
                {
                    symbol[size++] = startLine;
                }
                symbol[size] = '\0';

                int val = search(*symbolTable, symbol);
                if (val == -1)
                {
                    // TODO - Create Labels table symbolTable is your RAM labels are just constants.
                    insert(symbolTable, symbol, lineCount + 1);
                }
               
                free(symbol);
                break;
            }
            case '@':
            case 'A':
            case 'D':
            case 'M':
            case '0':
            case '1': {
                lineCount++;
                break;
            }
        }
    }
    free(str);
    fclose(file);
}

void second_pass(hashmap_t** symbolTable, hashmap_t* instructions, char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("ERROR: Could not open file {%s}.\n", file_path);
        return;
    }

    // Get the name of the file - "asm" text
    char* machine_path = malloc(strlen(file_path) + 2); // + 1 for '\0' + 1 for 4th letter in hack
    strcpy(machine_path, file_path);
    machine_path[strlen(file_path) - 3] = '\0';
    
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

    int lineCount = -1;
    
    char* str = NULL;
    size_t len = 0;

    while (getline(&str, &len, file) != -1)
    {
        char* line = str;
        remove_preceding_spaces(line);
        char startLine = line[0];

        // Parsed Symbols
        char* comp = NULL; 
        char* dest = NULL;
        char* jump = NULL;
        char* token = calloc(32, sizeof(char)); // Temp symbol
        
        switch (startLine)
        {
            case '@': { 
                line++;
                char* variable = malloc(len);
                int i = 0;
                while ((startLine = *line++) != '\n')
                {
                    variable[i++] = startLine;
                }
                variable[i] = '\0';
                if (isSymbol(variable))
                {
                    int val = search(*symbolTable, variable);
                    if (val == -1)
                    {
                        printf("%s, %d\n", variable, (*symbolTable)->numNodes - 3);
                        insert(symbolTable, variable, (*symbolTable)->numNodes - 3);
                    }
                    sprintf(variable, "%d", val);
                }
                int16_t instruction = aInstruction(variable);
                writeInstruction(machine_file, instruction);

                free(variable);
                lineCount++;
                break;
            }
            case '0':
            case '1':
            case 'A':
            case 'D':
            case 'M': {
                bool jmpInstr = false;

                int i = 0;
                while((startLine = *line++) != '\0')
                {
                    // A dest instruction
                    if (startLine == '=')
                    {
                        jmpInstr |= false;
                        dest = instructionCoder(token, 'd');
                        i = 0;
                    }

                    // A jump instruction
                    else if (startLine == ';')
                    {
                        jmpInstr |= true;
                        comp = instructionCoder(token, 'c');
                        i = 0;
                    } 

                    else if (startLine != '\n')
                    {
                        // Append next char to token
                        token[i++] = startLine;
                    }
                }
                // If jump instruction rvalue = jump
                if (jmpInstr)
                {
                    jump = instructionCoder(token, 'j');
                    dest = instructionCoder("null", 'd');
                }
                // Else rvalue = comp
                else 
                {
                    comp = instructionCoder(token, 'c');
                    jump = instructionCoder("null", 'j');
                }

                int16_t instruction = cInstruction(instructions, comp, dest, jump);
                writeInstruction(machine_file, instruction);
                lineCount++;
                break;
            }
        }
        free(comp);
        free(dest);
        free(jump);
        free(token);
    }
    free(str);
    free(machine_path);
    fclose(file);
    fclose(machine_file);
    return;
}


int main(int argc, char *argv[])
{
    char* file_path = argv[1];

    hashmap_t* symbolTable = malloc(sizeof *symbolTable);
    initialization(&symbolTable);

    hashmap_t* instr = malloc(sizeof *instr);
    // Initialize generic symbols
    initializeInstr(&instr);

    // Initialize program symbols
    first_pass(&symbolTable, file_path);
    
    // Translate parsed assembly into machine code.
    second_pass(&symbolTable, instr, file_path);
    
    freeHashMap(symbolTable);
    freeHashMap(instr);
    return 0;
}
