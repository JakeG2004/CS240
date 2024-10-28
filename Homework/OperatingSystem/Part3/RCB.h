/*
RCB.h
Defines what an RCB is, along with some basic functions for RCBs
*/

#ifndef RCB_H
#define RCB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCBQueue.h"

// Enum to demonstrate state
enum
{
    ALLOCATED,
    FREE
};

// Struct definition for an RCB
typedef struct RCB
{
    char resourceName[32];
    int state;
    int resourceTimer;
    QNodePtr PCBQueue;
} RCB;

// Function prototypes
RCB CreateRCB(char* resourceName);
void PrintRCB(RCB RCB);
void OwnRCB(PCB* process, RCB* resource);
void FreeRCB(PCB* process, RCB* resource);

#endif // RCB_H
