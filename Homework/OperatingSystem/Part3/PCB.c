/*
PCB.c
Implements the functions for managing PCBs
*/

#include "PCB.h"

// Function to create a PCB
PCB CreatePCB(char* processName, int arrivalTime, int totalCPUTimeNeeded, int PID, int priority)
{
    PCB newPCB;

    // Truncate process name if longer than 32 characters
    strncpy(newPCB.processName, processName, sizeof(newPCB.processName) - 1);

    // Append null terminator
    newPCB.processName[sizeof(newPCB.processName) - 1] = '\0';

    // Fill in the rest of the fields
    newPCB.arrivalTime = arrivalTime;
    newPCB.totalCPUTimeNeeded = totalCPUTimeNeeded;
    newPCB.PID = PID;
    newPCB.priority = priority;

    // These should always be the same for every new process
    newPCB.currentCPUTimeUsed = 0;
    newPCB.state = READY;

    return newPCB;
}

// Function to print all the values in the PCB
void PrintPCB(PCB PCB)
{
    printf("\n==========\n");

    printf("Process name: %s\n", PCB.processName);
    printf("Arrival time: %i\n", PCB.arrivalTime);
    printf("Total CPU time needed: %i\n", PCB.totalCPUTimeNeeded);
    printf("PID: %i\n", PCB.PID);
    printf("Current CPU time used: %i\n", PCB.currentCPUTimeUsed);
    printf("Priority: %i\n", PCB.priority);

    printf("State: ");

    switch (PCB.state)
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