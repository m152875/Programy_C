#include <stdio.h>
#include <string.h>
#include <ctype.h>

char strToInt(char str)
{
    char res = '\0';
        if((str >= 'a' && str <= 'c') || (str >= 'A' && str <= 'C'))
            return '2';
        else if((str >= 'd' && str <= 'f') || (str >= 'D' && str <= 'F'))
            return '3';
        else if((str >= 'g' && str <= 'i') || (str >= 'G' && str <= 'I'))
            return '4';
        else if((str >= 'j' && str <= 'l') || (str >= 'J' && str <= 'L'))
            return '5';
        else if((str >= 'm' && str <= 'o') || (str >= 'M' && str <= 'O'))
            return '6';
        else if((str >= 'p' && str <= 's') || (str >= 'P' && str <= 'S'))
            return '7';
        else if((str >= 't' && str <= 'v') || (str >= 'T' && str <= 'V'))
            return '8';
        else if((str >= 'w' && str <= 'z') || (str >= 'W' && str <= 'Z'))
            return '9';
        else if(str == '+')
            return '0';
    return res;
}

void readContact(char *contactP)
{
    contactP = "Test poitra";
    printf("Value at *ptr = %s \n", contactP);
}

void manageInput(int size, char seekedNumbers[])
{
    char rawInputLine[101];
    char contact[202];

    readContact(contact);
    printf("Dobre: %s\n", contact);
    
    while(fgets(rawInputLine, 101, stdin) != NULL)
    {
        //striping the line to the right lenght - \n 
        int inputSize = strlen(rawInputLine)-2;
        char inputLine[inputSize]; 
        memcpy(inputLine, rawInputLine, inputSize*sizeof(char));

        //for each letter or number check the equality (only untill inputSize - numbersSize)
        for(int inpIndex = 0; inpIndex < inputSize-size; inpIndex ++)
        {
            int isSame = 1;
            for (int i = 0; i < size; i++)
            {
                int test = inputLine[inpIndex+i];
                if(isalpha(inputLine[inpIndex+i]))
                    test = strToInt(inputLine[inpIndex+i]);
                if (test != seekedNumbers[i])
                {
                    isSame = 0;
                    continue;
                }
            }
            if(isSame == 1)
            {
                printf("%s\n", inputLine);
                continue;
            }
        }

    }
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        printf("%s\n",argv[1]);
        printf("Dlzka cisla: %d\n",(int)strlen(argv[1]));
        manageInput(strlen(argv[1]), argv[1]);
    }
    else
    {
        manageInput(0,"");
    }
}