#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//function that calculates the index in 1D array from x, y index and the column size 
#define IDX(bitMap, x, y) ((x) * (bitMap->columns) + (y))

//enum for the options of the program
enum programOptions {Help, Test, Hline, Vline, Square};

struct bitMapStruct{
    int rows; // number of rows in a bitMap
    int columns; // number of columns in a bitMap
    int *data; // 2D bitMap saved as a 1D array
};

struct coordinates{
    int X; // index of the row 
    int Y;// index of the column
};

void setCoordinates(struct coordinates *inp, int X, int Y)
{
    //get row and column numbers and assign them to the coordinates structure

    inp->X = X; //assign X
    inp->Y = Y; //assign Y
}

int bitMapCtor(struct bitMapStruct *bitMap,int rowsNum, int columsNum)
{
    //allocate the memory for the bitMap and assign rows and columns 
    bitMap->rows = rowsNum;
    bitMap->columns = columsNum;
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
    //open a file from the filePath
    FILE *fptr;
    fptr = fopen(filePath, "r");

    //if the file wasnt opened properly return 1
    if(fptr == NULL)
        return 1;
    
    //read the numbers of rows and colums from the first line of the file
    int rowsNum = 0, columnsNum  = 0;
    fscanf(fptr,"%d %d", &rowsNum, &columnsNum);

    //create a bitMap and assign rows and columns
    bitMapCtor(bitMap, rowsNum, columnsNum);

    //count the loaded bits, so we know if we had loaded all of them
    int loadedBits = 0;
    char readChar;
    //read chars while we dont have a whole bitMap or while we dont get to the EOF
    while (loadedBits < (bitMap->rows * bitMap->columns) && (readChar = getc(fptr)) != EOF)
    {
        //skip the white chars
        if(!isspace(readChar))
        {
            //change char to the numeric value
            int bit = atoi(&readChar);
            //if the bit is not 1 or 0 return error
            if(bit < 0 || bit > 1)
            {
                fclose(fptr);
                return 1;
            }
            bitMap->data[loadedBits] = bit; //load the bitMap
            loadedBits++; //increase the number of loadedBits
        }
    };

    //if the bitMap wasnt loaded properly return error
    if(loadedBits != bitMap->rows * bitMap->columns)
    {
        fclose(fptr);
        return 1;
    }

    //if the loading was successful close the file steam
    fclose(fptr);
    return 0;
}
    
int assignInputToEmun(char ArgInp[])
{   
    //assign number to match the inputed argument
    const char stringMapping[5][7] = {"--help","test","hline","vline","square"};
    for (int idx = 0; idx < 5; idx++)
    {
        // if we find a match return his number
        if(!strcmp(ArgInp, stringMapping[idx]))
            return idx;
    }
    //else return -1
    return -1;
}

void printHelp()
{
    //print the instructions of the program
    fprintf(stdout, "Welcome to figsearch program!\n");
    fprintf(stdout, "The options are:\n");
    //help explanation
    fprintf(stdout, "%d.) Pass \"--help\" as the first argument\n", Help);
    fprintf(stdout, "\tThat will dispaly you the posibibities of the program.\n");
    fprintf(stdout, "\n");
    //test explanation
    fprintf(stdout, "%d.) Pass \"test\" as the first argument and path to a bitmap as a second.\n", Test);
    fprintf(stdout, "\tThat will test the correctness of the bitmap located in the file\n");
    fprintf(stdout, "\tand will print \"Valid\" or \"Invalid\" according to the test results.\n");
    fprintf(stdout, "\n");
    //hline explanation
    fprintf(stdout, "%d.) Pass \"hline\" as the first argument and path to a bitmap as a second\n", Hline);
    fprintf(stdout, "\tThat will test the correctness of the bitmap located in the file\n");
    fprintf(stdout, "\tand will find the longest horizontal line in the bitmap.\n");
    fprintf(stdout, "\tThe program than outputs the coordinates of the start and the end of the line.\n");
    fprintf(stdout, "\n");
    //vline explanation
    fprintf(stdout, "%d.) Pass \"vline\" as the first argument and path to a bitmap as a second\n", Vline);
    fprintf(stdout, "\tThat will test the correctness of the bitmap located in the file\n");
    fprintf(stdout, "\tand will find the longest vertical line in the bitmap.\n");
    fprintf(stdout, "\tThe program than outputs the coordinates of the start and the end of the line.\n");
    fprintf(stdout, "\n");
    //square explanation
    fprintf(stdout, "%d.) Pass \"vline\" as the first argument and path to a bitmap as a second\n", Square);
    fprintf(stdout, "\tThat will test the correctness of the bitmap located in the file\n");
    fprintf(stdout, "\tand will find the biggest square in the bitmap.\n");
    fprintf(stdout, "\tThe program than outputs the coordinates of the top left and the bottom right corners of the square.\n");
    fprintf(stdout, "\n");
}

void findLine(struct bitMapStruct *bitMap, int isHorizontal)
{
    //find the longest line in the bitMap
    //isHorizontal changes the bitMap orientation

    //variables for the coordinates of the start and end of the line
    struct coordinates start, end;

    int maxLength = 0; // value that the function will return

    //set the size of the outer and inner loop
    int outerLoopSize = isHorizontal ? bitMap->rows : bitMap->columns;
    int innerLoopSize = isHorizontal ? bitMap->columns : bitMap->rows;

    //check every line for the longest one
    for (int lineIdx = 0; lineIdx < outerLoopSize; lineIdx++)
    {
        int length = 0;
        struct coordinates lineStart;
        //check each bit in the line
        for (int bitIdx = 0; bitIdx < innerLoopSize; bitIdx++)
        {
            //get the value of the bit
            int bitValue = isHorizontal ? bitMap->data[IDX(bitMap, lineIdx, bitIdx)] 
            : bitMap->data[IDX(bitMap, bitIdx, lineIdx)];

            //if the value is 0 or it is the end of the line we need to check the length of the line
            if(!bitValue || bitIdx == innerLoopSize - 1)
            {
                //if it is bigger set a new max length
                if(length > maxLength)
                {
                    maxLength = length;
                    
                    //assign the start and end coordinates of the line according to the orientation
                    if(isHorizontal)
                    {
                        setCoordinates(&start, lineStart.X, lineStart.Y);
                        setCoordinates(&end, lineIdx, bitIdx);
                    }
                    else
                    {
                        setCoordinates(&start, lineStart.Y, lineStart.X);
                        setCoordinates(&end, bitIdx, lineIdx);
                    }
                }
                //reset the length
                length = 0;
            }
            else
            {
                //check the start position of the line and assign according tothe orientation
                int startPosition = isHorizontal ? lineStart.X : lineStart.Y;
                if(startPosition + length != bitIdx)
                {
                    if (isHorizontal)
                        setCoordinates(&lineStart, lineIdx, bitIdx - length);
                    else
                        setCoordinates(&lineStart, bitIdx - length, lineIdx);
                }
                length++;
            }
        }
    }
    // if there was no line in the bitMap print "Not found"
    if(maxLength == 0)
        fprintf(stdout, "Not found\n");
    //else print the start and end position of the line
    else
        fprintf(stdout, "%d %d %d %d\n", start.X, start.Y, end.X, end.Y);
}

int isSquare(struct bitMapStruct *bitMap, struct coordinates *topLeftCorner, struct coordinates *bottomRightCorner)
{
    //calculate the potencial size of the square
    int size = bottomRightCorner->X - topLeftCorner->X + 1;

    //check if all of potentional squares borders contains only 1 
    for (int offset  = 0; offset  < size; offset++)
    {
        //from the top left corner to the rigth
        if(bitMap->data[IDX(bitMap, topLeftCorner->X, topLeftCorner->Y + offset)] == 0)
            return 0;
        //from the top left corner down
        if(bitMap->data[IDX(bitMap, topLeftCorner->X + offset, topLeftCorner->Y)] == 0)
            return 0;
        //from the bottom rigth corner to the left
        if(bitMap->data[IDX(bitMap, bottomRightCorner->X + offset, bottomRightCorner->Y - offset)] == 0)
            return 0;
        //from the bottom rigth corner up
        if(bitMap->data[IDX(bitMap, bottomRightCorner->X - offset, bottomRightCorner->Y)] == 0)
           return 0;
    }

    // return the size of the square
    return size;    
}

void findSquare(struct bitMapStruct *bitMap)
{
    //variables to help with the finding of the biggest square
    int maxSquare = 0;
    struct coordinates testStart, testEnd;

    //variables for the coordinates of the start and end of the square
    struct coordinates start, end;

    //chcek every bit of the map
    for (int i = 0; i < bitMap->rows; i++)
    {
        for (int e = 0; e < bitMap->columns; e++)
        {
            //if it equals to 0 skip it
            if(bitMap->data[IDX(bitMap, i, e)])
            {
                //set the top right corner of the square and calculate its max posible size
                setCoordinates(&testStart, i, e);
                int maxPosibleSquare = bitMap->columns - e < bitMap->rows - i ? bitMap->columns - e : bitMap->rows - i;
                int squareSize = 0;

                //if the max posible size is smaller that the actual max size in the bitMap skip the bit
                if(maxSquare < maxPosibleSquare)
                {
                    //check every posible left bottom corner of the square that can be created from the bit
                    for (int a = maxSquare; a < maxPosibleSquare; a++)
                    {
                        //if it equals to 0 skip it
                        if(bitMap->data[IDX(bitMap,i + a, e + a)])
                        {
                            //set the atribute for the isSquare function
                            setCoordinates(&testEnd, i + a, e + a);
                            
                            //check if the bits create a square
                            squareSize = isSquare(bitMap, &testStart, &testEnd);

                            //if the sqares size is bigger change the actual max and assign the start and end 
                            if(squareSize > maxSquare)
                            {
                                maxSquare = squareSize;
                                setCoordinates(&start,testStart.X, testStart.Y);
                                setCoordinates(&end, testEnd.X, testEnd.Y);
                            }
                        }
                    }
                }
            }
        }
    }
    // if there was no line in the bitMap print "Not found"
    if(maxSquare == 0)
        fprintf(stdout, "Not found\n");
    //else print the start and end position of the square
    else
        fprintf(stdout, "%d %d %d %d\n", start.X, start.Y, end.X, end.Y);
}

int manageInput(char ArgInp[], char filePath[])
{
    //manage the arguments given to the program
    switch (assignInputToEmun(ArgInp))
    {
        //create the bitmap 
        struct bitMapStruct bitMap;

        //print the instructions to the program
        case Help:
            printHelp();
            return 0;
            break;
        
        //test the inputed file
        case Test:
            //return Valid or Invalid based on the tests results
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
        //find the longest horizontal line
        case Hline:
            //test and load the bitMap
            if(!bitMapLoad(&bitMap,filePath))
            {
                //find the longest line in the horizontal orintation
                findLine(&bitMap, 1);
                return 0;
            }
            else
            {
                //when there is a problem with the bitMap loading print an error
                fprintf(stderr, "Problem with loading of the bitMap!\n");
                return 1;
            }
            break;
        //find the longest vertical line
        case Vline:
            //test and load the bitMap
            if(!bitMapLoad(&bitMap,filePath))
            {
                //find the longest line in the vertical orintation
                findLine(&bitMap, 0);
                return 0;
            }
            else
            {
                //when there is a problem with the bitMap loading print an error
                fprintf(stderr, "Problem with loading of the bitMap!\n");
                return 1;
            }
            break;
        //find the biggest square in the bitMap
        case Square:
            //test and load the bitMap
            if(!bitMapLoad(&bitMap,filePath))
            {
                //find the squares coordinates
                findSquare(&bitMap);
                return 0;
            }
            else
            {
                //when there is a problem with the bitMap loading print an error
                fprintf(stderr, "Problem with loading of the bitMap!\n");
                return 1;
            }
            break;
        //if the arguments dont match the programs options print a error
        default:
            fprintf(stderr, "Wrong inputed arguments!\n");
            return 1;
            break;
    }
}

int main(int argc, char *argv[])
{
    //check the number of arguments
    if(argc > 1)
    {
        //if the number of arguments is 2 set the second parameter as empty string
        if(argc == 2)
            return manageInput(argv[1], "");
        //else send the arguments to the manageInput function
        else
            return manageInput(argv[1],argv[2]);
    }
    //is there is wrong number of arguments print error
    else
    {
        fprintf(stderr, "Wrong number of given arguments!\n");
        return 1;
    }
}