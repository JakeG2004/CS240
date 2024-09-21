#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

void Produce();
void CreateData();
void SignalHandler();

int main(void)
{
    srand(time(NULL));

    if(signal(SIGUSR1, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }

    printf("PRODUCER: Awaiting signal...\n");

    //wait for signals to arrive
    while(1)
    {
        pause();
    }
}

void Produce()
{
    //create buffer and fill it with random data
    char buffer[10];
    CreateData(buffer, sizeof(buffer));

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

    sleep(1);
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

void SignalHandler(int sig)
{
    if(sig == SIGUSR1)
    {
        Produce();
        kill(getppid(), SIGUSR1);
    }

    else
    {
        printf("PRODUCER: Received unkown signal.\n");
    }
}