#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void InitSemaphores(sem_t** mutex, sem_t** empty, sem_t** full);
void ConsumeItem(char* str);

int main(void)
{
    srand(time(NULL));

    sem_t* mutex;
    sem_t* empty;
    sem_t* full;

    InitSemaphores(&mutex, &empty, &full);

    while(1)
    {
        char item[20];

        //wait for file access and for file to be empty
        sem_wait(full);
        sem_wait(mutex);

        //open file
        FILE* file;
        file = fopen("file.txt", "r");

        if(file == NULL)
        {
            printf("CONSUMER: Unable to open file!\n");
        }

        if(fgets(item, sizeof(item), file) != NULL)
        {
            ConsumeItem(item);
        }

        //reset file pointer to start
        fseek(file, 0, SEEK_SET);

        //create temp file for truncation
        FILE *temp = fopen("temp.txt", "w");
        char line[20];

        //copy every line except for the first one
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strcmp(line, item) != 0) {
                fprintf(temp, "%s", line);
            }
        }

        //update buffer
        rename("temp.txt", "file.txt");

        //close the files
        fclose(temp);
        fclose(file);

        //update semaphores
        sem_post(empty);
        sem_post(mutex);

        sleep(2);
    }
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

void ConsumeItem(char* str)
{
    printf("CONSUMER: Consumed %s\n", str);
}