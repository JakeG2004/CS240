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
    int bufferSize = 1024;

    if(!file)
    {
        *line = NULL;
        return 0;
    }

    char* buffer = (char*)malloc(sizeof(char) * bufferSize);

    //read line
    if(fgets(buffer, bufferSize, file) != NULL)
    {
        int len = strlen(buffer);

        //replace newline with null terminator
        if(len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        *line = buffer;
        return 1;
    }
}

char* GetField(char* line, int fieldIndex) 
{
    //duplicate the line string
    char* tmp = strdup(line);

    char* token;
    int currentField = 0;
    
    //grab first token
    token = strtok(tmp, ",");

    //while token is valid
    while (token != NULL) 
    {
        if (currentField == fieldIndex) {
            //keep the token
            char* result = strdup(token);
            
            //free and return the result
            free(tmp);
            return result;
        }

        //increment and grab next token
        token = strtok(NULL, ",");
        currentField++;
    }
    
    //free and return
    free(tmp);
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