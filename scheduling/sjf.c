// sjf.c
#include <stdio.h>
#include <limits.h> // For INT_MAX

// Structure to hold process details
struct Process {
    int pid;       // Process ID
    int at;        // Arrival Time
    int bt;        // Burst Time
    int ct;        // Completion Time
    int tat;       // Turnaround Time
    int wt;        // Waiting Time
    int completed; // Completion flag (0 = not completed, 1 = completed)
};

// Function to find the next shortest job that has arrived and is not completed
int findShortestJob(struct Process p[], int n, int currentTime) {
    int minIndex = -1;       // Index of the shortest job found, -1 if none
    int minBurst = INT_MAX;  // Initialize minimum burst time to max possible value

    for (int i = 0; i < n; i++) {
        // Check if process is not completed AND has arrived by current time
        if (!p[i].completed && p[i].at <= currentTime) {
            // Check if this process has a shorter burst time than current minimum
            if (p[i].bt < minBurst) {
                minBurst = p[i].bt; // Update minimum burst time
                minIndex = i;       // Update index of the shortest job
            }

            // Optional: Tie-breaking rule for processes with same burst time
            /*
            else if (p[i].bt == minBurst) {
                // Choose the one that arrived earlier
                if (p[i].at < p[minIndex].at) {
                    minIndex = i;
                }
            }
            */
        }
    }

    return minIndex; // Return index of selected process, or -1 if none ready
}

// Function to calculate SJF (Non-Preemptive) scheduling metrics
void sjf(struct Process p[], int n) {
    int completed = 0;        // Counter for completed processes
    int currentTime = 0;      // Simulation clock
    int pro[n];               // Array to store process sequence for Gantt chart (PIDs)
    int times[n];             // Array to store completion times for Gantt chart
    int ind = 0;              // Index for Gantt chart arrays

    printf("--- Gantt Chart ---\n");

    while (completed < n) {
        // Find the shortest job ready to run
        int shortest = findShortestJob(p, n, currentTime);

        if (shortest == -1) { // No process is ready (CPU idle)
            // Find the next arrival time to jump ahead (optional optimization)
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; ++i) {
                if (!p[i].completed && p[i].at < next_arrival && p[i].at > currentTime) {
                    next_arrival = p[i].at;
                }
            }

            if (next_arrival != INT_MAX) { // Future arrival exists
                printf("Idle (%d) ", next_arrival);
                currentTime = next_arrival; // Jump time to next arrival
            } else { // No future arrivals, advance time by 1
                printf("Idle (%d) ", currentTime + 1);
                currentTime++;
            }
            continue; // Skip to next iteration
        }

        // Process the selected shortest job
        printf("| P%d (%d) ", p[shortest].pid, currentTime + p[shortest].bt);

        // Calculate Completion Time (CT)
        p[shortest].ct = currentTime + p[shortest].bt;

        // Calculate Turnaround Time (TAT = CT - AT)
        p[shortest].tat = p[shortest].ct - p[shortest].at;

        // Calculate Waiting Time (WT = TAT - BT)
        p[shortest].wt = p[shortest].tat - p[shortest].bt;

        // Update current time
        currentTime = p[shortest].ct;

        // Mark process as completed
        p[shortest].completed = 1;
        completed++; // Increment completed counter

        // Store info for Gantt chart
        pro[ind] = p[shortest].pid;
        times[ind++] = p[shortest].ct;
    }

    printf("|\n--- End Gantt Chart ---\n");

    /* 
    // Alternative Gantt printing using stored arrays
    for (int i = 0; i < n; i++) {
        printf("P%d\t", pro[i]);
    }
    printf("\n0\t");
    for (int i = 0; i < n; i++) {
        printf("%d\t", times[i]);
    }
    printf("\n");
    */
}

// Function to display the process details and calculated times
void display(struct Process p[], int n) {
    printf("\n--- Results Table ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        // Find process by PID to print in original order if needed
        int idx = -1;
        for (int k = 0; k < n; ++k) {
            if (p[k].pid == (i + 1)) {
                idx = k;
                break;
            }
        }

        if (idx != -1) {
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",
                   p[idx].pid, p[idx].at, p[idx].bt,
                   p[idx].ct, p[idx].tat, p[idx].wt);

            total_wt += p[idx].wt;
            total_tat += p[idx].tat;
        }
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

int main() {
    int n; // Number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n]; // Array of Process structures

    // Taking user input for Arrival Time (AT) and Burst Time (BT)
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        p[i].pid = i + 1; // Assign process ID
        scanf("%d %d", &p[i].at, &p[i].bt);

        p[i].completed = 0; // Initialize completed status to false
        p[i].ct = 0;
        p[i].tat = 0;
        p[i].wt = 0;
    }

    // Apply SJF Scheduling
    sjf(p, n);

    // Display the results table
    display(p, n);

    return 0;
}
