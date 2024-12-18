#include <stdio.h>
#include <stdlib.h>

#ifndef LRQ_H
#define LRQ_H

typedef struct node
{
    int data;
    struct node* next;
} node;

typedef node* NodePtr;

// Returns 0 on failure, 1 on success
int Enqueue(NodePtr* head, int data)
{
    // Create and initialize new node
    NodePtr newNode = (NodePtr)malloc(sizeof(node));

    if(newNode == NULL)
    {
        return 0;
    }

    newNode -> data = data;
    newNode -> next = NULL;

    // Handle empty list
    if(*head == NULL)
    {
        *head = newNode;
        return 1;
    }

    // Handle list with content
    newNode -> next = *head;

    *head = newNode;

    return 1;
}

// Returns 0 on failure, 1 on success
int GetLastItem(NodePtr* head, int* lastNode)
{
    // Handle empty list
    if(*head == NULL)
    {
        printf("Attempting to get item from empty queue\n");
        *lastNode = -1;
        return 0;
    }

    // Make Traversal nodes
    NodePtr p = *head;
    NodePtr q = (*head) -> next;

    // Handle one element list
    if(q == NULL)
    {
        *lastNode = p -> data;
        *head = NULL;
        return 1;
    }

    // Traverse until q -> next = NULL
    while(q -> next != NULL)
    {
        p = q;
        q = q -> next;
    }

    *lastNode = q -> data;

    // Free memory and break link
    free(q);
    p -> next = NULL;

    return 1;
}

void PrintQueue(NodePtr head)
{
    NodePtr p = head;

    // Handle empty list
    if(p == NULL)
    {
        printf("Empty queue\n");
        return;
    }

    // Travserse and print
    while(p != NULL)
    {
        printf("%i\n", p -> data);
        p = p -> next;
    }
}

int SizeOfQueue(NodePtr head)
{
    int retVal = 0;

    NodePtr p = head;

    // Handle empty list
    if(p == NULL)
    {
        return 0;
    }

    // Travserse and print
    while(p != NULL)
    {
        retVal++;
        p = p -> next;
    }

    return retVal;
}

int RemoveAllInstancesOfN(NodePtr* head, int n)
{
    // Check if the list is empty
    if(*head == NULL)
    {
        return 1;
    }

    // Handle the head node if it contains the value n
    NodePtr temp;

    while (*head != NULL && (*head) -> data == n)
    {
        temp = *head;
        *head = (*head) -> next;
        free(temp);
    }

    // Create traversal nodes
    NodePtr p = *head;
    NodePtr q = (*head) ? (*head) -> next : NULL;

    // Traverse the list
    while (q != NULL)
    {
        // If a match is found, erase it and adjust pointers
        if (q->data == n)
        {
            // Re-link list
            p -> next = q -> next;

            // Break link and free
            q -> next = NULL;
            free(q);

            // Finish linking
            q = p -> next;
        }
        else
        {
            // Increment pointers
            p = q;
            q = p -> next;
        }
    }

    return 1;
}

int UpdateUsageQueue(NodePtr* usageQueue, int usedNum)
{
    RemoveAllInstancesOfN(usageQueue, usedNum);
    Enqueue(usageQueue, usedNum);
}


#endif