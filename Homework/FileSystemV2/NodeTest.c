#include <stdio.h>
#include <stdlib.h>

#include "FSNode.h"

int main()
{
    FSNodePtr root = CreateNode("/", DIRECTORY, NULL);
    FSNodePtr dir1 = CreateNode("dir1", DIRECTORY, root);

    PrintNode(root);
    PrintNode(dir1);

    FreeNode(dir1);
    FreeNode(root);
}