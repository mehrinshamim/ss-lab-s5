// FCFS (First Come First Serve) CPU Scheduling in C
#include <stdio.h>

// Structure to hold process details
struct Process {
    int pid;   // Process ID
    int at;    // Arrival Time
    int bt;    // Burst Time
    int ct;    // Completion Time
    int tat;   // Turnaround Time
    int wt;    // Waiting Time
};

// Function to sort processes by Arrival Time using Bubble Sort
void sortByArrivalTime(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Swap if current process arrives later than the next one
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Function to calculate Completion, Turnaround, and Waiting times
void fcfs(struct Process p[], int n) {
    int currentTime = 0; // CPU clock tracker
    int pro[n];          // For Gantt chart process order
    int times[n];        // For Gantt chart completion times

    printf("\nGantt Chart:\n");

    // Iterate through each process
    for (int i = 0; i < n; i++) {
        // If CPU is idle until this process arrives
        if (currentTime < p[i].at) {
            currentTime = p[i].at;
        }

        // Calculate Completion Time
        p[i].ct = currentTime + p[i].bt;

        // Calculate Turnaround Time = CT - AT
        p[i].tat = p[i].ct - p[i].at;

        // Calculate Waiting Time = TAT - BT
        p[i].wt = p[i].tat - p[i].bt;

        // Update current time
        currentTime = p[i].ct;

        // Store info for Gantt chart
        pro[i] = p[i].pid;
        times[i] = p[i].ct;
    }

    // Print Gantt Chart
    for (int i = 0; i < n; i++) {
        printf("P%d\t", pro[i]);
    }
    printf("\n0\t");

    for (int i = 0; i < n; i++) {
        printf("%d\t", times[i]);
    }
    printf("\n");
}

// Function to display all process details and results
void display(struct Process p[], int n) {
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");

    float totalWT = 0, totalTAT = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);

        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }

    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
}

int main() {
    int n; // Number of processes

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n]; // Array of processes

    // Input: Arrival Time (AT) and Burst Time (BT)
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        p[i].pid = i + 1;
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    // Step 1: Sort by arrival time
    sortByArrivalTime(p, n);

    // Step 2: Run FCFS scheduling
    fcfs(p, n);

    // Step 3: Display results
    display(p, n);

    return 0;
}
