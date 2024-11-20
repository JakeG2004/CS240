#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FSNode.h"

int InsertIntoDir(FSNodePtr* curDir, FSNodePtr newNode);
int MakeNode(char* name, FSNodePtr* parent, int type);
int SearchDir(char* query, FSNodePtr srcDir);
int ListDir(FSNodePtr curDir);
int IsEmptyDir(FSNodePtr dir);

int InsertIntoDir(FSNodePtr* curDir, FSNodePtr newNode) // 1 on success, 0 on failure
{
    if(curDir == NULL || *curDir == NULL || newNode == NULL)
    {
        //printf("NULL\n");
        return 0;
    }

    // Set new parent
    newNode -> parent = (*curDir);

    // Handle empty list
    if((*curDir) -> child == NULL)
    {
        //printf("First child\n");
        (*curDir) -> child = newNode;
        return 1;
    }

    // Handle nonempty list
    FSNodePtr p = (*curDir) -> child;

    // Navigate to end of linked list
    while(p -> nextSibling != NULL)
    {
        p = p -> nextSibling;
    }

    // Assign the new node
    p -> nextSibling = newNode;

    return 1;
}

int MakeNode(char* name, FSNodePtr* parent, int type) // 1 on success, 0 on failure
{
    // Handle invalid directory
    if(parent == NULL || *parent == NULL)
    {
        //printf("NULL\n");
        return 0;
    }
    // Don't do anything on existing file
    if(SearchDir(name, *parent) == 1)
    {
        //printf("File overlap\n");
        return 0;
    }

    // Create new node and insert
    FSNodePtr newNode = CreateNode(name, type, NULL);
    return InsertIntoDir(parent, newNode);
}

int SearchDir(char* query, FSNodePtr srcDir)
{
    FSNodePtr p = srcDir -> child;

    // Traverse, looking for name
    while(p != NULL)
    {
        if(strcmp((p -> name), query) == 0)
        {
            return 1;
        }

        p = p -> nextSibling;
    }

    return 0;
}

int RemoveFromDirByName(char* query, FSNodePtr* src)
{
    if(src == NULL || *src == NULL || query == NULL)
    {
        //printf("NULL");
        return 0;
    }

    if(SearchDir(query, *src) == 0)
    {
        //printf("No file found...\n");
        return 0;
    }

    FSNodePtr cur = (*src) -> child;
    FSNodePtr prev = NULL;

    while(cur != NULL)
    {
        // If current file name is not target file name, continue
        if(strcmp(query, cur -> name) != 0)
        {
            // Increment
            prev = cur;
            cur = cur -> nextSibling;
            continue;
        }

        // If found and list is 1 element, then it must be first element
        if(prev == NULL)
        {
            if(cur -> type == DIRECTORY && !IsEmptyDir(cur))
            {
                break;
            }

            // Readjust link and free
            (*src) -> child = cur -> nextSibling;
            FreeNode(cur);
            return 1;
        }

        else
        {
            //printf("not first\n");
            prev -> nextSibling = cur -> nextSibling;
            FreeNode(cur);
            return 1;            
        }

        // Increment
        prev = cur;
        cur = cur -> nextSibling;
    }

    return 0;
}

int IsEmptyDir(FSNodePtr dir)
{
    return (dir -> child == NULL);
}

int ListDir(FSNodePtr curDir)
{
    FSNodePtr p = curDir -> child;

    while(p != NULL)
    {
        printf("%s\n", p -> name);
        p = p -> nextSibling;
    }
}