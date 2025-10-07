#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void scan(int req[], int n, int ds, int head, int dir) {
    sort(req, n);

    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (req[i] == head) {
            pos = i;
            break;
        }
    }

    int seek = 0;
    printf("Seek sequence:\n");

    if (dir == 1) { // Right direction
        for (int i = pos; i < n; i++) {
            printf("%d -> ", req[i]);
            seek += abs(req[i] - head);
            head = req[i];
        }

        // Go to end if not already there
        if (head != ds - 1) {
            printf("%d -> ", ds - 1);
            seek += abs(ds - 1 - head);
            head = ds - 1;
        }

        for (int i = pos - 1; i >= 0; i--) {
            printf("%d -> ", req[i]);
            seek += abs(req[i] - head);
            head = req[i];
        }
    } else { // Left direction
        for (int i = pos; i >= 0; i--) {
            printf("%d -> ", req[i]);
            seek += abs(req[i] - head);
            head = req[i];
        }

        // Go to start if not already there
        if (head != 0) {
            printf("0 -> ");
            seek += abs(head - 0);
            head = 0;
        }

        for (int i = pos + 1; i < n; i++) {
            printf("%d -> ", req[i]);
            seek += abs(req[i] - head);
            head = req[i];
        }
    }

    printf("END\n");
    printf("Total seek time: %d\n", seek);
}

int main() {
    int n;
    printf("Enter size of sequence: ");
    scanf("%d", &n);

    int req[n + 1];
    printf("Enter sequence:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    int head;
    printf("Enter head: ");
    scanf("%d", &head);

    req[n] = head;
    n++;

    int dir;
    printf("Enter direction (1 for right, 0 for left): ");
    scanf("%d", &dir);

    int ds;
    printf("Enter disk size: ");
    scanf("%d", &ds);

    scan(req, n, ds, head, dir);
    return 0;
}