#include <stdio.h>
#include <stdlib.h>

#include "FSFile.h"

int GetFileFromDir(FSFilePtr curDir, char* fileName, FSFilePtr* result);
int DeleteFileByName(char* fileName, FSFilePtr* curDir);

/*
TODO:
move
tree
*/

FSFilePtr CreateFS()
{
    FSFilePtr newFile = (FSFilePtr)malloc(sizeof(FSFile));

    // Fill data with the root
    newFile -> name = "/";
    newFile -> nextFile = NULL;
    newFile -> parent = NULL;
    newFile -> type = DIRECTORY;

    return newFile;
}

int InsertIntoDir(FSFilePtr* parent, FSFilePtr child)
{
    child->parent = *parent;
    child->nextFile = NULL;

    // If the directory is empty, attach as the first child
    if ((*parent)->nextFile == NULL)
    {
        (*parent)->nextFile = child;
        return 1;
    }

    FSFilePtr p = (*parent)->nextFile;

    // Traverse to the end of the list
    while (p->nextFile != NULL)
    {
        p = p->nextFile;
    }

    p->nextFile = child;  // Append at the end
    return 1;
}


int MakeDirectory(FSFilePtr* parent, char* dirName)
{
    FSFilePtr newDir = CreateFile(DIRECTORY, dirName, *parent);
    return InsertIntoDir(parent, newDir);
}

int MakeFile(FSFilePtr* parent, char* name)
{
    FSFilePtr tmp = CreateFile(FSFILE, name, *parent);
    return InsertIntoDir(parent, tmp);
}


int SearchDir(FSFilePtr dir, char* query)
{
    FSFilePtr p = dir;

    while(p != NULL)
    {
        if(strcmp(query, p -> name) == 0)
        {
            printf("Found file %s\n", query);
            return 1;
        }

        p = p -> nextFile;
    }

    printf("Failed to find file %s\n", query);
    return 0;
}

int MoveFile(char* fileName, FSFilePtr* sourceDir, FSFilePtr* destDir)
{
    FSFilePtr fileToMove = NULL;
    if (GetFileFromDir(*sourceDir, fileName, &fileToMove) == 0)
    {
        printf("Failed to find %s in source directory.\n", fileName);
        return 0;
    }

    // Detach from source
    if (DeleteFileByName(fileName, sourceDir) == 0)
    {
        printf("Failed to delete %s from source.\n", fileName);
        return 0;
    }

    // Insert into destination
    if (InsertIntoDir(destDir, fileToMove) == 0)
    {
        printf("Failed to insert %s into destination.\n", fileName);
        return 0;
    }

    return 1;
}



int GetFileFromDir(FSFilePtr curDir, char* fileName, FSFilePtr* result)
{
    if (curDir == NULL || fileName == NULL || result == NULL)
    {
        return 0;
    }

    FSFilePtr p = curDir;

    while (p != NULL)
    {
        if (strcmp(p -> name, fileName) == 0)
        {
            *result = p;  // Update caller's pointer correctly.
            return 1;
        }

        p = p -> nextFile;
    }

    *result = NULL;  // File not found.
    return 0;
}


int DeleteFileByName(char* fileName, FSFilePtr* curDir)
{
    if (curDir == NULL || *curDir == NULL) 
    {
        return 0;  // Directory is empty or invalid
    }

    FSFilePtr current = *curDir;
    FSFilePtr prev = NULL;

    // Traverse the linked list
    while (current != NULL)
    {
        if (strcmp(current->name, fileName) == 0)
        {
            // Break chain first
            if (prev == NULL)
            {
                // Deleting the first node
                *curDir = current->nextFile;
            }
            else
            {
                // Deleting middle or last node
                prev->nextFile = current->nextFile;
            }

            // Now safe to unlink and delete
            current->nextFile = NULL;
            free(current->name);
            free(current);

            return 1; // Deletion successful
        }

        // Update previous to current, traverse
        prev = current;
        current = current->nextFile;
    }

    return 0;  // File not found
}



void ListDir(FSFilePtr curDir)
{
    FSFilePtr p = curDir;

    // Handle Empty directory
    if(p -> nextFile == NULL)
    {
        printf("Directory %s is an empty directory.\n", curDir -> name);
        return;
    }

    // Go to first child file
    p = p -> nextFile;

    // Print whole linked list
    while(p != NULL)
    {
        printf("%s\n", p -> name);
        p = p -> nextFile;
    }
}