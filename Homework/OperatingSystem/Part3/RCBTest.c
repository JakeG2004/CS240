#include "PCB.h"
#include "RCB.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    RCB resource1 = CreateRCB("Resource1");
    PrintRCB(resource1);

    PCB p1 = CreatePCB("p1", 0, 0, 0, 0);
    Enqueue(&(resource1.PCBQueue), p1);

    PrintRCB(resource1);

    resource1.state = ALLOCATED;
    PrintRCB(resource1);

    FreeQueue(&(resource1.PCBQueue));
}