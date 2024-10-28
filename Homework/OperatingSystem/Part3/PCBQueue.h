/*
PCBQueue.h
Defines a queue structure for PCBs and functions for manipulating the queue
*/

#ifndef PCBQUEUE_H
#define PCBQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

// Node structure for the queue
typedef struct QNode
{
    PCB PCB;
    struct QNode* next;
} QNode;

typedef QNode* QNodePtr;

// Function prototypes
int Enqueue(QNodePtr* head, PCB PCB);
PCB Dequeue(QNodePtr* head);
int SizeOfQueue(QNodePtr head);
void PrintQueue(QNodePtr head);
void FreeQueue(QNodePtr* head);

#endif // PCBQUEUE_H
