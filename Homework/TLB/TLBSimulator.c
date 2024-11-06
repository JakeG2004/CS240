#include <stdio.h>
#include <stdlib.h>

#include "LRUQueue.h"
#include "TLB.h"
#include "pageTable.h"

int CalculatePage(int vAddr);
int TranslateAddress(int vAddr, int pageTable[], TLB_Node TLB[]);

int PAGE_SIZE = 4096;
int curTLBSize = 0;

int numHits;
int numMisses;

NodePtr usageQueue = NULL;

int main()
{
    srand(1057);
    
    TLB_Node TLB[100];
    int pageTable[10000];

    InitPageTable(pageTable);
    ClearTLB(TLB);

    int numCycles = 1000;

    for(int i = 0; i < numCycles; i++)
    {
        int newAddr = TranslateAddress(rand() % (PAGE_TABLE_MAX * 100), pageTable, TLB);
        printf("Physical Address: %i\n", newAddr);
    }

    printf("\n===== RESULTS =====\n");

    printf("Hits: %i\n", numHits);
    printf("Misses: %i\n", numMisses);
    printf("Hitrate: %f\n", 100 * (double)(numHits) / (double)(numCycles));
}

int CalculatePage(int vAddr)
{
    // Make it divisible by PAGE_SIZE
    int tmp = vAddr % PAGE_SIZE;
    tmp = vAddr - tmp;

    //printf("tmp: %i\n", tmp);
    // Divide, and return the value
    tmp = tmp / PAGE_SIZE;

    return tmp;
}

int TranslateAddress(int vAddr, int pageTable[], TLB_Node TLB[])
{
    int retVal = 0;
    int page = CalculatePage(vAddr);
    int offset = vAddr % PAGE_SIZE;

    if(CheckForPageInTLB(TLB, page))
    {
        printf("TLB hit!\n");
        numHits++;
        retVal = GetAddressFromTLB(TLB, page, offset, PAGE_SIZE);
        UpdateUsageQueue(&usageQueue, page);
        return retVal;
    }

    numMisses++;
    printf("TLB Miss!\n");

    // Create the new node
    TLB_Node newTLBEntry;
    newTLBEntry.pFrame = FetchPFrameFromPageTable(pageTable, page);
    newTLBEntry.vPage = page;

    // check for invalid
    if(newTLBEntry.pFrame == -1)
    {
        printf("ERROR!!\n");
        return -1;
    }

    // Insert if there is room
    if(curTLBSize < TLB_MAX)
    {
        InsertIntoTLB(TLB, newTLBEntry, &curTLBSize);
        retVal = GetAddressFromTLB(TLB, page, offset, PAGE_SIZE);
        UpdateUsageQueue(&usageQueue, page);
        printf("Added entry to TLB\n");
        return retVal;
    }

    // Page fault
    printf("Page fault!\n");

    // Get the least recently used page
    int pageToReplace;
    if(GetLastItem(&usageQueue, &pageToReplace) == 0)
    {
        printf("ERROR2!!\n");
    }

    if(!PageFault(TLB, newTLBEntry, pageToReplace))
    {
        printf("ERROR3!!\n");
    }

    UpdateUsageQueue(&usageQueue, page);

    retVal = GetAddressFromTLB(TLB, page, offset, PAGE_SIZE);
    return retVal;
}

