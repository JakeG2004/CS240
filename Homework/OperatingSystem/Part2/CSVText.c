#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSV.h"

int main()
{
    FILE* CSVFile = OpenCSV("TestData.csv");

    char* line = NULL;

    ReadLine(CSVFile, &line);

    for(int i = 0; i < 4; i++)
    {
        printf("Field %i is: %s\n", i, GetField(line, i));
        free(line);
    }

    CloseCSV(CSVFile);
}