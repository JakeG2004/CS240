#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "CSV.h"
#include "PCB.h"
#include "RCB.h"
#include "PCBQueue.h"
#include "ProcessTable.h"


//=============PROTOTYPES===================
PCB SimulateProcessCycle();

void InsertProcessesFromCSVIntoTable(char* filePath, PTNodePtr* head);
void GetProcessesAtArrivalTime(int arrivalTime);

void PromoteAndDemoteProcess(PCB* currentProcess);
void ReEnqueueProcess(PCB currentProcess, int* hasProcess);
void HandleProcessCompletion(PCB* currentProcess);
void AgeProcesses(PCB* currentProcess);

void InitResources(RCB resources[], int numResources);
int ClaimResource(PCB* process, RCB* resource);

int ChooseCurrentProcess(PCB* currentProcess);


//=============GLOBAL VARIABLES=============
int TICK_MAX = 1000000;
int QUANTUM = 100;
int MAX_PRIORITY = 2;

float PROMOTION_THRESHOLD = 0.8;
float DEMOTION_THRESHOLD = 0.2;

PTNodePtr processTable = NULL;

QNodePtr p0Queue = NULL;
QNodePtr p1Queue = NULL;
QNodePtr p2Queue = NULL;

QNodePtr terminatedQueue = NULL;

const int numResources = 3;
RCB resources[3];

int main()
{
    srand(time(NULL));
    InsertProcessesFromCSVIntoTable("TestData.csv", &processTable);
    InitResources(resources, numResources);

    PCB currentProcess = SimulateProcessCycle();

    //print useful info
    printf("\nAT TICK %i\nCURRENT PROCESS:\n", TICK_MAX);
    if(currentProcess.totalCPUTimeNeeded < 1)
    {
        printf("No remaining processes.\n");
    }

    else
    {
        PrintPCB(currentProcess);
    }

    printf("\nNUMBER OF PROCESSES IN P0 QUEUE: %i", SizeOfQueue(p0Queue));
    printf("\nNUMBER OF PROCESSES IN P1 QUEUE: %i", SizeOfQueue(p1Queue));
    printf("\nNUMBER OF PROCESSES IN P2 QUEUE: %i", SizeOfQueue(p2Queue));
    printf("\nNUMBER OF COMPLETED PROCESSES: %i\n", SizeOfQueue(terminatedQueue));


    //free priority queues
    FreeQueue(&p0Queue);
    FreeQueue(&p1Queue);
    FreeQueue(&p2Queue);

    //free terminated queue
    FreeQueue(&terminatedQueue);

    //free process table
    FreeTable(&processTable);
}

//emulate the processing cycle of the operating system, will end when TICK_MAX reached
PCB SimulateProcessCycle()
{
    PCB currentProcess;
    int hasProcess = 0;
    int startTime = 0;
    int needsQing;

    for(int tick = 0; tick < TICK_MAX; tick++)
    {
        //age processes to top every S ticks
        if(tick % (QUANTUM * 100) == 0)
        {
            AgeProcesses(&currentProcess);
        }

        //end if TICK_MAX reached
        if(tick == TICK_MAX)
        {
            return currentProcess;
        }

        //fetch from process table and insert into queues
        GetProcessesAtArrivalTime(tick);

        //grab current process if it doesn't already have one
        if(!hasProcess)
        {
            printf("Fetching new process!\n");
            //if there is no current process, loop
            if(ChooseCurrentProcess(&currentProcess) == 0)
            {
                continue;
            }

            else
            {
                hasProcess = 1;
                currentProcess.state = RUNNING;
                currentProcess.timeUsage = (rand() % QUANTUM) + 1;
            }
        }

        if(currentProcess.arrivalTime != -1)
        {
            printf("%s wants %i at time %i\n", currentProcess.processName, currentProcess.desiredResource, currentProcess.resourceAcquiredAtTime);
            //printf("%i state: %i\n", currentProcess.desiredResource, resources[currentProcess.desiredResource].state);
        }
        getchar();
        printf("\np0Q: ");
        PrintQueue(p0Queue);        
        getchar();

        printf("\np1Q: ");
        PrintQueue(p1Queue);        
        getchar();

        printf("\np2Q: ");
        PrintQueue(p2Queue);        
        getchar();

        //run process time slice
        for(int j = 0; j < QUANTUM; j++)
        {
            needsQing = 0;
            //add to tick
            tick++;

            // handle resource allocation for processes
            if(currentProcess.currentCPUTimeUsed == currentProcess.resourceAcquiredAtTime && currentProcess.resource == NULL && currentProcess.desiredResource != -1)
            {
                // resource is already allocated
                if(ClaimResource(&currentProcess, &resources[currentProcess.desiredResource]) == 0)
                {
                    // enqueue the process to the resource's queue
                    printf("%s enqueued to %s\n", currentProcess.processName, resources[currentProcess.desiredResource].resourceName);
                    currentProcess.state = BLOCKED;
                    Enqueue(&(resources[currentProcess.desiredResource].PCBQueue), currentProcess);
                    break;
                }

                printf("%s has %s\n", currentProcess.processName, currentProcess.resource->resourceName);
            }

            if(currentProcess.resource != NULL)
            {
                currentProcess.resource -> resourceTimer--;

                if(currentProcess.resource -> resourceTimer == 0)
                {
                    printf("%s freed %s\n", currentProcess.processName, currentProcess.resource -> resourceName);
                    
                    // If there is anything in the resource's ready list, dequeue it
                    if(SizeOfQueue(currentProcess.resource -> PCBQueue) != 0)
                    {
                        printf("Dequeuing process from resource\n");
                        PCB newProcess = Dequeue(&(currentProcess.resource -> PCBQueue));
                        newProcess.state = READY;

                        ReEnqueueProcess(newProcess, &hasProcess);
                    }

                    FreeRCB(&currentProcess, currentProcess.resource);
                }
            }

            //age processes to top every S ticks
            if(tick % (QUANTUM * 100) == 0)
            {
                AgeProcesses(&currentProcess);
            }

            //end if TICK_MAX reached
            if(tick == TICK_MAX)
            {
                return currentProcess;
            }

            //if process cant utilize the rest of the quantum, break
            if(j >= currentProcess.timeUsage)
            {
                needsQing = 1;
                if(currentProcess.arrivalTime != -1)
                    printf("%s hit usage limit\n", currentProcess.processName);
                break;
            }

            //add processes to queue
            GetProcessesAtArrivalTime(tick);

            currentProcess.currentCPUTimeUsed++;

            //end quantum early if process is complete
            if(currentProcess.currentCPUTimeUsed == currentProcess.totalCPUTimeNeeded)
            {
                if(currentProcess.arrivalTime != -1)
                    printf("%s complete\n", currentProcess.processName);
                break;
            }
        }

        //handle process completion if process complete
        if(currentProcess.currentCPUTimeUsed >= currentProcess.totalCPUTimeNeeded)
        {
            //printf("%s completed at tick %i, ended with priority %i, and arrived at time %i\n", currentProcess.processName, tick, currentProcess.priority, currentProcess.arrivalTime);
            HandleProcessCompletion(&currentProcess);
            printf("\nTEST1\n");
        }

        //handle re-enqueuing if process doesn't need a resource, or it successfully got a resource
        else if(currentProcess.desiredResource ==  -1 || currentProcess.resource != NULL)
        {
            PromoteAndDemoteProcess(&currentProcess);
            ReEnqueueProcess(currentProcess, &hasProcess);
            printf("\nTEST2\n");
        }

        // process wants resource, but couldn't get it
        else if(currentProcess.desiredResource >= 0 && currentProcess.resource == NULL)
        {
            printf("\nTEST3\n");
            if(needsQing)
            {
                //printf("Re-enqueueing\n");
                ReEnqueueProcess(currentProcess, &hasProcess);
            }

            //printf("Process wanted a resource, but couldn't get it\n");


            ChooseCurrentProcess(&currentProcess);

            //set new time usage
            currentProcess.timeUsage = rand() % QUANTUM;
        }
    }

    return currentProcess;
}

//handle taking data from CSV file and inserting it into the process table
void InsertProcessesFromCSVIntoTable(char* filePath, PTNodePtr* head)
{
    printf("Inserting processes to table\n");
    FILE* processFile = OpenCSV(filePath);

    char* line = NULL;
    int PID = 0;

    //while there are lines to be read
    while(ReadLine(processFile, &line))
    {
        //printf("%s\n", line);
        //printf("Reading line %i\n", PID);

        //get data fields from the line
        char* processName = GetField(line, 0);
        int arrivalTime = atoi(GetField(line, 1));
        int totalCPUTimeNeeded = atoi(GetField(line, 2));
        int processPriority = atoi(GetField(line, 3));
        int desiredResource = atoi(GetField(line, 4));
        int totalResourceTime = atoi(GetField(line, 5));
        int resourceAcquireTime = atoi(GetField(line, 6));

        //create a PCB
        PCB newPCB = CreatePCB(processName, arrivalTime, totalCPUTimeNeeded, PID, processPriority, totalResourceTime, resourceAcquireTime, desiredResource);

        //PrintPCB(newPCB);

        //create and insert new node
        PTNodePtr newPTNode = CreatePTNode(newPCB);
        InsertPTNode(head, newPTNode);

        free(line);
        free(processName);

        //increment the PID
        PID++;
    }

    CloseCSV(processFile);
    printf("Done!\n");
}

//fetches all processes with arrival time n from the process table, and queue them accordingly
void GetProcessesAtArrivalTime(int arrivalTime)
{
    PCB newPCB;

    //fetch all the processes with the arrival time
    newPCB = FetchProcessWithArrivalTime(&processTable, arrivalTime);
    while(newPCB.arrivalTime != -1)
    {
        switch(newPCB.priority)
        {
            case 0:
                Enqueue(&p0Queue, newPCB);
                break;

            case 1:
                Enqueue(&p1Queue, newPCB);
                break;

            case 2:
                Enqueue(&p2Queue, newPCB);
                break;

            default:
                printf("Encountered a process with priority %i\n", newPCB.priority);
        }
        newPCB = FetchProcessWithArrivalTime(&processTable, arrivalTime);
    }
}

//promote and demote processes according to promotion and demotion rules
void PromoteAndDemoteProcess(PCB* currentProcess)
{
    //promote if great time usage
    if((*currentProcess).timeUsage > (QUANTUM * PROMOTION_THRESHOLD))
    {
        //printf("Promoted process!\n");
        (*currentProcess).priority--;

        if((*currentProcess).priority < 0)
            (*currentProcess).priority = 0;
    }

    //demote if less time usage
    if((*currentProcess).timeUsage < (QUANTUM * DEMOTION_THRESHOLD))
    {
        //printf("Demoted process!\n");
        (*currentProcess).priority++;

        if((*currentProcess).priority > MAX_PRIORITY)
            (*currentProcess).priority = MAX_PRIORITY;
    }
}

//handle re-enqueing of process after it uses its quantum
void ReEnqueueProcess(PCB currentProcess, int* hasProcess)
{
    //re-enqueue if process not complete
    if(currentProcess.currentCPUTimeUsed < currentProcess.totalCPUTimeNeeded)
    {
        *hasProcess = 0;
        currentProcess.state = READY;
        currentProcess.timeUsage = rand() % QUANTUM;
        //currentProcess.timeUsage = QUANTUM;

        switch(currentProcess.priority)
        {
            case 0:
                Enqueue(&p0Queue, currentProcess);
                printf("Re-enqueued %s to p0\n", currentProcess.processName);
                break;
            case 1:
                Enqueue(&p1Queue, currentProcess);
                printf("Re-enqueued %s to p1\n", currentProcess.processName);
                break;
            case 2:
                Enqueue(&p2Queue, currentProcess);
                printf("Re-enqueued %s to p2\n", currentProcess.processName);
                break;
        }

        /*PrintQueue(p0Queue);
        getchar();
        PrintQueue(p1Queue);
        getchar();
        PrintQueue(p2Queue);
        getchar();*/
    }
}

//mark process as complete and select a new process
void HandleProcessCompletion(PCB* currentProcess)
{
    //negative arrival time denotes no current process
    if((*currentProcess).arrivalTime != -1)
    {
        (*currentProcess).state = TERMINATED;
        Enqueue(&terminatedQueue, *currentProcess);
        ChooseCurrentProcess(currentProcess);

        //set new time usage
        (*currentProcess).timeUsage = rand() % QUANTUM;
    } 
}

//move all processes to top queue to prevent starvation
void AgeProcesses(PCB* currentProcess)
{
    printf("Aging processes\n");

    (*currentProcess).priority = 0;

    //move from p1queue into p0queue
    while(SizeOfQueue(p1Queue) != 0)
    {
        Enqueue(&p0Queue, Dequeue(&p1Queue));
    }

    //move from p2queue into p0 queue
    while(SizeOfQueue(p2Queue) != 0)
    {
        Enqueue(&p0Queue, Dequeue(&p2Queue));
    }
}

void InitResources(RCB resources[], int numResources)
{
    char curName[32];

    for(int i = 0; i < numResources; i++)
    {
        sprintf(curName, "resource%i", i);
        resources[i] = CreateRCB(curName);
    }
}

//will return 0 if couldnt find a process, 1 otherwise
int ChooseCurrentProcess(PCB* currentProcess)
{
    //grab from p0queue
    if(SizeOfQueue(p0Queue) != 0)
    {
        *currentProcess = Dequeue(&p0Queue);
        return 1;
    }

    //grab from p1queue
    if(SizeOfQueue(p1Queue) != 0)
    {
        *currentProcess = Dequeue(&p1Queue);
        return 1;
    }

    //grab from p2queue
    if(SizeOfQueue(p2Queue) != 0)
    {
        *currentProcess = Dequeue(&p2Queue);
        return 1;
    }

    *currentProcess = CreatePCB("", -1, -1, -1, -1, -1, -1, -1);
    return 0;
}

int ClaimResource(PCB* process, RCB* resource)
{
    if(resource -> state == ALLOCATED)
    {
        process -> resource = NULL;
        return 0;
    }

    OwnRCB(process, resource);
    return 1;
}



