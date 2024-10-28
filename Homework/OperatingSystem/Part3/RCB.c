/*
RCB.c
Implements the functions for managing RCBs
*/

#include "RCB.h"

// Function to create an RCB
RCB CreateRCB(char* resourceName)
{
    RCB newRCB;

    // Truncate resource name if longer than 32 characters
    strncpy(newRCB.resourceName, resourceName, sizeof(newRCB.resourceName) - 1);

    // Append null terminator
    newRCB.resourceName[sizeof(newRCB.resourceName) - 1] = '\0';

    // These should always be the same for every new resource
    newRCB.resourceTimer = 0;
    newRCB.state = FREE;

    // Initialize the PCB queue as empty
    newRCB.PCBQueue = NULL;

    return newRCB;
}

// Function to print all the values in the RCB
void PrintRCB(RCB RCB)
{
    printf("\n==========\n");

    printf("Resource name: %s\n", RCB.resourceName);
    printf("Resource timer: %i\n", RCB.resourceTimer);

    printf("State: ");
    switch (RCB.state)
    {
        case FREE:
            printf("Free\n");
            break;

        case ALLOCATED:
            printf("Allocated\n");
            break;
    }

    // Print the PCB queue associated with this RCB
    PrintQueue(RCB.PCBQueue);
}

void OwnRCB(PCB* process, RCB* resource)
{
    process -> resource = resource;

    resource -> state = ALLOCATED;

    resource -> resourceTimer = process -> totalResourceTime;
}

void FreeRCB(PCB* process, RCB* resource)
{
    process -> resource = NULL;
    process -> desiredResource = -1;

    resource -> state = FREE;
    resource -> resourceTimer = 0;
}
