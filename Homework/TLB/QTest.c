#include <stdio.h>
#include <stdlib.h>
#include "LRUQueue.h"

int main()
{
    NodePtr head = NULL;

    PrintQueue(head);

    for(int i = 0; i < 10; i++)
        Enqueue(&head, i);

    PrintQueue(head);
    printf("Q Size: %i\n", SizeOfQueue(head));

    int lastVal = 100;

    GetLastItem(&head, &lastVal);

    printf("Last item is %i\n", lastVal);

    PrintQueue(head);
}