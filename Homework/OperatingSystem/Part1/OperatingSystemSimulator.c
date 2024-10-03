#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CSV.h"
#include "PCB.h"
#include "PCBQueue.h"
#include "ProcessTable.h"

void InsertProcessesFromCSVIntoTable(char* filePath, PTNodePtr* head);
PCB SimulateProcessCycle(PTNodePtr* processTable, QNodePtr* processQueue, QNodePtr* terminatedQueue, int TICK_MAX);
void GetProcessesAtArrivalTime(PTNodePtr* processTable, QNodePtr* processQueue, int arrivalTime);

int main()
{
    int TICK_MAX = 1000;

    PTNodePtr processTable = NULL;
    QNodePtr readyQueue = NULL;
    QNodePtr terminatedQueue = NULL;

    InsertProcessesFromCSVIntoTable("TestData.csv", &processTable);

    PCB currentProcess = SimulateProcessCycle(&processTable, &readyQueue, &terminatedQueue, TICK_MAX);

    printf("\nAT TICK %i\nCURRENT PROCESS:\n", TICK_MAX);
    if(currentProcess.totalCPUTimeNeeded == 0)
    {
        printf("No remaining processes.\n");
    }

    else
    {
        PrintPCB(currentProcess);
    }

    printf("\nNUMBER OF REMAINING PROCESSES: %i\n", SizeOfQueue(readyQueue));
    printf("\nNUMBER OF COMPLETED PROCESSES: %i\n", SizeOfQueue(terminatedQueue));

    FreeQueue(&readyQueue);
    FreeQueue(&terminatedQueue);
    FreeTable(&processTable);
}

PCB SimulateProcessCycle(PTNodePtr* processTable, QNodePtr* processQueue, QNodePtr* terminatedQueue, int TICK_MAX)
{
    PCB currentProcess;
    int hasProcess = 0;
    int startTime = 0;

    for(int tick = 0; tick < TICK_MAX; tick++)
    {
        if(tick == TICK_MAX)
        {
            return currentProcess;
        }

        //fetch from process table and insert into queue
        GetProcessesAtArrivalTime(processTable, processQueue, tick);

        //if empty queue, simply move along
        if(SizeOfQueue(*processQueue) == 0 && !hasProcess)
        {
            continue;
        }

        //select a process if one is not currently being run
        if(!hasProcess)
        {
            currentProcess = Dequeue(processQueue);
            currentProcess.state = RUNNING;
            hasProcess = 1;
            startTime = tick;
        }

        //simulate process taking time
        for(int elapsedTime = currentProcess.currentCPUTimeUsed; elapsedTime < currentProcess.totalCPUTimeNeeded; elapsedTime++)
        {
            //add to tick to simulate usage
            tick++;
            currentProcess.currentCPUTimeUsed++;

            if(tick == TICK_MAX)
            {
                return currentProcess;
            }

            GetProcessesAtArrivalTime(processTable, processQueue, tick);
        }

        //print useful data
        //printf("%s arrived at time %i, had process time of %i, started at time %i, and finished at time %i\n", currentProcess.processName, currentProcess.arrivalTime, currentProcess.totalCPUTimeNeeded, startTime, tick);

        //clear process flag
        hasProcess = 0;
        currentProcess.state = TERMINATED;
        Enqueue(terminatedQueue, currentProcess);

        //switch process if queue not empty to avoid switching time bug
        if(SizeOfQueue(*processQueue) != 0)
        {
            currentProcess = Dequeue(processQueue);
            currentProcess.state = RUNNING;
            hasProcess = 1;
            startTime = tick;

            //handle switching time bug
            currentProcess.currentCPUTimeUsed++;
        }
    }

    PrintQueue(*processQueue);
}

void GetProcessesAtArrivalTime(PTNodePtr* processTable, QNodePtr* processQueue, int arrivalTime)
{
    PCB newPCB;

    //fetch all the processes with the arrival time
    newPCB = FetchProcessWithArrivalTime(processTable, arrivalTime);
    while(newPCB.arrivalTime != -1)
    {
        Enqueue(processQueue, newPCB);
        newPCB = FetchProcessWithArrivalTime(processTable, arrivalTime);
    }
}

void InsertProcessesFromCSVIntoTable(char* filePath, PTNodePtr* head)
{
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

        //create a PCB
        PCB newPCB = CreatePCB(processName, arrivalTime, totalCPUTimeNeeded, PID);

        PTNodePtr newPTNode = CreatePTNode(newPCB);
        InsertPTNode(head, newPTNode);

        free(line);
        free(processName);

        //increment the PID
        PID++;
    }

    CloseCSV(processFile);
}