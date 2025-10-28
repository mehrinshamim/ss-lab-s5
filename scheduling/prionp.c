// prio.c
#include <stdio.h>
#include <stdbool.h> // For bool type (though int 0/1 is used)
#include <limits.h>  // For INT_MAX

// Structure to hold process details
struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int priority;   // Priority (lower value = higher priority)
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
    int completed;  // Completed flag (0 = not completed, 1 = completed)
};

// Function to find the highest priority (lowest number) process that has
// arrived and is not completed
int findNextProcess(struct Process p[], int n, int currentTime) {
    int minPriority = INT_MAX; // Initialize minimum priority
    int index = -1;            // Index of selected process

    for (int i = 0; i < n; i++) {
        // Process not completed AND has arrived
        if (!p[i].completed && p[i].arrival <= currentTime) {
            if (p[i].priority < minPriority) {
                minPriority = p[i].priority;
                index = i;
            }
            // Optional tie-breaking rule (e.g., lower PID or earlier arrival)
        }
    }

    return index; // Return index of selected process, or -1 if none ready
}

// Function to calculate waiting and turnaround times (Non-Preemptive Priority)
void calculateTimes(struct Process p[], int n) {
    int currentTime = 0;       // Simulation clock
    int completedProcesses = 0; // Number of processes completed

    printf("\n--- Execution Sequence ---\n");

    while (completedProcesses < n) {
        int index = findNextProcess(p, n, currentTime);

        if (index == -1) { // CPU idle
            printf("Time %d: CPU Idle\n", currentTime);
            currentTime++;
        } else {
            printf("Time %d: Process %d starts execution\n", currentTime, p[index].id);

            // Calculate waiting time: Time spent waiting since arrival
            p[index].waiting = currentTime - p[index].arrival;

            // Calculate turnaround time: Total time from arrival to completion
            p[index].turnaround = (currentTime + p[index].burst) - p[index].arrival;

            // Update current time after process execution
            currentTime += p[index].burst;

            p[index].completed = 1; // Mark as completed
            completedProcesses++;

            printf("Time %d: Process %d finishes execution\n", currentTime, p[index].id);
        }
    }

    printf("--- Execution End ---\n");
}

// Function to display the results table
void display(struct Process p[], int n) {
    printf("\n--- Results Table ---\n");
    printf("ID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");

    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].priority,
               p[i].waiting, p[i].turnaround);

        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

int main() {
    int n; // Number of processes

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Get process details from user
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time, burst time, and priority for process %d: ", p[i].id);
        scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority);
        p[i].completed = 0; // Initially not completed
    }

    // Calculate scheduling times
    calculateTimes(p, n);

    // Display results
    display(p, n);

    return 0;
}