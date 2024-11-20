#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int FSNODE_NAME_LEN = 32;

enum
{
    FSFILE,
    DIRECTORY
};

typedef struct FSNode
{
    char* name;
    int type;

    struct FSNode* parent;
    struct FSNode* nextSibling;
    struct FSNode* child;
} FSNode;

typedef struct FSNode* FSNodePtr;

FSNodePtr CreateNode(char* name, int type, FSNodePtr parent)
{
    // Allocate new node
    FSNodePtr newNode = (FSNodePtr)malloc(sizeof(FSNode));

    if(newNode == NULL)
    {
        return newNode;
    }

    // Handle name
    newNode -> name = (char*)malloc(sizeof(char) * FSNODE_NAME_LEN);
    strncpy(newNode -> name, name, 31);
    (newNode -> name)[31] = '\0';

    // Handle other passed params
    newNode -> type = type;
    newNode -> parent = parent;

    // Handle always null params
    newNode -> nextSibling = NULL;
    newNode -> child = NULL;

    return newNode;
}

void FreeNode(FSNodePtr node) // Assumes that links have already been properly broken
{
    // Free name, then free node
    node -> parent = NULL;
    node -> nextSibling = NULL;
    node -> child = NULL;

    free(node -> name);
    free(node);
}

void PrintNode(FSNodePtr curNode)
{
    printf("=====\n");

    printf("Node name: %s\n", curNode -> name);

    // Parent
    if(curNode -> parent == NULL)
    {
        printf("Node parent: %p\n", (void*)(curNode -> parent));
    }

    else
    {
        printf("Node parent: %s\n", (curNode -> parent -> name));
    }

    // Next sibling
    if(curNode -> nextSibling == NULL)
    {
        printf("Node sibling: %p\n", (void*)(curNode -> nextSibling));
    }

    else
    {
        printf("Node sibling: %s\n", (curNode -> nextSibling -> name));
    }

    // Child
    if(curNode -> child == NULL)
    {
        printf("Node child: %p\n", (void*)(curNode -> child));
    }

    else
    {
        printf("Node child: %s\n", (curNode -> child -> name));
    }

    // Type
    printf("Node type: ");
    if(curNode -> type == FSFILE)
    {
        printf("File\n");
    }

    else
    {
        printf("Directory\n");
    }
}