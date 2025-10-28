#include <stdio.h>
#include <string.h>

#define MAX_ARGS 10
#define MAX_LEN  20

int find_param_index(char param_names[MAX_ARGS][MAX_LEN], int param_count, const char *op) {
    for (int i = 0; i < param_count; i++) {
        if (strcmp(param_names[i], op) == 0)
            return i;
    }
    return -1;
}

int main() {
    FILE *input, *namtab, *deftab, *argtab, *output;
    char label[20], mne[20], op[100];
    char name[20], temp[20];
    char def_mne[20], def_op[100];
    char param_names[MAX_ARGS][MAX_LEN];
    int param_count = 0;

    input = fopen("inp.txt", "r");
    namtab = fopen("namtab.txt", "w+");
    deftab = fopen("deftab.txt", "w+");
    argtab = fopen("argtab.txt", "w+");
    output = fopen("output.txt", "w");

    if (!input) {
        printf("Error opening input file.\n");
        return 1;
    }

    fscanf(input, "%s%s%s", label, mne, op);
    while (strcmp(mne, "END") != 0) {

        // Case 1: Macro definition
        if (strcmp(mne, "MACRO") == 0) {
            strcpy(name, label);
            fprintf(namtab, "%s\n", name);

            // Store prototype
            fprintf(deftab, "%s\t%s\n", label, op);

            // Parse parameter names from prototype
            param_count = 0;
            char op_copy[100];
            strcpy(op_copy, op);
            char *tok = strtok(op_copy, ",");
            while (tok) {
                strcpy(param_names[param_count++], tok);
                tok = strtok(NULL, ",");
            }

            // Read and store macro body
            fscanf(input, "%s%s%s", label, mne, op);
            while (strcmp(mne, "MEND") != 0) {
                fprintf(deftab, "%s\t%s\n", mne, op);
                fscanf(input, "%s%s%s", label, mne, op);
            }
            fprintf(deftab, "%s\n", mne); // Write MEND
        }

        // Case 2: Macro call
        else {
            rewind(namtab);
            int found = 0;
            while (fscanf(namtab, "%s", temp) != EOF) {
                if (strcmp(mne, temp) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found) {
                // Directly extract arguments into args array
                int arg_count = 0;
                char op_copy[100];
                strcpy(op_copy, op);
                char args[MAX_ARGS][MAX_LEN];
                char *tok = strtok(op_copy, ",");
                while (tok && arg_count < MAX_ARGS) {
                    strcpy(args[arg_count++], tok);
                    tok = strtok(NULL, ",");
                }

                // Expand the macro
                rewind(deftab);
                fscanf(deftab, "%s%s", def_mne, def_op); // Skip prototype
                fscanf(deftab, "%s%s", def_mne, def_op);

                while (strcmp(def_mne, "MEND") != 0) {
                    int idx = find_param_index(param_names, arg_count, def_op);
                    if (idx != -1) {
                        fprintf(output, "%s\t%s\n", def_mne, args[idx]);
                    } else {
                        fprintf(output, "%s\t%s\n", def_mne, def_op);
                    }
                    fscanf(deftab, "%s%s", def_mne, def_op);
                }
            }
            else {
                fprintf(output, "%s\t%s\t%s\n", label, mne, op);
            }
        }

        fscanf(input, "%s%s%s", label, mne, op);
    }

    fprintf(output, "%s\t%s\t%s\n", label, mne, op); // Write END line

    printf("\nMacro Processing Completed.\n");

    fclose(input);
    fclose(namtab);
    fclose(deftab);
    fclose(argtab);
    fclose(output);
    return 0;
}