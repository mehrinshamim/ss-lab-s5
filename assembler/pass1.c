#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // Variables to store label, opcode, operand
    char label[20], opcode[20], operand[20];
    
    // Variables for reading from the opcode table
    char optab_opcode[20], optab_value[20];
    
    // Location counter, program starting address, program length
    int locctr, start, lengthy;
    
    // File pointers for input assembly, symbol table, opcode table, intermediate file, length file
    FILE *input, *symtab, *optab, *intermediate, *length;

    // Open files
    input = fopen("input.txt", "r");        // Assembly source program
    symtab = fopen("symtab.txt", "w");      // Symbol table to be created
    optab = fopen("optab.txt", "r");        // Opcode table containing opcode and machine code
    intermediate = fopen("intermediate.txt", "w"); // Intermediate file with addresses
    length = fopen("length.txt", "w");      // File to store program length

    // Check if all files opened successfully
    if (!input || !symtab || !optab || !intermediate || !length)
    {
        printf("Error opening files\n");
        return 1;
    }

    // Read first line of input (usually START directive)
    fscanf(input, "%s %s %s", label, opcode, operand);

    // If START directive is present, initialize location counter to operand value
    if (strcmp(opcode, "START") == 0)
    {
        locctr = (int)strtol(operand, NULL, 16); // Convert hex string to integer
        start = locctr;                          // Save starting address
    }
    else
    {
        locctr = 0;  // If no START, start from 0
        start = 0;
    }

    // Write first line to intermediate file
    fprintf(intermediate, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Read next line of input
    fscanf(input, "%s %s %s", label, opcode, operand);

    // Process each line until END directive
    while (strcmp(opcode, "END") != 0)
    {
        // Write current line with location counter to intermediate file
        fprintf(intermediate, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // If label exists, write it to symbol table with current address
        if (strcmp(label, "~") != 0)
        {
            fprintf(symtab, "%s\t%X\n", label, locctr);
        }

        // Check opcode table for matching opcode
        rewind(optab); // Go back to beginning of optab
        int found = 0;
        while (fscanf(optab, "%s %s", optab_opcode, optab_value) == 2)
        {
            if (strcmp(optab_opcode, opcode) == 0)
            {
                locctr += 3; // All machine instructions assumed to be 3 bytes
                found = 1;
                break;
            }
        }

        // If opcode not found in optab, check for assembler directives
        if (found == 0)
        {
            if (strcmp(opcode, "WORD") == 0)
            {
                locctr += 3; // WORD is 3 bytes
            }
            else if (strcmp(opcode, "RESW") == 0)
            {
                locctr += 3 * atoi(operand); // Reserve n words (3 bytes each)
            }
            else if (strcmp(opcode, "RESB") == 0)
            {
                locctr += atoi(operand); // Reserve n bytes
            }
            else if (strcmp(opcode, "BYTE") == 0)
            {
                if (operand[0] == 'C') // Character constant
                {
                    locctr += (strlen(operand) - 3); // Length minus C' '
                }
                else if (operand[0] == 'X') // Hex constant
                {
                    locctr += (strlen(operand) - 3) / 2; // 2 hex digits per byte
                }
            }
        }

        // Read next line
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    // Write END line to intermediate file
    fprintf(intermediate, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Compute program length and write to length file
    lengthy = locctr - start;
    fprintf(length, "%X", lengthy);

    // Close all files
    fclose(input);
    fclose(symtab);
    fclose(optab);
    fclose(intermediate);
    fclose(length);

    return 0;
}
