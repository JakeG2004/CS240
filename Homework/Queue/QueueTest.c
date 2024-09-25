#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

int main() {
    //create head
    NodePtr head = NULL;

    printf("IsEmpty: %i\n", IsEmpty(head));

    //insert some items
    Enqueue(10, &head);
    Enqueue(20, &head);
    Enqueue(30, &head);

    //print the size and the queue
    printf("Queue size: %d\n", GetQueueSize(head));
    PrintQueue(head);
    printf("IsEmpty: %i\n", IsEmpty(head));

    //get a value from the queue
    int val = Dequeue(&head);
    printf("Fetched: %d\n", val);

    //print new queue
    printf("Queue size: %d\n", GetQueueSize(head));
    PrintQueue(head);

    return 0;
}