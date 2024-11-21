#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FSOperations.h"

const int EXP_SIZE_MAX = 256;
const int COMM_SIZE = 16;
const int OP_SIZE = 64;

FSNodePtr test = NULL;

enum
{
    MKDIR,
    ADD,
    SEARCH,
    LS,
    RM,
    MV,
    TREE,
    READ,
    QUIT,
    NODE,
    CD,
    ERROR
};

int GetUserExpression(char* command, char* op1, char* op2);
int CommandToEnum(char* command);
int ProcessCommand(int command, char* op1, char* op2, FSNodePtr* curDir, FSNodePtr* root);
int NavigateToDirectory(char* dir, FSNodePtr* curDir, FSNodePtr root);

int main()
{
    FSNodePtr root = CreateNode("root", DIRECTORY, NULL);
    FSNodePtr curDir = root;

    while(1)
    {
        printf("%s > ", curDir -> name);
        char* command = (char*)malloc(sizeof(char) * COMM_SIZE);
        char* op1 = (char*)malloc(sizeof(char) * OP_SIZE);
        char* op2 = (char*)malloc(sizeof(char) * OP_SIZE);

        GetUserExpression(command, op1, op2);

        if(ProcessCommand(CommandToEnum(command), op1, op2, &curDir, &root) == 0)
        {
            printf("Quitting...\n");
            exit(0);
        }
    }
}

int ProcessCommand(int command, char* op1, char* op2, FSNodePtr* curDir, FSNodePtr* root) {
    switch (command) {
        case MKDIR:
            FSNodePtr travNode = *curDir;
            NavigateToDirectory(op1, &travNode, *root);
            MakeNode(op1, &travNode, DIRECTORY);
            break;

        case ADD:
            MakeNode(op1, curDir, FSFILE);
            break;

        case SEARCH:
            SearchDir(op1, *curDir);
            break;

        case LS:
            ListDir(*curDir);
            break;

        case RM:
            RemoveFromDirByName(op1, curDir);
            break;

        case MV:
            NavigateToDirectory(op1, curDir, *root);
            break;

        case TREE:
            DisplayTree(*root, 0);
            break;

        case READ:
            printf("Read\n");
            break;

        case QUIT:
            return 0;

        case NODE:
            PrintNode(*curDir);
            break;

        case CD:
            if(SearchDir(op1, *curDir) == 0 && strcmp(op1, "..") != 0)
            {
                printf("Invalid paath\n");
                break;
            }
            GetFileFromDir(*curDir, op1, curDir);
            break;

        default:
            printf("Unknown command\n");
    }

    return 1;
}

int NavigateToDirectory(char* path, FSNodePtr* curDir, FSNodePtr root) 
{
    // Handle null
    if(path == NULL || curDir == NULL || *curDir == NULL || root == NULL)
    {
        return 0;
    }

    // Get next part of path
    char* nextDir = strtok(path, "/");
    while(nextDir != NULL)
    {
        // Go to next directory if it is a match
        if(SearchDir(nextDir, *curDir) == 1)
        {
            GetFileFromDir(*curDir, nextDir, curDir);
        }

        nextDir = strtok(NULL, "/");
    }

    return 1;
}

int CommandToEnum(char* command)
{
    if(strncmp(command, "mkdir", COMM_SIZE) == 0)
    {
        return MKDIR;
    }

    if(strncmp(command, "add", COMM_SIZE) == 0)
    {
        return ADD;
    }

    if(strncmp(command, "search", COMM_SIZE) == 0)
    {
        return SEARCH;
    }

    if(strncmp(command, "ls", COMM_SIZE) == 0)
    {
        return LS;
    }

    if(strncmp(command, "rm", COMM_SIZE) == 0)
    {
        return RM;
    }

    if(strncmp(command, "mv", COMM_SIZE) == 0)
    {
        return MV;
    }

    if(strncmp(command, "tree", COMM_SIZE) == 0)
    {
        return TREE;
    }

    if(strncmp(command, "read", COMM_SIZE) == 0)
    {
        return READ;
    }

    if(strncmp(command, "quit", COMM_SIZE) == 0)
    {
        return QUIT;
    }

    if(strncmp(command, "node", COMM_SIZE) == 0)
    {
        return NODE;
    }

    if(strncmp(command, "cd", COMM_SIZE) == 0)
    {
        return CD;
    }

    return ERROR;
}

int GetUserExpression(char* command, char* op1, char* op2)
{
    char expression[EXP_SIZE_MAX];
    if(!fgets(expression, EXP_SIZE_MAX, stdin))
    {
        return 0;
    }

    int curIndex = 0;

    // Get command
    for(int i = 0; i < COMM_SIZE; i++)
    {
        if(expression[i] == ' ' || expression[i] == '\n' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }

        command[i] = expression[i];
    }

    // Increment past the space
    curIndex++;

    // Get first operand
    for(int i = curIndex; (i - curIndex) < OP_SIZE; i++)
    {
        if(expression[i] == ' ' || expression[i] == '\n' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }

        op1[i - curIndex] = expression[i];
    }

    // Increment past the space
    curIndex++;

    // Get second operand
    for(int i = curIndex; (i - curIndex) < OP_SIZE; i++)
    {
        if(expression[i] == ' ' || expression[i] == '\n' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }

        op2[i - curIndex] = expression[i];
    }

    return 1;
}