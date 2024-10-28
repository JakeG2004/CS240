/*
ProcessTable.h
A header file for creating a dynamic 2D array of processes organized based on their arrival time.
*/

#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

// Node for making a 2D linked list
typedef struct PTNode
{
    PCB PCB;
    struct PTNode* nextBucket;
    struct PTNode* nextItem;
} PTNode;

typedef PTNode* PTNodePtr;

// Function prototypes
PTNodePtr CreatePTNode(PCB PCB);
void InsertPTNode(PTNodePtr* head, PTNodePtr newNode);
PCB FetchProcessWithArrivalTime(PTNodePtr* head, int arrivalTime);
void PrintContentsOfBucketWithArrivalTime(PTNodePtr head, int arrivalTime);
void PrintBuckets(PTNodePtr head);
void FreeTable(PTNodePtr* head);

#endif // PROCESS_TABLE_H
