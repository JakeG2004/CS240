/*
PCB.h
Defines what a PCB is, along with some basic functions for PCBs
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//file guards
#ifndef PCB_H
#define PCB_H

//enum to demonstrate state
enum
{
    READY,
    RUNNING,
    TERMINATED
};

//create the struct
struct PCB
{
    char processName[32];
    int arrivalTime;
    int totalCPUTimeNeeded;
    int PID;
    int currentCPUTimeUsed;
    int state;
};

typedef struct PCB PCB;

PCB CreatePCB(char* processName, int arrivalTime, int totalCPUTimeNeeded, int PID)
{
    PCB newPCB;

    //truncate process name if longer than 32 characters
    strncpy(newPCB.processName, processName, sizeof(newPCB.processName));
    //append null terminator
    newPCB.processName[sizeof(newPCB.processName) - 1] = '\0';

    //fill in the rest of the fields
    newPCB.arrivalTime = arrivalTime;
    newPCB.totalCPUTimeNeeded = totalCPUTimeNeeded;
    newPCB.PID = PID;

    //these should always be the same for every new process
    newPCB.currentCPUTimeUsed = 0;
    newPCB.state = READY;

    return newPCB;
}

//prints all the values in the PCB
void PrintPCB(PCB PCB)
{
    printf("\n==========\n");

    printf("Process name: %s\n", PCB.processName);
    printf("Arrival time: %i\n", PCB.arrivalTime);
    printf("Total CPU time needed: %i\n", PCB.totalCPUTimeNeeded);
    printf("PID: %i\n", PCB.PID);
    printf("Current CPU time used: %i\n", PCB.currentCPUTimeUsed);

    printf("State: ");

    switch(PCB.state)
    {
        case READY:
            printf("Ready\n");
            break;

        case RUNNING:
            printf("Running\n");
            break;

        case TERMINATED:
            printf("Terminated\n");
            break;
    }
}

#endif