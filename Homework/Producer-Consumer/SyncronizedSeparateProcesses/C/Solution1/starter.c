// starter.c
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    //create semaphores
    sem_t *mutex = sem_open("/mutex", O_CREAT, 0666, 1);  // Mutual exclusion semaphore
    sem_t *empty = sem_open("/empty", O_CREAT, 0666, 1);  // Buffer empty semaphore
    sem_t *full = sem_open("/full", O_CREAT, 0666, 0);    // Buffer full semaphore

    //check for errors
    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("Semaphore initialization failed");
        return 1;
    }

    printf("Semaphores initialized.\n");

    //start consumer
    pid_t consumer_pid = fork();
    if(consumer_pid == 0)
    {
        execl("./consumer", "./consumer", NULL);
        perror("Failed to start consumer");
        exit(-1);
    }

    //start producer
    pid_t producer_pid = fork();
    if(producer_pid == 0)
    {
        execl("./producer", "./producer", NULL);
        perror("Failed to start producer");
        exit(-1);
    }


    //prompt user to end process
    printf("Press Enter to stop: \n");
    getchar();

    //kill child processes
    kill(consumer_pid, SIGTERM);
    kill(producer_pid, SIGTERM);

    //wait for children to clis
    waitpid(consumer_pid, NULL, 0);
    waitpid(producer_pid, NULL, 0);

    //close the semaphores
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);

    printf("Semaphores closed and process terminated.\n");
}
