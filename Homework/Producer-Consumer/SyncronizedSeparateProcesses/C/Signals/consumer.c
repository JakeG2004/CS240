#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void SignalHandler(int sig);
void Consume();

int main(void)
{
    if(signal(SIGUSR1, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }

    printf("CONSUMER: Awaiting signal...\n");

    //wait for signals to arrive
    while(1)
    {
        pause();
    }
}

void Consume()
{
    char item[20];
    
    //open file
    FILE* file;
    file = fopen("file.txt", "r");

    //check for errors
    if(file == NULL)
    {
        printf("CONSUMER: Unable to open file!\n");
    }

    //read first item
    if(fgets(item, sizeof(item), file) != NULL)
    {
        printf("CONSUMER: Consuming %s\n", item);
    }

    //reset file pointer to start
    fseek(file, 0, SEEK_SET);

    //create temp file for truncation
    FILE *temp = fopen("temp.txt", "w");
    char line[20];

    //copy every line except for the first one
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        if (strcmp(line, item) != 0) 
        {
            fprintf(temp, "%s", line);
        }
    }

    //update buffer
    rename("temp.txt", "file.txt");

    //close the files
    fclose(temp);
    fclose(file);

    sleep(1);

}

void SignalHandler(int sig)
{
    if(sig == SIGUSR1)
    {
        Consume();
        kill(getppid(), SIGUSR2);
    }

    else
    {
        printf("CONSUMER: Received unkown signal.\n");
    }
}

