#include <stdio.h>
#include <stdlib.h>

#include "FSOperations.h"

int main()
{
    FSNodePtr root = CreateNode("/", DIRECTORY, NULL);
    MakeNode("dir1", &root, DIRECTORY);
    MakeNode("dir2", &root, DIRECTORY);
    MakeNode("file1", &root, FSFILE);

    printf("%i\n", RemoveFromDirByName("file1", &root));

    ListDir(root);
}