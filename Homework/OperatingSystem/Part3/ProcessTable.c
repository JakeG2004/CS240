/*
ProcessTable.c
Implements the functions for managing a dynamic 2D array of processes organized by arrival time.
*/

#include "ProcessTable.h"

// Function to create a new PTNode
PTNodePtr CreatePTNode(PCB PCB)
{
    PTNodePtr newNode = (PTNodePtr)(malloc(sizeof(PTNode)));

    // Check for memory allocation errors
    if (newNode == NULL)
    {
        printf("Failed to malloc new node in ProcessTable.h! Exiting...\n");
        exit(-1);
    }

    // Initialize node data
    newNode->nextBucket = NULL;
    newNode->nextItem = NULL;
    newNode->PCB = PCB;

    return newNode;
}

// Function to insert a PTNode into the 2D linked list
void InsertPTNode(PTNodePtr* head, PTNodePtr newNode)
{
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    PTNodePtr p = *head;
    while (p->nextBucket != NULL && p->PCB.arrivalTime != newNode->PCB.arrivalTime)
    {
        p = p->nextBucket;
    }

    if (p->PCB.arrivalTime == newNode->PCB.arrivalTime)
    {
        while (p->nextItem != NULL)
        {
            p = p->nextItem;
        }
        p->nextItem = newNode;
        return;
    }

    if (p->nextBucket == NULL)
    {
        p->nextBucket = newNode;
    }
}

// Function to fetch a process with a given arrival time
PCB FetchProcessWithArrivalTime(PTNodePtr* head, int arrivalTime)
{
    PCB retVal = CreatePCB("", -1, 0, 0, 0, 0, 0, 0);

    if (*head == NULL)
    {
        return retVal;
    }

    PTNodePtr prevNode = NULL;
    PTNodePtr curNode = *head;

    while (curNode != NULL && curNode->PCB.arrivalTime != arrivalTime)
    {
        prevNode = curNode;
        curNode = curNode->nextBucket;
    }

    if (curNode == NULL)
    {
        return retVal;
    }

    if (curNode == *head)
    {
        retVal = (*head)->PCB;
        if ((*head)->nextItem == NULL)
        {
            *head = (*head)->nextBucket;
        }
        else
        {
            *head = (*head)->nextItem;
            (*head)->nextBucket = curNode->nextBucket;
        }
        free(curNode);
        return retVal;
    }

    if (curNode->nextItem == NULL)
    {
        retVal = curNode->PCB;
        prevNode->nextBucket = curNode->nextBucket;
        free(curNode);
        return retVal;
    }

    retVal = curNode->PCB;
    curNode->nextItem->nextBucket = curNode->nextBucket;
    prevNode->nextBucket = curNode->nextItem;

    curNode->nextBucket = NULL;
    curNode->nextItem = NULL;
    free(curNode);

    return retVal;
}

// Function to print the contents of a bucket with a specified arrival time
void PrintContentsOfBucketWithArrivalTime(PTNodePtr head, int arrivalTime)
{
    if (head == NULL)
    {
        printf("Trying to print empty table...\n");
        return;
    }

    PTNodePtr p = head;
    while (p != NULL && p->PCB.arrivalTime != arrivalTime)
    {
        p = p->nextBucket;
    }

    if (p == NULL)
    {
        return;
    }

    while (p != NULL)
    {
        PrintPCB(p->PCB);
        p = p->nextItem;
    }
}

// Function to print all the buckets in the table
void PrintBuckets(PTNodePtr head)
{
    printf("BUCKETS:\n");

    PTNodePtr p = head;
    while (p != NULL)
    {
        printf("%i\n", p->PCB.arrivalTime);
        p = p->nextBucket;
    }
}

// Function to free the entire table
void FreeTable(PTNodePtr* head)
{
    if (*head == NULL)
    {
        return;
    }

    FreeTable(&(*head)->nextBucket);
    FreeTable(&(*head)->nextItem);

    free(*head);
}
