// Linked File Allocation in C
#include <stdio.h>
#include <stdbool.h>

#define MAX_BLOCKS 100   // Maximum number of disk blocks

int main() {
    int TOTAL_BLOCKS, n;

    // Step 1: Input total number of blocks on disk
    printf("Enter total number of blocks: ");
    scanf("%d", &TOTAL_BLOCKS);

    // Validation
    if (TOTAL_BLOCKS <= 0 || TOTAL_BLOCKS > MAX_BLOCKS) {
        printf("Invalid number of blocks.\n");
        return 1;
    }

    bool BLOCKS[MAX_BLOCKS] = {0};  // 0 = free, 1 = occupied
    int NEXT[MAX_BLOCKS];           // Stores pointer to next block
    for (int i = 0; i < MAX_BLOCKS; i++)
        NEXT[i] = -1;               // Initialize all next pointers to -1

    // Step 2: Input number of files
    printf("Enter number of files: ");
    scanf("%d", &n);

    // Step 3: Allocate blocks for each file
    for (int f = 0; f < n; f++) {
        int fileSize;
        printf("\nFile %d:\n", f + 1);
        printf("Enter File Size (number of blocks required): ");
        scanf("%d", &fileSize);

        int allocated[fileSize];       // To store allocated blocks
        int allocatedCount = 0;

        // Step 4: Find free blocks for this file
        for (int i = 0; i < TOTAL_BLOCKS && allocatedCount < fileSize; i++) {
            if (!BLOCKS[i]) {          // If block is free
                BLOCKS[i] = true;      // Mark it as occupied
                allocated[allocatedCount++] = i;
            }
        }

        // Step 5: Check if enough free blocks were found
        if (allocatedCount < fileSize) {
            printf("Allocation failed! Not enough free blocks.\n");

            // Rollback: free already allocated blocks
            for (int k = 0; k < allocatedCount; k++)
                BLOCKS[allocated[k]] = false;
        } else {
            // Step 6: Link the blocks together
            for (int k = 0; k < fileSize - 1; k++)
                NEXT[allocated[k]] = allocated[k + 1];

            NEXT[allocated[fileSize - 1]] = -1;  // End of chain

            // Step 7: Display allocation result
            printf("File allocated successfully!\n");
            printf("Starting Block: %d\n", allocated[0]);
            printf("Linked Blocks: ");
            for (int k = 0; k < fileSize; k++)
                printf("%d -> ", allocated[k]);
            printf("NULL\n");
        }
    }

    // Step 8: Display Block Allocation Table
    printf("\nBlock Allocation Table:\n");
    printf("Block Number\tStatus\tNext Block\n");

    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        printf("%d\t\t%s\t", i, BLOCKS[i] ? "Occupied" : "Free");
        if (BLOCKS[i])
            printf("%d\n", NEXT[i]);  // Show next linked block
        else
            printf("-\n");
    }

    return 0;
}
