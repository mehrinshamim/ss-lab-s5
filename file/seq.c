// Sequential File Allocation in C
#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 100   // Maximum number of blocks in the disk

// Structure to represent a file
struct File {
    char filename[20];   // File name
    int start_block;     // Starting block index
    int num_blocks;      // Number of blocks required
};

int main() {
    int n;  // Total number of disk blocks

    // Step 1: Input total number of disk blocks
    printf("Enter total number of blocks in disk (max %d): ", MAX_BLOCKS);
    scanf("%d", &n);

    // Validate number of blocks
    if (n > MAX_BLOCKS || n <= 0) {
        printf("Invalid number of blocks.\n");
        return 1;
    }

    int disk[MAX_BLOCKS] = {0};  // 0 = free block, 1 = occupied
    int m;                       // Number of files

    // Step 2: Input number of files to store
    printf("Enter number of files to store: ");
    scanf("%d", &m);

    struct File files[m];  // Array of file structures

    // Step 3: Allocate space for each file
    for (int i = 0; i < m; i++) {
        printf("\nEnter file name: ");
        scanf("%s", files[i].filename);

        printf("Enter number of blocks needed for %s: ", files[i].filename);
        scanf("%d", &files[i].num_blocks);

        int found = 0;  // Flag to check if space found

        // Step 4: Search for a sequence of free blocks
        for (int j = 0; j <= n - files[i].num_blocks; j++) {
            int free = 1;  // Assume blocks are free

            // Check next num_blocks positions
            for (int k = 0; k < files[i].num_blocks; k++) {
                if (disk[j + k] == 1) {
                    free = 0;  // Found occupied block
                    break;
                }
            }

            // Step 5: Allocate if space is free
            if (free) {
                files[i].start_block = j;

                for (int k = 0; k < files[i].num_blocks; k++) {
                    disk[j + k] = 1;  // Mark block as allocated
                }

                found = 1;
                break;
            }
        }

        // Step 6: Handle case when allocation fails
        if (!found) {
            files[i].start_block = -1;
            printf("Allocation failed for file %s\n", files[i].filename);
        }
    }

    // Step 7: Display File Allocation Table
    printf("\nFile Allocation Table:\n");
    printf("Filename\tStart Block\tBlocks\n");

    for (int i = 0; i < m; i++) {
        if (files[i].start_block != -1) {
            printf("%s\t\t%d\t\t%d\n",
                   files[i].filename,
                   files[i].start_block,
                   files[i].num_blocks);
        }
    }

    return 0;
}
