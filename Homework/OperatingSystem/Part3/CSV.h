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

    char* buffer = (char*)malloc(sizeof(char) * bufferLen);

    if(fgets(buffer, bufferLen, file)) 
    {
        buffer[bufferLen - 1] = '\0';

        // Replace newline with \0
        for(int i = 0; i < bufferLen; i++)
        {
            if(buffer[i] == '\n')
            {
                buffer[i] = '\0';
            }
        }

        *line = buffer;
        return 1;
    }

    free(buffer);

    return 0;
}

char* GetField(char* line, int fieldIndex) 
{
    // Duplicate line because strtok modifies in place
    char* tmp = strdup(line);
    if (tmp == NULL) {
        return NULL; // strdup failed
    }

    // Get the first token
    char* token = strtok(tmp, ",");

    // Iterate through tokens
    for (int i = 0; token != NULL; i++) 
    {
        if (i == fieldIndex) 
        {
            free(tmp); // Free the duplicated line

            return token; // Return the copied field
        }

        // Get the next token
        token = strtok(NULL, ",");
    }

    // Free memory if fieldIndex is out of bounds
    free(tmp);
    
    // Return NULL if fieldIndex is out of bounds
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