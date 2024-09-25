#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node* next;
};

typedef struct node* NodePtr;

void Enqueue(int newData, NodePtr* head)
{
    //make new node
    NodePtr newNode = (NodePtr)(malloc(sizeof(struct node)));

    //check for errors
    if(newNode == NULL)
    {
        printf("Failed to malloc new node!\n");
        return;
    }

    //assign node data
    newNode->data = newData;
    newNode->next = NULL;

    //handle empty list
    if(*head == NULL)
    {
        *head = newNode;
        return;
    }

    //navigate to end of list
    NodePtr p = *head;
    while(p->next != NULL)
    {
        p = p->next;
    }

    //insert new node at end of list
    p->next = newNode;
}

void PrintQueue(NodePtr head)
{
    NodePtr p = head;

    while(p != NULL)
    {
        printf("%i\n", p->data);
        p = p->next;
    }
}

int Dequeue(NodePtr* head) 
{
    //handle empty list
    if(*head == NULL)
    {
        printf("Tried to fetch from empty list\n");
        return 0;
    }

    //get return value and reassign head
    NodePtr p = *head;
    int retVal = (*head)->data;
    *head = (*head)->next;

    //break link and delete head
    p->next = NULL;
    free(p);

    return retVal;
}

int GetQueueSize(NodePtr head)
{
    NodePtr p = head;
    int retVal = 0;

    while(p != NULL)
    {
        retVal++;
        p = p->next;
    }

    return retVal;
}

int IsEmpty(NodePtr head)
{
    return(head == NULL);
}
