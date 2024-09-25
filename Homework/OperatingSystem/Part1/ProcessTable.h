/*
ProcessTable.h
A header file for creating a dynamic 2d array of processes organized based on their arrival time.
Strange amalgamation of a hash map and dynamic array
*/

#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

//file guards
#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

//node for making a 2d linked list
struct PTNode
{
    PCB PCB;
    struct PTNode* nextBucket;
    struct PTNode* nextItem;
};

typedef struct PTNode PTNode;
typedef struct PTNode* PTNodePtr;

PTNodePtr CreatePTNode(PCB PCB)
{
    //create new node
    PTNodePtr newNode = (PTNodePtr)(malloc(sizeof(PTNode)));

    //check for error
    if(newNode == NULL)
    {
        printf("Failed to malloc new node in ProcessTable.h! Exiting...\n");
        exit(-1);
    }

    //fill node data
    newNode -> nextBucket = NULL;
    newNode -> nextItem = NULL;
    newNode -> PCB = PCB;

    return newNode;
}

//create a 2d linked list, with "buckets" representing arrivaltimes. Processes are inserted into the table based on their arrival time
void InsertPTNode(PTNodePtr* head, PTNodePtr newNode)
{
    //handle empty list
    if(*head == NULL)
    {
        *head = newNode;
        return;
    }

    //check buckets for the process arrival time
    PTNodePtr p = *head;
    while(p -> nextBucket != NULL && p -> PCB.arrivalTime != newNode -> PCB.arrivalTime) //exit conditions are p -> next = NULL or p -> arrivalTime = newNode -> arrivaltimed
    {
        p = p -> nextBucket;
    }

    //if found arrivalTime bucket, insert
    if(p -> PCB.arrivalTime == newNode -> PCB.arrivalTime)
    {
        //navigate to bottom of bucket
        while(p -> nextItem != NULL)
        {
            p = p -> nextItem;
        }

        //insert node
        p -> nextItem = newNode;

        return;
    }

    //otherwise, create bucket
    if(p -> nextBucket == NULL)
    {
        p -> nextBucket = newNode;
        return;
    }
}

//returns process with arrival time -1 if failed
PCB FetchProcessWithArrivalTime(PTNodePtr* head, int arrivalTime)
{
    PCB retVal = CreatePCB("", -1, 0, 0);

    //handle empty list
    if(*head == NULL)
    {
        printf("Trying to fetch from empty table...\n");
        return retVal;
    }

    //traverse buckets to find arrival time
    PTNodePtr prevNode = NULL;
    PTNodePtr curNode = *head;

    //navigate through buckets until either at end of list or arrivalTime bucket found
    while(curNode != NULL && curNode -> PCB.arrivalTime != arrivalTime)
    {
        prevNode = curNode;
        curNode = curNode -> nextBucket;
    }

    //bucket not found
    if(curNode == NULL)
    {
        printf("Unable to find process bucket with arrival time %i\n", arrivalTime);
        return retVal;
    }

    //handle bucket is head
    if(curNode == *head)
    {
        //get return value
        retVal = (*head) -> PCB;

        //handle empty bucket
        if((*head) -> nextItem == NULL)
        {
            *head = (*head) -> nextBucket;
        }

        //handle if bucket has entries
        else
        {
            *head = (*head) -> nextItem;
            (*head) -> nextBucket = curNode -> nextBucket;
        }

        free(curNode);

        return retVal;
    }
    
    //if bucket not head and bucket is empty
    if(curNode -> nextItem == NULL)
    {
        retVal = curNode -> PCB;

        prevNode -> nextBucket = curNode -> nextBucket;
        free(curNode);

        return retVal;
    }

    //if bucket non-empty
    else
    {
        //grab return value
        retVal = curNode -> PCB;

        //relink how it should be
        curNode -> nextItem -> nextBucket = curNode -> nextBucket;
        prevNode -> nextBucket = curNode -> nextItem;

        //break links and free
        curNode -> nextBucket = NULL;
        curNode -> nextItem = NULL;
        free(curNode);

        return retVal;
    }
}

void PrintContentsOfBucketWithArrivalTime(PTNodePtr head, int arrivalTime)
{
    //handle empty list
    if(head == NULL)
    {
        printf("Trying to print empty table...\n");
        return;
    }

    //traverse buckets to find arrival time
    PTNodePtr p = head;

    //navigate through the buckets
    while(p != NULL && p -> PCB.arrivalTime != arrivalTime)
    {
        p = p -> nextBucket;
    }

    //bucket not found
    if(p == NULL)
    {
        printf("Unable to find process bucket with arrival time %i\n", arrivalTime);
        return;
    }

    //bucket found
    if(p -> PCB.arrivalTime == arrivalTime)
    {
        //navigate to end
        while(p != NULL)
        {
            PrintPCB(p -> PCB);
            p = p -> nextItem;
        } 
    }

    return;
}

void PrintBuckets(PTNodePtr head)
{
    printf("BUCKETS:\n");

    PTNodePtr p = head;

    while(p != NULL)
    {
        printf("%i\n", p -> PCB.arrivalTime);
        p = p -> nextBucket;
    }
}

void FreeTable(PTNodePtr* head)
{
    if(*head == NULL)
    {
        return;
    }

    FreeTable(&(*head) -> nextBucket);
    FreeTable(&(*head) -> nextItem);

    free(*head);
}

#endif