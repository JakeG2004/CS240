#include <stdio.h>
#include <stdlib.h>

#include "FSOperations.h"

int main()
{
    FSNodePtr root = CreateNode("root", DIRECTORY, NULL);
    MakeNode("dir1", &root, DIRECTORY);
    MakeNode("dir2", &root, DIRECTORY);
    MakeNode("file1", &root, FSFILE);
    
    //printf("\n==ROOT==\n");
    //ListDir(root);

    MoveFile(&root, &(root -> child), "file1");
    MakeNode("file2", &root -> child -> child, FSFILE);
    //printf("%i\n", MoveFile(&root, &(root -> child), "file1"));

   // printf("\n==dir1==\n");
    //ListDir(root -> child);

    //PrintNode(root -> child -> child);
    

    DisplayTree(root, 0);
}