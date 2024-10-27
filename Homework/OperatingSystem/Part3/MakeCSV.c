#include <stdio.h>
#include <stdlib.h>

int MAX_CPU_TIME = 10;

int main(int argc, char* argv[]) {
    if(argc != 3)
    {
        printf("Incorrect usage: ./MakeCSV <number of rows> <max_cpu_time>\n");
        exit(-1);
    }

    int n = atoi(argv[1]);
    int MAX_CPU_TIME = atoi(argv[2]);

    // Allocate memory dynamically for process data
    char **processes = (char **)malloc(n * sizeof(char *));

    int *arrival_times = (int *)malloc(n * sizeof(int));
    int *cpu_times = (int *)malloc(n * sizeof(int));
    int *priorities = (int*)malloc(n * sizeof(int));
    int *resources = (int*)malloc(n * sizeof(int));

    // Generate process details (you can customize this logic)
    for (int i = 0; i < n; i++) {
        processes[i] = (char *)malloc(50 * sizeof(char));  // allocate space for process name
        snprintf(processes[i], 50, "Process%d", i + 1);    // Generate process name "Process1", "Process2", etc.
        arrival_times[i] = rand() % n + 1;                         // Example: generate arrival times (e.g., 0, 2, 4...)
        cpu_times[i] = rand() % MAX_CPU_TIME;                     // Example: generate CPU times (e.g., 10, 20, 30...)
        priorities[i] = rand() % 3;                        // priority levels 0,1,2
        resources[i] = (rand() % 4) - 1;                // 3 resources plus resource -1 for no resouurce
    }

    // Open the CSV file for writing
    FILE *file = fopen("TestData.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write each process data to the CSV
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s,%d,%d,%d,%d\n", processes[i], arrival_times[i], cpu_times[i], priorities[i], resources[i]);
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
    free(priorities);
    free(resources);

    printf("CSV file created successfully.\n");

    return 0;
}
