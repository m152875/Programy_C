#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum programOptions {Help, Test, Hline, Vline, Square};

struct bitMapStruct{
    int rows;
    int columns;
    int *data;
};
struct coordinates{
    int X;
    int Y;
};

void setCoordinates(struct coordinates *inp, int X, int Y)
{
    inp->X = X;
    inp->Y = Y;
}

int bitMapCtor(struct bitMapStruct *bitMap,int r, int c)
{
    bitMap->rows = r;
    bitMap->columns = c;
    fprintf(stdout,"l: %d, w: %d\n",bitMap->rows, bitMap->columns);
    bitMap->data = malloc((bitMap->rows * bitMap->columns) * sizeof(int));
    if(bitMap->data == NULL)
    {
        bitMap->rows = 0;
        bitMap->columns = 0;
        return 0;
    }   
    return 1;
     
}

int bitMapLoad(struct bitMapStruct *bitMap, char filePath[])
{
    //read the lenght and the width from the file
    FILE *fptr;
    fptr = fopen(filePath, "r");
    if(fptr == NULL)
    {
        return 1;
    }
    int rows = 0, columns  = 0;
    fscanf(fptr,"%d %d", &rows, &columns);
    bitMapCtor(bitMap, rows, columns);
    int loadedBits = 0;
    char c;
    while (loadedBits < (bitMap->rows * bitMap->columns) && (c = getc(fptr)) != EOF)
    {
        if(!isspace(c))
        {
            int bit = atoi(&c);
            if(bit < 0 || bit > 1)
            {
                fclose(fptr);
                return 1;
            }
            bitMap->data[loadedBits] = bit;
            loadedBits++;
        }
    };

    if(loadedBits != bitMap->rows * bitMap->columns)
    {
        fclose(fptr);
        return 1;
    }
    fclose(fptr);
    return 0;
}
    
int assignInputToEmun(char ArgInp[])
{   
    const char stringMapping[5][7] = {"--help","test","hline","vline","sqare"};
    for (int idx = 0; idx < 5; idx++)
    {
        if(!strcmp(ArgInp, stringMapping[idx]))
            return idx;
    }
    return -1;
}

int printHelp()
{
    fprintf(stdout, "Welcome to figsearch program!\n");
    fprintf(stdout, "The options are:\n");
    fprintf(stdout, "\"--help\" that dispalys the posibibities of the program.\n");
    fprintf(stdout, "Welcome to figsearch program!");
    fprintf(stdout, "Welcome to figsearch program!");
    fprintf(stdout, "Welcome to figsearch program!");
    fprintf(stdout, "Welcome to figsearch program!\n");

    return 0;
}

int lineLength(struct bitMapStruct *bitMap, struct coordinates *start, struct coordinates *end,int orientation)
{
    int maxLength = 0;
    int r, s;

    if(orientation)
    {
        r = bitMap->rows;
        s = bitMap->columns;
    }
    else
    {
        s = bitMap->rows;
        r = bitMap->columns;
    }

    for (int rowIdx = 0; rowIdx < r; rowIdx++)
    {
        int length = 0;
        struct coordinates rowStart;
        for (int bitIdx = 0; bitIdx < s; bitIdx++)
        {
            if(!bitMap->data[(rowIdx * s) + bitIdx] || bitIdx == s - 1)
            {
                if(length > maxLength)
                {
                    maxLength = length;
                    setCoordinates(start, rowStart.X, rowStart.Y);
                    setCoordinates(end, rowIdx, bitIdx);
                }
                length = 0;
            }
            else
            {
                if(rowStart.Y + length != bitIdx)
                {
                    setCoordinates(&rowStart, rowIdx, bitIdx - length);
                }
                length++;
            }
        }
    }
    fprintf(stdout, "(%d,%d) -- (%d,%d)\n",start->X,start->Y, end->X,end->Y);
    return 0;
}

int manageInput(char ArgInp[], char filePath[])
{

    switch (assignInputToEmun(ArgInp))
    {
        struct bitMapStruct bitMap;
        struct coordinates start, end;
        case Help:
            return printHelp();
            break;

        case Test:
            if(!bitMapLoad(&bitMap,filePath))
            {
                fprintf(stdout,"Valid\n");
                return 0;
            }
            else
            {
                fprintf(stderr, "Invalid\n");
                return 1;
            }
            break;

        case Hline:
            if(!bitMapLoad(&bitMap,filePath))
                return lineLength(&bitMap, &start, &end, 1);
            else
                return 0;
            break;

        case Vline:
            if(!bitMapLoad(&bitMap,filePath))
                return lineLength(&bitMap, &start, &end, 0);
            else
                return 0;
            break;

        default:
            return 1;
            break;
    }
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(argc == 2)
            return manageInput(argv[1], "");
        else
            return manageInput(argv[1],argv[2]);
    }
    return 0;
}