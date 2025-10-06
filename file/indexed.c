// Indexed File Allocation in C
#include <stdio.h>
#include <stdbool.h>

#define MAX_BLOCKS 100   // Maximum number of disk blocks

int main() {
    int TOTAL_BLOCKS, n;

    // Step 1: Input total number of disk blocks
    printf("Enter total number of blocks: ");
    scanf("%d", &TOTAL_BLOCKS);

    // Validate input
    if (TOTAL_BLOCKS <= 0 || TOTAL_BLOCKS > MAX_BLOCKS) {
        printf("Invalid number of blocks.\n");
        return 1;
    }

    bool BLOCKS[MAX_BLOCKS] = {0};  // 0 = free, 1 = occupied

    // Step 2: Input number of files
    printf("Enter number of files: ");
    scanf("%d", &n);

    // Step 3: Allocate blocks for each file
    for (int i = 0; i < n; i++) {
        int fileSize;     // Number of data blocks needed
        int indexBlock;   // Block that stores addresses of all data blocks

        printf("\nFile %d:\n", i + 1);
        printf("Enter File Size (number of blocks required): ");
        scanf("%d", &fileSize);

        // Step 4: Allocate an index block
        indexBlock = -1;
        for (int j = 0; j < TOTAL_BLOCKS; j++) {
            if (!BLOCKS[j]) {        // If block is free
                indexBlock = j;
                BLOCKS[j] = true;    // Mark as allocated
                break;
            }
        }

        if (indexBlock == -1) {
            printf("Allocation failed! No free block available for index block.\n");
            continue;
        }

        int allocated[fileSize];      // To store allocated data blocks
        int allocatedCount = 0;

        // Step 5: Allocate data blocks
        for (int j = 0; j < TOTAL_BLOCKS && allocatedCount < fileSize; j++) {
            if (!BLOCKS[j]) {
                BLOCKS[j] = true;     // Mark block as used
                allocated[allocatedCount++] = j;
            }
        }

        // Step 6: Check if enough blocks were allocated
        if (allocatedCount < fileSize) {
            printf("Allocation failed! Not enough free blocks.\n");

            // Rollback — free all allocated blocks
            BLOCKS[indexBlock] = false;
            for (int k = 0; k < allocatedCount; k++)
                BLOCKS[allocated[k]] = false;
        } else {
            // Step 7: Display allocation result
            printf("File allocated successfully!\n");
            printf("Index Block: %d\n", indexBlock);
            printf("Allocated Data Blocks: ");
            for (int k = 0; k < fileSize; k++)
                printf("%d ", allocated[k]);
            printf("\n");
        }
    }

    // Step 8: Display Final Block Allocation Table
    printf("\nBlock Allocation Table:\n");
    printf("Block Number\tStatus\n");

    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        printf("%d\t\t%s\n", i, BLOCKS[i] ? "Occupied" : "Free");
    }

    return 0;
}
