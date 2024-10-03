#include <stdio.h>
#include "PCB.h"
#include "PCBQueue.h"

int main()
{
    QNodePtr head = NULL;
    printf("\n\nadding {1...10} to the queue\n\n");

    for(int i = 0 ; i < 10; i++)
    {
        Enqueue(&head, CreatePCB("testProcess", i, i, i));
    }

    printf("\n\nPRINTING QUEUE\n\n");

    PrintQueue(head);
    printf("Queue size is %i\n", SizeOfQueue(head));

    printf("\n\nDELETING FROM QUEUE\n\n");

    printf("\n\nThe first value in the queue is: ");
    PrintPCB(Dequeue(&head));

    printf("\n\nThe new queue is\n\n");
    PrintQueue(head);

    printf("Queue size is %i\n", SizeOfQueue(head));

    FreeQueue(&head);
}