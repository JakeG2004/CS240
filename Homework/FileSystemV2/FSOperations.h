#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FSNode.h"

int InsertIntoDir(FSNodePtr* curDir, FSNodePtr newNode);
int MakeNode(char* name, FSNodePtr* parent, int type);
int SearchDir(char* query, FSNodePtr srcDir);
int RemoveFromDirByName(char* query, FSNodePtr* src);
int IsEmptyDir(FSNodePtr dir);
int MoveFile(FSNodePtr* src, FSNodePtr* dst, char* fileName);
int GetFileFromDir(FSNodePtr src, char* fileName, FSNodePtr* result);
void ListDir(FSNodePtr curDir);

int InsertIntoDir(FSNodePtr* curDir, FSNodePtr newNode) // Inserts a node into a directory. 1 on success, 0 on failure
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

int MakeNode(char* name, FSNodePtr* parent, int type) // Makes node and inserts into a directory. 1 on success, 0 on failure
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

int SearchDir(char* query, FSNodePtr srcDir) // Searches a directory by name. 1 on match, 0 on no match
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

int RemoveFromDirByName(char* query, FSNodePtr* src) // Removes from a directory by name. Only removes files and empty directories. 1 on success, 0 on failure
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

int IsEmptyDir(FSNodePtr dir) // Checks if a directory is empty. 1 if directory is empty, 0 otherwise
{
    return (dir -> child == NULL);
}

int MoveFile(FSNodePtr* src, FSNodePtr* dst, char* fileName) // Moves a file from src to dst. 1 if succesful, 0 otherwise.
{
    // Handle null
    if(src == NULL || *src == NULL || dst == NULL || *dst == NULL || fileName == NULL)
    {
        return 0;
    }

    // Check that file exists in source
    if(SearchDir(fileName, *src) == 0)
    {
        return 0;
    }

    // Check that file doesnt exist in source
    if(SearchDir(fileName, *dst) == 1)
    {
        return 0;
    }

    // Get the file
    FSNodePtr fileToMove = NULL;
    if(GetFileFromDir(*src, fileName, &fileToMove) == 0)
    {
        free(fileToMove);
        return 0;
    }

    // Remove it from the directory
    if(RemoveFromDirByName(fileName, src) == 0)
    {
        free(fileToMove);
        return 0;
    }

    // Insert into new directory
    if(InsertIntoDir(dst, fileToMove) == 0)
    {
        free(fileToMove);
        return 0;
    }

    return 1;
}

int GetFileFromDir(FSNodePtr src, char* fileName, FSNodePtr* result) 
{
    *result = NULL;

    // Check for null pointers
    if (src == NULL || fileName == NULL)
    {
        return 0;
    }

    // Handle ".." (parent directory) case
    if (strcmp(fileName, "..") == 0)
    {
        if (src->parent == NULL)
        {
            *result = src;
            return 1;
        }

        *result = src->parent;
        return 1;
    }

    // Traverse the child nodes to find the file
    FSNodePtr p = src->child;
    while (p != NULL)
    {
        if (strcmp(p->name, fileName) == 0)
        {
            // Directly assign the found node to result
            *result = p;
            return 1;
        }

        p = p->nextSibling;
    }

    return 0;  // Return 0 if the file is not found
}


int DisplayTree(FSNodePtr root, int depth) // Displays the tree. Returns 0 on fail, 1 on success
{
    // Handle null root case
    if (root == NULL)
    {
        return 0; // Return 0 to indicate failure
    }

    // Print indentation based on the current depth
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    // Print the current directory/file name
    printf("/%s\n", root->name);

    // Traverse the child nodes recursively
    FSNodePtr p = root -> child;
    while (p != NULL)
    {
        // Recursive call to display the children of the current node
        if (DisplayTree(p, depth + 1) == 0)
        {
            return 0; // If recursion fails, return failure
        }
        p = p->nextSibling; // Move to the next sibling
    }

    return 1; // Return success after completing the traversal
}

void ListDir(FSNodePtr curDir) // Lists all nodes in a directory. No return type
{
    FSNodePtr p = curDir -> child;

    if(p == NULL)
    {
        printf("Directory %s is empty\n", curDir -> name);
        return;
    }

    while(p != NULL)
    {
        printf("%s\n", p -> name);
        p = p -> nextSibling;
    }
}