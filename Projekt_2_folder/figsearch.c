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
    const char stringMapping[5][7] = {"--help","test","hline","vline","square"};
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

int findLine(struct bitMapStruct *bitMap, struct coordinates *start, struct coordinates *end, int isHorizontal)
{
    int maxLength = 0;
    //outer and inner Loop (line, bit)
    int widthLoopSize = isHorizontal ? bitMap->rows : bitMap->columns;
    int lengthLoopSize = isHorizontal ? bitMap->columns : bitMap->rows;

    for (int widthIdx = 0; widthIdx < widthLoopSize; widthIdx++)
    {
        int length = 0;
        struct coordinates lineStart;
        for (int lengthIdx = 0; lengthIdx < lengthLoopSize; lengthIdx++)
        {
            int bitValue = isHorizontal ? bitMap->data[(widthIdx * lengthLoopSize) + lengthIdx] 
            : bitMap->data[(lengthIdx * widthLoopSize) + widthIdx];

            if(!bitValue || lengthIdx == lengthLoopSize - 1)
            {
                if(length > maxLength)
                {
                    maxLength = length;
                    if(isHorizontal)
                    {
                        setCoordinates(start, lineStart.X, lineStart.Y);
                        setCoordinates(end, widthIdx, lengthIdx);
                    }
                    else
                    {
                        setCoordinates(start, lineStart.X, lineStart.Y);
                        setCoordinates(end, lengthIdx, widthIdx);
                    }
                }
                length = 0;
            }
            else
            {
                //set start position
                int startPosition = isHorizontal ? lineStart.X : lineStart.Y;

                if(startPosition + length != lengthIdx)
                {
                    if (isHorizontal)
                        setCoordinates(&lineStart, widthIdx, lengthIdx - length);
                    else
                        setCoordinates(&lineStart, lengthIdx - length, widthIdx);
                }
                length++;
            }
        }
    }
    fprintf(stdout, "(%d,%d) -- (%d,%d)\n",start->X,start->Y, end->X,end->Y);
    return 0;
}

int isSquare(struct bitMapStruct *bitMap, struct coordinates *LU, struct coordinates *RD)
{
    int size = RD->X - LU->X + 1;
    for (int offset  = 0; offset  < size; offset++)
    {
        //vpravo
        if(bitMap->data[(LU->X * bitMap->columns) + LU->Y + offset] == 0)
            return 0;
        //dole
        if(bitMap->data[((LU->X + offset) * bitMap->columns) + LU->Y] == 0)
            return 0;
        //vlavo
        if(bitMap->data[(RD->X * bitMap->columns) + RD->Y - offset] == 0)
            return 0;
        //hore
        if(bitMap->data[((RD->X - offset) * bitMap->columns) + RD->Y] == 0)
           return 0;
    }
    return size;    
}

int square(struct bitMapStruct *bitMap, struct coordinates *start, struct coordinates *end)
{
    int maxSquare = 0;
    struct coordinates testStart, testEnd;
    for (int i = 0; i < bitMap->rows; i++)
    {
        for (int e = 0; e < bitMap->columns; e++)
        {
            if(bitMap->data[(i * bitMap->columns) + e])
            {
                setCoordinates(&testStart, i, e);
                int maxPosibleSquare = bitMap->columns - e < bitMap->rows - i ? bitMap->columns - e : bitMap->rows - i;
                int squareSize = 0;
                if(maxSquare < maxPosibleSquare)
                {
                    for (int a = maxSquare; a < maxPosibleSquare; a++)
                    {
                        if(bitMap->data[((i + a) * bitMap->columns) + a + e])
                        {
                            setCoordinates(&testEnd, i + a, e + a);
                            squareSize = isSquare(bitMap, &testStart, &testEnd);
                            fprintf(stdout, "test: (%d,%d) -- (%d,%d) = %d\n",testStart.X,testStart.Y, testEnd.X,testEnd.Y, squareSize);
                            if(squareSize > maxSquare)
                            {
                                maxSquare = squareSize;
                                setCoordinates(start,testStart.X, testStart.Y);
                                setCoordinates(end, testEnd.X, testEnd.Y);
                            }
                        }
                    }
                }
            }
        }
    }
    fprintf(stdout, "(%d,%d) -- (%d,%d) = %d\n",start->X,start->Y, end->X,end->Y, maxSquare);
    return maxSquare;
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
                return findLine(&bitMap, &start, &end, 1);
            else
                return 0;
            break;

        case Vline:
            if(!bitMapLoad(&bitMap,filePath))
                return findLine(&bitMap, &start, &end, 0);
            else
                return 0;
            break;
        case Square:
            if(!bitMapLoad(&bitMap,filePath))
               return square(&bitMap, &start , &end);
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