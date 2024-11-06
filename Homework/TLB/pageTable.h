#include <stdio.h>
#include <stdlib.h>

const int PAGE_TABLE_MAX = 10000;

int FetchPFrameFromPageTable(int pageTable[], int page)
{
    if(page < 0 || page > PAGE_TABLE_MAX)
    {
        return -1;
    }

    return pageTable[page];
}

void InitPageTable(int pageTable[])
{
    int tmp[PAGE_TABLE_MAX];

    // Initialize tmp with values 0 to PAGE_TABLE_MAX - 1
    for (int i = 0; i < PAGE_TABLE_MAX; i++)
    {
        tmp[i] = i;
    }

    // Shuffle tmp to create a random order of 0 to 9999
    for (int i = PAGE_TABLE_MAX - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int randIndex = rand() % (i + 1);

        // Swap tmp[i] with tmp[randIndex]
        int temp = tmp[i];
        tmp[i] = tmp[randIndex];
        tmp[randIndex] = temp;
    }

    // Copy shuffled values into pageTable
    for (int i = 0; i < PAGE_TABLE_MAX; i++)
    {
        pageTable[i] = tmp[i];
    }
}

void PrintPageTable(int pageTable[])
{
    printf("\nPAGE TABLE:\n");
    for(int i = 0; i < PAGE_TABLE_MAX; i++)
    {
        printf("{%i, %i}\n", i, pageTable[i]);
    }
}