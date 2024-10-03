#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PCB.h"
#include "ProcessTable.h"

PCB CreateRandomProcess(int arrivalTime);

int main()
{
    srand(time(NULL));

    //create process table
    PTNodePtr processTable = NULL;

    //insert 10 nodes
    for(int i = 0; i < 10; i++)
    {
        PTNodePtr newNode = CreatePTNode(CreateRandomProcess(i));
        InsertPTNode(&processTable, newNode);
    }

    //insert 5 more
    for(int i = 0; i < 5; i++)
    {
        PTNodePtr newNode = CreatePTNode(CreateRandomProcess(i));
        InsertPTNode(&processTable, newNode);
    }

    //print values
    printf("\n\nALL VALUES IN TABLE\n\n");
    for(int i = 0; i < 10; i++)
    {
        PrintContentsOfBucketWithArrivalTime(processTable, i);
    }

    //remove some values
    printf("\n\n\nREMOVING 1 OF EACH {0, 9} FROM TABLE\n\n\n");

    for(int i = 0; i < 10; i++)
    {
        FetchProcessWithArrivalTime(&processTable, i);
    }

    //print values
    printf("\n\nALL VALUES IN TABLE\n\n");
    for(int i = 0; i < 10; i++)
    {
        PrintContentsOfBucketWithArrivalTime(processTable, i);
    }

    FreeTable(&processTable);
}

PCB CreateRandomProcess(int arrivalTime)
{
    return CreatePCB("testProcess", arrivalTime, 10, 1);
}