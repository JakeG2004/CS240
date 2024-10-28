/*
PCB.h
Defines what a PCB is, along with some basic functions for PCBs
*/

#ifndef PCB_H
#define PCB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration of RCB
struct RCB; // This tells the compiler that RCB is a structure, but without needing the full definition.

// Enum to demonstrate state
enum
{
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

// Create the PCB struct
struct PCB
{
    char processName[32];
    int arrivalTime;
    int totalCPUTimeNeeded;
    int PID;
    int priority;

    int totalResourceTime;
    int resourceAcquiredAtTime;
    int desiredResource;
    
    struct RCB* resource; // Use a pointer to RCB to avoid including RCB.h

    int currentCPUTimeUsed;
    int state;
    int timeUsage;
};

typedef struct PCB PCB;

// Function prototypes
PCB CreatePCB(char* processName, int arrivalTime, int totalCPUTimeNeeded, int PID, int priority, int totalResourceTime, int resourceNeededAtTime, int desiredResource);
void PrintPCB(PCB PCB);

#endif // PCB_H
