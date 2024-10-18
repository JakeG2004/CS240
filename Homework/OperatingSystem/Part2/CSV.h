#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CSV_H
#define CSV_H

FILE* OpenCSV(char* filename)
{
    FILE* file = fopen(filename, "r");

    //check for errors
    if(file == NULL)
    {
        printf("Failed to open file %s\n", filename);
        return NULL;
    }

    return file;
}

int ReadLine(FILE* file, char** line)
{
    int bufferLen = 1024;

    //handle file not open
    if(file == NULL)
    {
        printf("Failed to open file\n");
        *line = NULL;
        return 0;
    }

    char* buffer = (char*)malloc(sizeof(char) * 1024);

    if(fgets(buffer, bufferLen, file)) 
    {
        buffer[bufferLen - 1] = '\0';
        line = &buffer;
        return 1;
    }

    return 0;
}

char* GetField(char* line, int fieldIndex) 
{
    //duplicate line because strtok modifies in place
    char* tmp = strdup(line);

    //get first token
    char* token = strtok(tmp, ",");
    
    //get following tokens
    for(int i = 0; token != NULL; i++)
    {
        if(i == fieldIndex)
        {
            //free memory and return
            free(tmp);
            free(token);
            return token;
        }

        //get next token
        token = strtok(NULL, ",");
    }

    //free memory
    free(tmp);
    free(token);
    
    //return NULL if error encountered
    return NULL;
}


void CloseCSV(FILE* file)
{
    if(file != NULL)
    {
        fclose(file);
        return;
    }

    printf("Failed to close file\n");
}

#endif