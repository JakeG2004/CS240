/*
PCBQueue.c
Implements functions for managing a queue of PCBs
*/

#include "PCBQueue.h"

// Function to enqueue a PCB to the queue
int Enqueue(QNodePtr* head, PCB PCB)
{
    QNodePtr newNode = (QNodePtr)(malloc(sizeof(QNode)));

    // Check for memory allocation errors
    if (newNode == NULL)
    {
        printf("Failed to malloc in Enqueue\n");
        return 0;
    }

    // Fill data
    newNode->PCB = PCB;
    newNode->next = NULL;

    QNodePtr p = *head;

    // Handle empty list
    if (p == NULL)
    {
        *head = newNode;
        return 1;
    }

    // Traverse to the end
    while (p->next != NULL)
    {
        p = p->next;
    }

    // Insert at the end
    p->next = newNode;

    return 1;
}

// Function to dequeue a PCB from the queue
PCB Dequeue(QNodePtr* head)
{
    QNodePtr p = *head;

    // Grab return value
    PCB retVal = p->PCB;

    // Set head to the next node
    *head = (*head)->next;

    // Break link and free memory
    p->next = NULL;
    free(p);

    //printf("Dequeuing %s\n", retVal.processName);

    return retVal;
}

// Function to get the size of the queue
int SizeOfQueue(QNodePtr head)
{
    int size = 0;
    QNodePtr p = head;

    while (p != NULL)
    {
        size++;
        p = p->next;
    }

    return size;
}

// Function to print the entire queue
void PrintQueue(QNodePtr head)
{
    QNodePtr p = head;

    // Print each node's PCB
    while (p != NULL)
    {
        PrintPCB(p->PCB);
        p = p->next;
    }
}

// Function to free the memory of the queue
void FreeQueue(QNodePtr* head)
{
    if (*head == NULL)
    {
        return;
    }

    FreeQueue(&(*head)->next);

    free(*head);
}
