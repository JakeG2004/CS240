#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    DIRECTORY,
    FSFILE
};

typedef struct FSFile
{
    int type;
    char* name;
    struct FSFile* parent;
    struct FSFile* nextFile;
} FSFile;

typedef struct FSFile* FSFilePtr;

FSFilePtr CreateFile(int type, char* name, FSFile* parent)
{
    FSFilePtr newFile = (FSFilePtr)malloc(sizeof(FSFile));
    newFile -> type = type;
    newFile -> nextFile = NULL;
    newFile -> parent = parent;

    newFile -> name = (char*)malloc(sizeof(char) * 32);

    // Copy string, handle null terminator
    strncpy(newFile -> name, name, 32);
    newFile -> name[31] = '\0';

    return newFile;
}

void PrintFile(FSFilePtr curFile)
{
    printf("\n==========\n");
    printf("File name: %s\n", curFile -> name);

    printf("File Type: ");
    if(curFile -> type == DIRECTORY)
    {
        printf("directory\n");
    }
    else
    {
        printf("file\n");
    }

    printf("Next File Addr: %p\n", (void*)(curFile -> nextFile));
    printf("Parent Addr: %p\n", (void*)(curFile -> parent));
}

void FreeFile(FSFile* curFile)
{
    free(curFile -> name);
    curFile -> parent = NULL;
    curFile -> nextFile = NULL;
}