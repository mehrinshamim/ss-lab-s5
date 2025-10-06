#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Function to convert an integer to hexadecimal string
void intToHex(int num, char number[]) {
    if (num == 0) {
        strcpy(number, "00");
        return;
    }

    char tempB[20];
    int i = 0;

    // Convert integer to hex manually
    while (num != 0) {
        int temp = num % 16;
        tempB[i++] = (temp < 10) ? temp + '0' : temp + 'A' - 10;
        num /= 16;
    }

    int index = 0;
    for (int j = i - 1; j >= 0; j--) number[index++] = tempB[j];
    number[index] = '\0';
}

int main() {
    char loc[10], opcode[10], label[10], operand[10];
    char mnemonic[10], code[10], addr[10], objectcode[100];
    char text[30][100];

    FILE *input, *output, *symtab, *optab, *length;

    int locctr, start, len, byte_len = 0, count = 0;
    bool found_opcode, found_symbol;

    // Open all necessary files
    input = fopen("intermediate.txt", "r");
    output = fopen("output.txt", "w");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    length = fopen("length.txt", "r");

    if (!input || !output || !optab || !symtab || !length) {
        printf("File open error\n");
        return 1;
    }

    // Read program length
    fscanf(length, "%X", &len);
    fscanf(input, "%s %s %s %s", loc, label, opcode, operand);

    // Handle START directive (Header record)
    if (strcmp(opcode, "START") == 0) {
        start = (int)strtol(operand, NULL, 16);
        fprintf(output, "H^%s^%06X^%06X\n", label, start, len);
        fscanf(input, "%s %s %s %s", loc, label, opcode, operand);
    } else {
        start = (int)strtol(loc, NULL, 16);
        fprintf(output, "H^-^%06X^%06X\n", start, len);
    }

    locctr = start;

    // Process each line until END
    while (strcmp(opcode, "END") != 0) {
        found_opcode = false;
        rewind(optab);

        // Search opcode in optab
        while (fscanf(optab, "%s %s", mnemonic, code) == 2) {
            if (strcmp(mnemonic, opcode) == 0) {
                found_opcode = true;

                if (strcmp(operand, "-") != 0) {
                    found_symbol = false;
                    char symbol[10], sym_loc[10];
                    rewind(symtab);

                    // Search operand in symbol table
                    while (fscanf(symtab, "%s %s", symbol, sym_loc) == 2) {
                        if (strcmp(symbol, operand) == 0) {
                            strcpy(addr, sym_loc);
                            found_symbol = true;
                            break;
                        }
                    }

                    if (!found_symbol) {
                        printf("Invalid Symbol %s\n", operand);
                        return 1;
                    }
                } else {
                    strcpy(addr, "0000");
                }

                // Construct object code
                strcpy(objectcode, code);
                strcat(objectcode, addr);
                len = 3;
                break;
            }
        }

        // Handle assembler directives
        if (!found_opcode) {
            if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'X') {
                    int hex_len = strlen(operand) - 3;
                    strncpy(objectcode, operand + 2, hex_len);
                    objectcode[hex_len] = '\0';
                    len = hex_len / 2;
                } else if (operand[0] == 'C') {
                    int char_len = strlen(operand) - 3;
                    strcpy(objectcode, "");
                    for (int i = 0; i < char_len; i++) {
                        char temp[3];
                        intToHex((int)operand[i + 2], temp);
                        if (strlen(temp) == 1) strcat(objectcode, "0");
                        strcat(objectcode, temp);
                    }
                    len = char_len;
                } else {
                    printf("Invalid BYTE %s\n", operand);
                    return 1;
                }
            } else if (strcmp(opcode, "WORD") == 0) {
                char temp[10];
                intToHex(atoi(operand), temp);
                int t_len = strlen(temp);
                strcpy(objectcode, "");
                for (int i = 0; i < 6 - t_len; i++) strcat(objectcode, "0");
                strcat(objectcode, temp);
                len = 3;
            } else if (!strcmp(opcode, "RESB") || !strcmp(opcode, "RESW")) {
                // Flush current text record if memory reserved
                if (byte_len > 0) {
                    fprintf(output, "T^%06X^%02X", locctr, byte_len);
                    for (int i = 0; i < count; i++) fprintf(output, "^%s", text[i]);
                    fprintf(output, "\n");
                    byte_len = 0;
                    count = 0;
                }

                locctr += (!strcmp(opcode, "RESB")) ? atoi(operand) : 3 * atoi(operand);
                fscanf(input, "%s %s %s %s", loc, label, opcode, operand);
                continue;
            } else {
                printf("Invalid Opcode %s\n", opcode);
                return 1;
            }
        }

        // If object code record exceeds 30 bytes, write and start a new one
        if (byte_len + len > 30) {
            fprintf(output, "T^%06X^%02X", locctr, byte_len);
            for (int i = 0; i < count; i++) fprintf(output, "^%s", text[i]);
            fprintf(output, "\n");
            byte_len = len;
            count = 0;
            locctr = (int)strtol(loc, NULL, 16);
            strcpy(text[count++], objectcode);
        } else {
            strcpy(text[count++], objectcode);
            byte_len += len;
        }

        locctr += len;
        fscanf(input, "%s %s %s %s", loc, label, opcode, operand);
    }

    // Write remaining text record
    if (byte_len > 0) {
        fprintf(output, "T^%06X^%02X", locctr - byte_len, byte_len);
        for (int i = 0; i < count; i++) fprintf(output, "^%s", text[i]);
        fprintf(output, "\n");
    }

    // End record
    fprintf(output, "E^%06X\n", start);

    // Close files
    fclose(output);
    fclose(symtab);
    fclose(optab);
    fclose(input);
    fclose(length);

    return 0;
}
