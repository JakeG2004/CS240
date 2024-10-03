#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

struct QNode
{
    PCB PCB;
    struct QNode* next;
};

typedef struct QNode QNode;
typedef struct QNode* QNodePtr;

int Enqueue(QNodePtr* head, PCB PCB)
{
    QNodePtr newNode = (QNodePtr)(malloc(sizeof(QNode)));

    //check errors
    if(newNode == NULL)
    {
        printf("Failed to malloc in InsertQNode\n");
        return 0;
    }

    //fill data
    newNode -> PCB = PCB;
    newNode -> next = NULL;

    QNodePtr p = *head;

    //handle empty list
    if(p == NULL)
    {
        (*head) = newNode;
        return 1;
    }

    //traverse to the end
    while(p -> next != NULL)
    {
        p = p -> next;
    }

    //insert
    p -> next = newNode;

    return 1;
}

PCB Dequeue(QNodePtr* head)
{
    QNodePtr p = *head;

    //grab return value
    PCB retVal = p -> PCB;

    //set head
    (*head) = (*head) -> next;

    //break link and free
    p -> next = NULL;
    free(p);

    return retVal;
}

int SizeOfQueue(QNodePtr head)
{
    int size = 0;
    QNodePtr p = head;

    while(p != NULL)
    {
        size++;
        p = p -> next;
    }

    return size;
}

void PrintQueue(QNodePtr head)
{
    QNodePtr p = head;

    //print each node
    while(p != NULL)
    {
        PrintPCB(p -> PCB);
        p = p -> next;
    }
}

void FreeQueue(QNodePtr* head)
{
    if(*head == NULL)
    {
        return;
    }

    FreeQueue(&(*head) -> next);

    free(*head);
}