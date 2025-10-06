// rr.c
#include <stdio.h>
#include <stdlib.h> // For malloc, free

// Structure to represent a process
typedef struct {
    int name;       // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int remaining;  // Remaining burst time
    int status;     // 0 = not ready/arrived, 1 = ready/running
    int completion; // Completion time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
} Process;

// Structure to represent a node in the ready queue (linked list)
typedef struct Node {
    Process process;
    struct Node* next;
} Node;

// Global pointers for the ready queue
Node *front = NULL, *rear = NULL;

// Function to enqueue a process into the ready queue
void enqueue(Process process) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for queue node");
        exit(1);
    }

    newNode->process = process;
    newNode->next = NULL;

    if (!front) // Queue is empty
        front = rear = newNode;
    else { // Queue is not empty
        rear->next = newNode;
        rear = newNode;
    }
}

// Function to dequeue a process from the ready queue
Process dequeue() {
    if (!front) { // Should not happen if called correctly
        fprintf(stderr, "Attempt to dequeue from empty queue\n");
        exit(1);
    }

    Node* temp = front;
    Process process = temp->process;

    front = front->next;
    if (!front) rear = NULL;

    free(temp);
    return process;
}

// Round Robin Scheduling function
void roundRobin(Process processes[], int n, int quantum) {
    int currentTime = 0, completed = 0;

    // Sort processes by arrival time initially (bubble sort)
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (processes[j].arrival > processes[j + 1].arrival) {
                Process temp_proc = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp_proc;
            }
        }
    }

    printf("\n--- Gantt Chart ---\n");
    int current_process_index = 0;

    // Initial check for processes arriving at time 0
    while (current_process_index < n && processes[current_process_index].arrival <= currentTime) {
        if (!processes[current_process_index].status) {
            enqueue(processes[current_process_index]);
            processes[current_process_index].status = 1;
        }
        current_process_index++;
    }

    printf("Time 0 "); // Start of Gantt Chart

    // Main scheduling loop
    while (completed < n) {
        if (front) { // Ready queue is not empty
            Process currentProcess = dequeue();

            int executionTime = (currentProcess.remaining < quantum) ? currentProcess.remaining : quantum;

            currentProcess.remaining -= executionTime;
            currentTime += executionTime;

            printf("| P%d (%d) ", currentProcess.name, currentTime);

            // Check for new arrivals during or after this execution
            while (current_process_index < n && processes[current_process_index].arrival <= currentTime) {
                if (!processes[current_process_index].status) {
                    enqueue(processes[current_process_index]);
                    processes[current_process_index].status = 1;
                }
                current_process_index++;
            }

            if (currentProcess.remaining == 0) { // Process finished
                completed++;
                for (int i = 0; i < n; i++)
                    if (processes[i].name == currentProcess.name) {
                        processes[i].completion = currentTime;
                        break;
                    }
            } else { // Process not finished, enqueue again
                enqueue(currentProcess);
            }
        } else { // Ready queue empty, CPU idle
            if (current_process_index < n) {
                printf("| Idle (%d) ", processes[current_process_index].arrival);
                currentTime = processes[current_process_index].arrival;

                while (current_process_index < n && processes[current_process_index].arrival <= currentTime) {
                    if (!processes[current_process_index].status) {
                        enqueue(processes[current_process_index]);
                        processes[current_process_index].status = 1;
                    }
                    current_process_index++;
                }
            } else {
                break; // No processes left
            }
        }
    }

    printf("|\n--- End Gantt Chart ---\n\n");
    printf("Process Arrival Burst Completion Turnaround Waiting\n");

    float totalWT = 0, totalTAT = 0;

    // Calculate and display final times
    for (int i = 0; i < n; i++) {
        int idx = -1;
        for (int k = 0; k < n; ++k)
            if (processes[k].name == (i + 1)) { idx = k; break; }

        if (idx != -1 && processes[idx].completion != -1) {
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;

            totalWT += processes[idx].waiting;
            totalTAT += processes[idx].turnaround;

            printf("P%-7d %-7d %-6d %-10d %-10d %-6d\n",
                   processes[idx].name, processes[idx].arrival,
                   processes[idx].burst, processes[idx].completion,
                   processes[idx].turnaround, processes[idx].waiting);
        } else {
            printf("P%-7d %-7d %-6d %-10s %-10s %-6s\n",
                   (i + 1), processes[idx].arrival, processes[idx].burst, "N/A", "N/A", "N/A");
        }
    }

    printf("\nAverage Waiting Time = %.2f\n", totalWT / n);
    printf("Average Turnaround Time = %.2f\n", totalTAT / n);
}

int main() {
    int n, quantum;

    // User input for number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process processes[n];

    // User input for process details
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for P%d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);

        processes[i].name = i + 1;
        processes[i].remaining = processes[i].burst;
        processes[i].status = 0;       // 0: Not ready/arrived
        processes[i].completion = -1;  // Initialize completion time
        processes[i].waiting = 0;
        processes[i].turnaround = 0;
    }

    // Quantum time input
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    // Run Round Robin scheduling
    roundRobin(processes, n, quantum);

    return 0;
}
