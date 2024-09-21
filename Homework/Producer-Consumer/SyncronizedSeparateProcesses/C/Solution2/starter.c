#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void InitSemaphores(sem_t** mutex, sem_t** empty, sem_t** full);
void CloseSemaphores(sem_t* mutex, sem_t* empty, sem_t* full);

pid_t* StartProducers(int numProducers);
pid_t* StartConsumers(int numConsumers);

int GetIntInput(int numDigits);

void KillProcesses(pid_t* producers, pid_t* consumers, int numProducers, int numConsumers);

int main(void)
{
    int waitTime = 3;

    //create and init semaphores
    sem_t* mutex;
    sem_t* empty;
    sem_t* full;

    InitSemaphores(&mutex, &empty, &full);

    //get user input
    printf("Enter the number of producers: ");
    int numProducers = GetIntInput(3);

    printf("Enter the number of consumers: ");
    int numConsumers = GetIntInput(3);

    //inform user of how to exit
    printf("-----\nPRESS ENTER TO STOP. STARTING IN %i SECONDS\n-----\n", waitTime);

    sleep(waitTime);

    //start n processes
    pid_t* producers = StartProducers(numProducers);
    pid_t* consumers = StartConsumers(numConsumers);

    //wait for user input
    getchar();

    //kill the processes
    KillProcesses(producers, consumers, numProducers, numConsumers);

    CloseSemaphores(mutex, empty, full);
}

void KillProcesses(pid_t* producers, pid_t* consumers, int numProducers, int numConsumers)
{
    //kill producers
    printf("Killing producers...\n");
    for(int i = 0; i < numProducers; i++)
    {
        kill(producers[i], SIGTERM);
        waitpid(producers[i], NULL, 0);
    }

    //kill consumers
    printf("Killing consumers...\n");
    for(int i = 0; i < numConsumers; i++)
    {
        kill(consumers[i], SIGTERM);
        waitpid(consumers[i], NULL, 0);
    }

    //inform user that program is exiting successfully
    printf("Done! Exiting program...\n");
}

int GetIntInput(int numDigits)
{
    //temporary variables
    char buffer[numDigits + 2];  //+2 to account for newline and null-terminator
    int retVal = 0;

    //get the input
    fgets(buffer, sizeof(buffer), stdin);

    //convert string to int
    for(int i = 0; i < numDigits; i++)
    {
        //check if character is a digit
        if(buffer[i] >= '0' && buffer[i] <= '9')
        {
            retVal = retVal * 10 + (buffer[i] - '0');
        }
        else
        {
            break; //stop conversion on non-digit input
        }
    }

    return retVal;
}

pid_t* StartProducers(int numProducers)
{
    printf("Starting %i producers\n", numProducers);

    //create array of producers
    pid_t* producers = (pid_t*)(malloc(sizeof(pid_t) * numProducers));

    //spawn n producers
    for(int i = 0; i < numProducers; i++)
    {
        producers[i] = fork();

        //check for errors
        if(producers[i] < 0)
        {
            perror("Producer fork failure!\n");
            exit(-2);
        }

        //if no errors, start a producer process
        if(producers[i] == 0)
        {
            execl("./producer", "./producer", NULL);
        }
    }

    return producers;
}

pid_t* StartConsumers(int numConsumers)
{
    printf("Starting %i consumers\n", numConsumers);

    //create array of consumers
    pid_t* consumers = (pid_t*)(malloc(sizeof(pid_t) * numConsumers));

    //spawn n consumers
    for(int i = 0; i < numConsumers; i++)
    {
        consumers[i] = fork();

        //check for errors
        if(consumers[i] < 0)
        {
            perror("Consumer fork failure!\n");
            exit(-2);
        }

        //if no errors, start a producer process
        if(consumers[i] == 0)
        {
            execl("./consumer", "./consumer", NULL);
        }
    }

    return consumers;
}

void InitSemaphores(sem_t** mutex, sem_t** empty, sem_t** full)
{
    *mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    *empty = sem_open("/empty", O_CREAT, 0666, 1);
    *full = sem_open("/full", O_CREAT, 0666, 0);

    if(*mutex == SEM_FAILED || *empty == SEM_FAILED || *full == SEM_FAILED)
    {
        printf("Failed to initialize semaphores! Exiting...\n");
        exit(-1);
    }

    printf("Initialized Semaphores\n");
}

void CloseSemaphores(sem_t* mutex, sem_t* empty, sem_t* full)
{
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);

    printf("Closed Semaphores\n");
}