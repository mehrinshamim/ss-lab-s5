// FCFS Disk Scheduling
#include <stdio.h>
#include <stdlib.h>

// Function to perform FCFS disk scheduling
void fcfs(int r, int st, int request[r]) {
    int seektime = 0;   // Total seek time
    int parseek = 0;    // Seek time for current request
    int current = st;   // Current head position

    printf("Seek sequence: %d", current);

    for (int i = 0; i < r; i++) {
        parseek = abs(current - request[i]); // Calculate seek for this request
        seektime += parseek;                 // Add to total seek time
        current = request[i];                // Move head to this request
        printf(" -> %d", current);
    }

    printf("\nTotal seek time is: %d\n", seektime);
}

int main() {
    int r, st;

    // User input for number of requests
    printf("Enter number of requests: ");
    scanf("%d", &r);

    int request[r];

    // User input for request queue
    printf("Enter requests: ");
    for (int i = 0; i < r; i++) {
        scanf("%d", &request[i]);
    }

    // User input for starting head position
    printf("Enter starting head position: ");
    scanf("%d", &st);

    // Call FCFS disk scheduling function
    fcfs(r, st, request);

    return 0;
}
