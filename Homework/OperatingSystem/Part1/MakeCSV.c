#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = 100;  // Set the number of processes

    // Allocate memory dynamically for process data
    char **processes = (char **)malloc(n * sizeof(char *));
    int *arrival_times = (int *)malloc(n * sizeof(int));
    int *cpu_times = (int *)malloc(n * sizeof(int));

    // Generate process details (you can customize this logic)
    for (int i = 0; i < n; i++) {
        processes[i] = (char *)malloc(50 * sizeof(char));  // allocate space for process name
        snprintf(processes[i], 50, "Process%d", i + 1);    // Generate process name "Process1", "Process2", etc.
        arrival_times[i] = rand() % n + 1;                         // Example: generate arrival times (e.g., 0, 2, 4...)
        cpu_times[i] = rand() % n + 1;                     // Example: generate CPU times (e.g., 10, 20, 30...)
    }

    // Open the CSV file for writing
    FILE *file = fopen("TestData.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write each process data to the CSV
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s,%d,%d\n", processes[i], arrival_times[i], cpu_times[i]);
    }

    // Close the file
    fclose(file);

    // Free the dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(processes[i]);
    }
    free(processes);
    free(arrival_times);
    free(cpu_times);

    printf("CSV file created successfully.\n");

    return 0;
}
