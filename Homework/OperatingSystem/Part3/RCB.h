/*
RCB.h
Defines what a RCB is, along with some basic functions for RCBs
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PCBQueue.h"

//file guards
#ifndef RCB_H
#define RCB_H

//enum to demonstrate state
enum
{
    ALLOCATED,
    FREE
};

//create the struct
struct RCB
{
    char resourceName[32];

    int state;
    int resourceTimer;

    QNodePtr PCBQueue;
};

typedef struct RCB RCB;

RCB CreateRCB(char* resourceName)
{
    RCB newRCB;

    //truncate process name if longer than 32 characters
    strncpy(newRCB.resourceName, resourceName, sizeof(newRCB.resourceName) - 1);

    //append null terminator
    newRCB.resourceName[sizeof(newRCB.resourceName) - 1] = '\0';

    //these should always be the same for every new process
    newRCB.resourceTimer = 0;
    newRCB.state = FREE;

    newRCB.PCBQueue = NULL;

    return newRCB;
}

//prints all the values in the PCB
void PrintRCB(RCB RCB)
{
    printf("\n==========\n");

    printf("Resource name: %s\n", RCB.resourceName);
    printf("Resource timer: %i\n", RCB.resourceTimer);

    printf("State: ");
    switch(RCB.state)
    {
        case FREE:
            printf("Free\n");
            break;

        case ALLOCATED:
            printf("Allocated\n");
            break;
    }

    PrintQueue(RCB.PCBQueue);
}

#endif