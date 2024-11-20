#include <stdio.h>
#include <stdlib.h>
#include "FSFileTree.h"

int main()
{
    FSFilePtr head = CreateFS();
    MakeDirectory(&head, "testDir");
    MakeDirectory(&head, "testDir2");
    MakeFile(&head, "testFile");

    if (MoveFile("testFile", &head, &(head->nextFile)) == 0)
    {
        printf("Failed to move file\n");
    }

    ListDir(head);            // Lists files in head
    ListDir(head->nextFile);  // Lists files in testDir
}
