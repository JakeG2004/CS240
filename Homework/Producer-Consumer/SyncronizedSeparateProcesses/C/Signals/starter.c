#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int MakeChildProcess(pid_t*, char* process);
void SignalHandler(int sig);

void InitSignalHandler();
void InitProcesses();

//create pid values
pid_t producerPID;
pid_t consumerPID;

int main(void)
{
    //make our processes
    InitProcesses();

    //init our signal handler
    InitSignalHandler();

    //inform user on how to exit program
    printf("\n\n----------\nUse ctrl + c to exit the program when you are ready\n----------\n\n");
   
    //short delay
    sleep(3);

    //start the cycle
    kill(producerPID, SIGUSR1);

    //scan for signals
    while(1)
    {
        pause();
    }
}

//return true if success, false otherwise
int MakeChildProcess(pid_t* childPID, char* process)
{
    *childPID = fork();

    //handle error
    if(*childPID < 0)
    {
        printf("Failed to initialize child process with command %s\n", process);
        return 1;
    }

    //if child, do this
    else if(*childPID == 0)
    {
        printf("CHILD: Running process %s\n", process);

        //start corresponding process
        execl(process, process, NULL);

        printf("Error starting process %s\n", process);
        return 0;
    }

    //otherwise, if parent
    printf("Succesfully made %s process with PID %li\n", process, (long)*childPID);
    return 1;
}

void InitSignalHandler()
{
    //init SIGUSR1 listener
    if(signal(SIGUSR1, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }

    //inti SIGUSR2 listener
    if(signal(SIGUSR2, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }

    //inti SIGINT listener
    if(signal(SIGINT, SignalHandler) == SIG_ERR)
    {
        printf("RECEIVER: Error initializing signal listener. Exiting...\n");
        exit(-1);
    }
}

void InitProcesses()
{
    //make producer process
    if(MakeChildProcess(&producerPID, "./producer") == 0)
    {
        printf("Failure in making producer process. Exiting...\n");
        exit(-1);
    }

    //make consumer process
    if(MakeChildProcess(&consumerPID, "./consumer") == 0)
    {
        printf("Failure in making consumer process. Exiting...\n");
        exit(-1);
    }
}

void SignalHandler(int sig)
{
    //producer finished producing
    if(sig == SIGUSR1)
    {
        printf("STARTER: Received producer complete\n");
        //indicate to consumer that it may go
        kill(consumerPID, SIGUSR1);
    }

    //consumer finished consuming
    if(sig == SIGUSR2)
    {
        printf("STARTER: Received consumer complete\n");
        kill(producerPID, SIGUSR1);
    }

    //handle exiting program
    if(sig == SIGINT)
    {
        printf("\nKilling processes...\n");

        kill(consumerPID, SIGQUIT);
        kill(producerPID, SIGQUIT);

        //wait until processes are killed
        waitpid(producerPID, NULL, 0);
        waitpid(consumerPID, NULL, 0);

        printf("Successfully killed processes. Exiting...\n");

        exit(0);
    }
}