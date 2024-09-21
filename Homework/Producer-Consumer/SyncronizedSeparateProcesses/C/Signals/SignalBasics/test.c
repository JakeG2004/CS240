#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    //fork starts the child process on the same line, returning 0 to the child process and the child's PID to the parent process
    pid_t childPID = fork();

    //if this is the child
    if(childPID == 0)
    {
        printf("BROADCASTER: Child process PID is: %li\n", (long)getpid());
        execl("./receiver", "./receiver", NULL);
    }

    //if this is the parent
    else if(childPID > 0)
    {
        //delay so that the child process can start
        sleep(1);

        printf("BROADCASTER: Starting broadcast...\n");
        while(1)
        {
            printf("BROADCASTER: Sending signal SIGUSR1\n");
            kill(childPID, SIGUSR1);
            sleep(1);
        }
    }

    else
    {
        printf("BROADCASTER: error forking\n");
    }
}