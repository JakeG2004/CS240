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

//int GetUserExpression(char* command, char* op1, char* op2);
int GetUserInput(char* expression);
int ParseExpression(char* expression, char* command, char* op1, char* op2);
int CommandToEnum(char* command);
int ProcessCommand(int command, char* op1, char* op2, FSNodePtr* curDir, FSNodePtr* root);
int NavigateToDirectory(char* dir, FSNodePtr* curDir);
int ReadFromFile(char* fileName, FSNodePtr* curDir, FSNodePtr* root);
void GetLastTerm(char* path, char* lastTerm);
void TrimWhitespace(char* str);

int main()
{
    FSNodePtr root = CreateNode("root", DIRECTORY, NULL);
    FSNodePtr curDir = root;

    char* expression = (char*)malloc(sizeof(char) * EXP_SIZE_MAX);

    while(1)
    {
        printf("%s > ", curDir -> name);
        char* command = (char*)malloc(sizeof(char) * COMM_SIZE);
        char* op1 = (char*)malloc(sizeof(char) * OP_SIZE);
        char* op2 = (char*)malloc(sizeof(char) * OP_SIZE);

        //GetUserExpression(command, op1, op2);
        GetUserInput(expression);
        ParseExpression(expression, command, op1, op2);

        if(ProcessCommand(CommandToEnum(command), op1, op2, &curDir, &root) == 0)
        {
            printf("Quitting...\n");
            exit(0);
        }
    }
}

int ProcessCommand(int command, char* op1, char* op2, FSNodePtr* curDir, FSNodePtr* root) 
{
    FSNodePtr travNode = *curDir;
    char* fileName = (char*)malloc(sizeof(char) * OP_SIZE);
    GetLastTerm(op1, fileName);
    switch (command) {
        case MKDIR:
            if(NavigateToDirectory(op1, &travNode) == 0)
            {
                printf("Failed to navigate to directory\n");
            }

            if(MakeNode(fileName, &travNode, DIRECTORY) == 0)
            {
                printf("Failed to make directory\n");
            }
            break;

        case ADD:
            if(NavigateToDirectory(op1, &travNode) == 0)
            {
                printf("Failed to navigate to directory!\n");
            }

            if(MakeNode(fileName, &travNode, FSFILE) == 0)
            {
                printf("Failed to make file!\n");
            }
            break;

        case SEARCH:
            if(SearchDir(op1, *curDir) == 1)
            {
                printf("Found file %s in %s\n", op1, (*curDir) -> name);
            }
            else
            {
                printf("Unable to find file %s in %s\n", op1, (*curDir) -> name);
            }
            break;

        case LS:
            ListDir(*curDir);
            break;

        case RM:
            if(RemoveFromDirByName(op1, curDir, 1) == 0)
            {
                printf("Unable to remove %s from %s\n", op1, (*curDir) -> name);
            }
            break;

        case MV:
            if(NavigateToDirectory(op2, &travNode) == 0)
            {
                printf("Unable to navigate to directory!\n");
            }
            if(MoveFile(curDir, &travNode, op1) == 0)
            {
                printf("Unable to move file %s from %s to %s\n", op1, (*curDir) -> name, travNode -> name);
            }
            break;

        case TREE:
            if(DisplayTree(*root, 0) == 0)
            {
                printf("Unable to display tree!\n");
            }
            break;

        case READ:
            if(ReadFromFile(op1, curDir, root) == 0)
            {
                printf("Unable to read file %s\n", op1);
            }
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

int NavigateToDirectory(char* path, FSNodePtr* curDir) 
{
    // Handle null
    if(path == NULL || curDir == NULL || *curDir == NULL)
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

        else
        {
            break;
        }

        nextDir = strtok(NULL, "/");
    }

    if(nextDir != NULL)
    {
        strcpy(path, nextDir);
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

int GetUserInput(char* expression)
{
    if (!fgets(expression, EXP_SIZE_MAX, stdin))
    {
        return 0; // If reading input fails
    }

    // Remove trailing newline character if it exists
    int len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') 
    {
        expression[len - 1] = '\0';
    }

    return 1; // Success
}

// Function to parse the expression into command, op1, and op2
int ParseExpression(char* expression, char* command, char* op1, char* op2)
{
    // Reset expressions
    for(int i = 0; i < COMM_SIZE; i++)
    {
        command[i] = '\0';
    }

    for(int i = 0; i < OP_SIZE; i++)
    {
        op1[i] = '\0';
        op2[i] = '\0';
    }


    int curIndex = 0;

    // Get command
    for (int i = 0; i < COMM_SIZE; i++)
    {
        if (expression[i] == ' ' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }
        command[i] = expression[i];
    }

    // Increment past the space
    curIndex++;

    // Get first operand
    for (int i = curIndex; (i - curIndex) < OP_SIZE; i++)
    {
        if (expression[i] == ' ' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }
        op1[i - curIndex] = expression[i];
    }

    // Increment past the space
    curIndex++;

    // Get second operand
    for (int i = curIndex; (i - curIndex) < OP_SIZE; i++)
    {
        if (expression[i] == ' ' || expression[i] == '\0')
        {
            curIndex = i;
            break;
        }
        op2[i - curIndex] = expression[i];
    }

    return 1; // Success
}

int ReadFromFile(char* fileName, FSNodePtr* curDir, FSNodePtr* root)
{
    // Open file
    FILE* comList = fopen(fileName, "r");

    if(comList == NULL)
    {
        return 0;
    }

    char* expression = (char*)malloc(sizeof(char) * EXP_SIZE_MAX);
    char* command = (char*)malloc(sizeof(char) * COMM_SIZE);
    char* op1 = (char*)malloc(sizeof(char) * OP_SIZE);
    char* op2 = (char*)malloc(sizeof(char) * OP_SIZE);

    printf("Reading %s\n", fileName);

    // Read each line
    while(fgets(expression, EXP_SIZE_MAX, comList))
    {
        int len = strlen(expression);

        if(expression[len - 1] == '\n')
        {
            expression[len - 1] = '\0';
        }

        //printf("%s\n", expression);

        // Parse and process expression
        ParseExpression(expression, command, op1, op2);
        //printf("Command: %s\n", command);
        //printf("Op1: %s\n", op1);
        //printf("Op2: %s\n", op2);

        //getchar();
        if(ProcessCommand(CommandToEnum(command), op1, op2, curDir, root) == 0)
        {
            printf("Quitting...\n");
            exit(0);
        }

        for(int i = 0; i < OP_SIZE; i++)
        {
            command[i] = '\0';
        }
        
    }

    free(expression);
    free(command);

    // Close the file
    fclose(comList);

    printf("Done readiding %s\n", fileName);
}

void GetLastTerm(char* path, char* lastTerm) 
{
    // Find the last occurrence of '/' or '\\' (for Unix or Windows paths)
    char* lastSlash = strrchr(path, '/');  // For Unix-like systems (e.g., Linux, macOS)
    if (!lastSlash) {
        lastSlash = strrchr(path, '\\');  // For Windows paths
    }

    // If a slash was found, the last term starts right after it
    if (lastSlash) {
        strcpy(lastTerm, lastSlash + 1);
    }
    else {
        // No slash found, the whole string is the last term
        strcpy(lastTerm, path);
    }
}