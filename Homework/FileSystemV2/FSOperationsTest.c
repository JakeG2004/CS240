#include <stdio.h>
#include <stdlib.h>

#include "FSOperations.h"

int main()
{
    FSNodePtr root = CreateNode("/", DIRECTORY, NULL);
    MakeNode("dir1", &root, DIRECTORY);
    MakeNode("dir1", &root, DIRECTORY);
    MakeNode("file1", &root, FSFILE);
    MakeNode("file2", &(root -> child), FSFILE);

    ListDir(root);

    printf("%i\n", RemoveFromDirByName("dir1", &root));

    ListDir(root);
}