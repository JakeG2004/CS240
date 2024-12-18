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

//=============FUNCTIONS DEALING WITH THE PROCESS TABLE====================
void InsertProcessesFromCSVIntoTable(char* filePath, PTNodePtr* head);
void GetProcessesAtArrivalTime(int arrivalTime);

//==============FUNCTIONS DEALING WITH QUEUES============
void PromoteAndDemoteProcess(PCB* currentProcess);
void ReEnqueueProcess(PCB currentProcess);
void HandleProcessCompletion(PCB* currentProcess);
void AgeProcesses(PCB* currentProcess);

//===========FUNCTIONS DEALING WITH PROCESSES==========
void GetRandomTimeUsage(PCB* process);
int ChooseCurrentProcess(PCB* currentProcess);

//============FUNCTIONS DEALING WITH RESOURCES============
void HandleProcessWithResource(PCB* currentProcess);
void InitResources(RCB resources[], int numResources);

int ClaimResource(PCB* process, RCB* resource);
int GetResourceIfAvailable(PCB* currentProcess, int* timeUsed);


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

    // free resource queues
    for(int i = 0; i < numResources; i++)
    {
        printf("\nNUMBER OF PROCESSES IN P%i QUEUE: %i", i, SizeOfQueue(resources[i].PCBQueue));
    }

    printf("\n");


    //free priority queues
    FreeQueue(&p0Queue);
    FreeQueue(&p1Queue);
    FreeQueue(&p2Queue);

    //free terminated queue
    FreeQueue(&terminatedQueue);

    // free resource queues
    for(int i = 0; i < numResources; i++)
    {
        FreeQueue(&(resources[i].PCBQueue));
    }

    //free process table
    FreeTable(&processTable);
}

//emulate the processing cycle of the operating system, will end when TICK_MAX reached
PCB SimulateProcessCycle()
{
    PCB currentProcess;
    int timeUsed = 0;

    for(int tick = 0; tick < TICK_MAX; tick++)
    {
        // Get all the processes at the current tick
        GetProcessesAtArrivalTime(tick);

        // Go to next iteration if no processes
        if(ChooseCurrentProcess(&currentProcess) == 0)
        {
            continue;
        }

        // Assign time usage
        GetRandomTimeUsage(&currentProcess);

        // track time usage
        timeUsed = 0;

        // process the process
        for(int j = 0; j < QUANTUM; j++)
        {
            // Return if simulation complete
            if(tick >= TICK_MAX)
            {
                return currentProcess;
            }

            // Condition 1 to leave the loop: process is complete
            if(currentProcess.currentCPUTimeUsed >= currentProcess.totalCPUTimeNeeded)
            {
                timeUsed = j;
                break;
            }

            // Condition 2 to leave this loop: process doesn't use its whole quanatum
            if(j >= currentProcess.timeUsage)
            {
                timeUsed = j;
                break;
            }

            if(GetResourceIfAvailable(&currentProcess, &timeUsed) == -1)
            {
                break;
            }

            // Increment and decrement counters accordingly
            currentProcess.currentCPUTimeUsed++;

            tick++;

            HandleProcessWithResource(&currentProcess);

            // Insert new processes into the table
            GetProcessesAtArrivalTime(tick);
        }

        // process completed
        if(currentProcess.currentCPUTimeUsed >= currentProcess.totalCPUTimeNeeded)
        {
            HandleProcessCompletion(&currentProcess);
        }

        // process didn't use whole timeslice
        else if(timeUsed < QUANTUM && timeUsed != -1)
        {
            PromoteAndDemoteProcess(&currentProcess);
            ReEnqueueProcess(currentProcess);
        }
    }

    return currentProcess;
}

//=============FUNCTIONS DEALING WITH PROCESS TABLE============

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


//===============FUNCTIONS DEALING WITH QUEUES====================

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
void ReEnqueueProcess(PCB currentProcess)
{
    //re-enqueue if process not complete
    if(currentProcess.currentCPUTimeUsed < currentProcess.totalCPUTimeNeeded)
    {
        currentProcess.state = READY;
        currentProcess.timeUsage = rand() % QUANTUM;
        //currentProcess.timeUsage = QUANTUM;

        switch(currentProcess.priority)
        {
            case 0:
                Enqueue(&p0Queue, currentProcess);
                break;
            case 1:
                Enqueue(&p1Queue, currentProcess);
                break;
            case 2:
                Enqueue(&p2Queue, currentProcess);
                break;
        }
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


//============FUNCTIONS DEALING WITH PROCESSES===================

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

void GetRandomTimeUsage(PCB* process)
{
    if(process != NULL)
        process -> timeUsage = rand() % QUANTUM + 1;
}


//==========FUNCTIONS DEALING WITH RESOURCES=================

// 1 for got resource, -1 for blocked on resource, 0 for not checked
int GetResourceIfAvailable(PCB* currentProcess, int* timeUsed)
{
    // Handle resources
    if(currentProcess -> desiredResource != -1 && currentProcess -> resource == NULL && currentProcess -> currentCPUTimeUsed == currentProcess -> resourceAcquiredAtTime)
    {
        // Attempt to claim a resource, if failure, block and enqueue
        if(ClaimResource(currentProcess, &(resources[currentProcess -> desiredResource])) == 0)
        {
            //printf("Blocking %s on %s\n", currentProcess.processName, resources[currentProcess.desiredResource].resourceName);
            currentProcess -> state = BLOCKED;
            Enqueue(&(resources[currentProcess -> desiredResource].PCBQueue), *currentProcess);
            
            *timeUsed = -1;

            //break;
            return -1;
        }

        return 1;

        //printf("%s has %s\n", currentProcess.processName, currentProcess.resource -> resourceName);
    }

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

void HandleProcessWithResource(PCB* currentProcess)
{
    if(currentProcess -> resource != NULL)
    {
        currentProcess -> resource -> resourceTimer--;

        if(currentProcess -> resource -> resourceTimer == 0)
        {
            if(SizeOfQueue(currentProcess -> resource -> PCBQueue) > 0)
            {
                PCB freedProcess = Dequeue(&(currentProcess -> resource -> PCBQueue));
                freedProcess.state = READY;
                ReEnqueueProcess(freedProcess);
            }
            
            FreeRCB(currentProcess, currentProcess -> resource);
        }
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