#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>

void CreateData(char* str, int size);
void InitSemaphores(sem_t** mutex, sem_t** empty, sem_t** full);

int main(void)
{
    srand(time(NULL));

    sem_t* mutex;
    sem_t* empty;
    sem_t* full;

    InitSemaphores(&mutex, &empty, &full);

    while(1)
    {
        //create buffer and fill it with random data
        char buffer[10];
        CreateData(buffer, sizeof(buffer));

        //wait for file access and for file to be empty
        sem_wait(empty);

        sem_wait(mutex);

        //make and open file
        FILE* file;
        file = fopen("file.txt", "a");

        if(file == NULL)
        {
            printf("PRODUCER: Unable to open file...\n");
        }

        printf("PRODUCER: Writing %s\n", buffer);

        //write to the file
        fputs(buffer, file);

        //close the file
        fclose(file);

        //update semaphores
        sem_post(full);
        sem_post(mutex);

        sleep(1);
    }
}

void CreateData(char* str, int size)
{
    //create a 9 digit string of random characters
    for(int i = 0; i < size - 1; i++)
    {
        str[i] = rand() % 26 + 'a';
    }

    str[size - 1] = '\n';

    //printf("String: %s\n", str);
}

void InitSemaphores(sem_t** mutex, sem_t** empty, sem_t** full)
{
    *mutex = sem_open("/mutex", 0);
    *empty = sem_open("/empty", 0);
    *full = sem_open("/full", 0);

    if(*mutex == SEM_FAILED || *empty == SEM_FAILED || *full == SEM_FAILED)
    {
        printf("Failed to initialize semaphores! Exiting...\n");
        exit(-1);
    }

    //printf("Initialized Semaphores\n");
}