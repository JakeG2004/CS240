#include <stdio.h>
#include <stdlib.h>
#include "LRUQueue.h"

int main()
{
    NodePtr head = NULL;

    PrintQueue(head);

    for(int i = 0; i < 10; i++)
        Enqueue(&head, i % 3);

    PrintQueue(head);

    RemoveAllInstancesOfN(&head, 0);

    printf("fdsa\n");

    PrintQueue(head);
}