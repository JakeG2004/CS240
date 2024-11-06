#include <stdio.h>
#include <stdlib.h>

typedef struct TLB_Node
{
    int vPage;
    int pFrame;
} TLB_Node;

const int TLB_MAX = 100;

int CheckForPageInTLB(TLB_Node TLB[], int page)
{
    // Search TLB
    for(int i = 0; i < TLB_MAX; i++)
    {
        // If Query found, return 1
        if(TLB[i].vPage == page)
        {
            return 1;
        }
    }

    // Otherwise, return 0
    return 0;
}

int InsertIntoTLB(TLB_Node TLB[], TLB_Node page, int* curTLBSize)
{
    // If it's in the TLB already, return 1
    if(CheckForPageInTLB(TLB, page.vPage))
    {
        return 1;
    }

    // Test for full list
    if(*curTLBSize == TLB_MAX)
    {
        return 0;
    }

    (*curTLBSize)++;

    // Insert into TLB
    for(int i = 0; i < TLB_MAX; i++)
    {
        if(TLB[i].vPage == -1)
        {
            TLB[i] = page;
            break;
        }
    }

    return 1;
}

int GetAddressFromTLB(TLB_Node TLB[], int page, int offset, int pageSize)
{
    for(int i = 0; i < TLB_MAX; i++)
    {
        if(TLB[i].vPage == page)
        {
            //printf("TLB page: %i\n", TLB[i].pFrame);
            return((TLB[i].pFrame) + offset);
        }
    }

    return -1;
}

int PageFault(TLB_Node TLB[], TLB_Node newTLBEntry, int pageToReplace)
{
    for(int i = 0; i < TLB_MAX; i++)
    {
        if(TLB[i].vPage == pageToReplace)
        {
            TLB[i] = newTLBEntry;
            return 1;
        }
    }

    return 0;
}

void ClearTLB(TLB_Node TLB[])
{
    TLB_Node tmp;
    tmp.pFrame = -1;
    tmp.vPage = -1;

    for(int i = 0; i < TLB_MAX; i++)
    {
        // -1 on either value indicates that it is free
        TLB[i] = tmp;
    }
}

void PrintTLB(TLB_Node TLB[])
{
    printf("{vPage, vFrame}\n");

    for(int i = 0; i < TLB_MAX; i++)
    {
        printf("{%i, %i}\n", TLB[i].vPage, TLB[i].pFrame);
    }
}