#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void SignalHandler(int sig);

int main()
{
    printf("RECEIVER: getpid() returns: %li\n", (long)getpid());

    if(signal(SIGUSR1, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }

    printf("RECEIVER: Waiting for signal...\n");
    while(1)
    {
        pause();
    }
}

void SignalHandler(int sig)
{
    printf("RECEIVER: Received signal SIGUSR1!\n");
}