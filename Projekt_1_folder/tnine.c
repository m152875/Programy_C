#include <stdio.h>
#include <string.h>
#define MAX_INPUT_LENGTH 101

// struct for storing contacts from the input
struct contactS{
    char name[MAX_INPUT_LENGTH];
    char number[MAX_INPUT_LENGTH];
};

char strToInt(char str)
{
    // get a letter in the arguments and return a number coresponding to that letter
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

int readLine(char *dest, int maxSize)
{
    int indx = 0;
    char c;
    //read until the end of the line or to the max input size
    while (indx < maxSize - 1 && (c = getchar()) != '\n' && c != EOF)
    {
        //skip '\r' in order to have the printf function working properly
        if(c == '\r' )
        {
            continue;
        }
        dest[indx] = c;
        indx++;
    }
    if(indx == 100)
    {
        fprintf(stderr,"Read line has exceded the lenght limit\n");
        dest[indx] = '\0'; //add ''\0' to the last character
        return 0;
    }
    return 1;
}

void initContact(struct contactS *_contact)
{
    //erase the name from the contact 
    memset(_contact->name,0, sizeof(_contact->name));

    //erase the name from the contact 
    memset(_contact->number,0,sizeof(_contact->number));
}

int readContact(struct contactS *_contact)
{
    //erase data from memory 
    initContact(_contact);

    //read name from the first line and store it in the contacts
    if(readLine(_contact->name, MAX_INPUT_LENGTH) == 0)
        return 0; // wrong input

    //read the number from the second line and store it in the contacts
    if(readLine(_contact->number, MAX_INPUT_LENGTH) == 0)
        return 0; // wrong input
        
    //if input was read correctly
    return 1;
}

int containsNumbers(char strInput[], int inpSize,char seekedNumbers [], int numSize)
{
    //if there is seeked number sequence in the string return 1 else 0 
    for(int strIndex = 0; strIndex < inpSize; strIndex++)
    {
        int isSame = 1;
        // compare the following numbers
        for (int i = 0; i < numSize; i++)
        {
            //if the values are not matching leave go to the next letter
            char inpChar = strInput[strIndex+i];
            // translate the letter to the numbers
            if((strInput[strIndex+i] >= 'a' && strInput[strIndex+i] <= 'z' ) ||
             (strInput[strIndex+i] >= 'A' && strInput[strIndex+i] <= 'Z' ) || strInput[strIndex+i] == '+' )
                {
                    inpChar = strToInt(strInput[strIndex+i]);
                }
            // if the chars are not the same continue
            if (inpChar != seekedNumbers[i]) 
            {
                isSame = 0;
                continue;
            }
            
        }
        //if the strInput contains the seekedNumbers return 1
        if(isSame == 1)
            return 1;
    }
    return 0;
}

int manageInput(char seekedNumbers[])
{
    struct contactS contact;
    int foundCount = 0;

    int numSize = strlen(seekedNumbers);

    //read the first contact from the file to get the while running
    if(readContact(&contact) == 0)
        return 1;

    while((int)strlen(contact.name) != 0)
    {
        // if there are no inputed numbers print all of the contacts in the file
        if(numSize == 0)
        {
            printf("%s, %s\n", contact.name, contact.number);
            foundCount++;// add 1 to the foundCount
        }
        // else try to find the matching numbers
        else
        {
            int found = 0;

            //for each letter in Name look for equality in the numbers[]
            if (containsNumbers(contact.name, (int)strlen(contact.name), seekedNumbers, numSize) == 1)
                found = 1;

            //for each digit in Number look for equality in the numbers[]
            if (containsNumbers(contact.number, (int)strlen(contact.number), seekedNumbers, numSize) == 1)
                found = 1;

            if (found == 1)
            {
                foundCount++;// add 1 to the foundCount
                printf("%s, %s\n", contact.name, contact.number);
            }
        }

        // erase the last contact and read a new one
        if(readContact(&contact) == 0)
            return 1;
    }
    //if the wasn`t any match in the file and if there were some arguments given print "Not found"
    if(foundCount < 1)
    {
        printf("Not found\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    //if there are any args feed them to manageInput function
    if(argc > 1)
    {
        //check if the inputed argumentnumber is a number
        for (int argIndx = 0; argIndx < (int)strlen(argv[1]); argIndx++)
        {
            if(argv[1][argIndx] < '0' || argv[1][argIndx] > '9')
            {
                fprintf(stderr,"Inputed number was in the wrong format\n");
                return 0;
            }
        }       
        return manageInput(argv[1]);
    }
    //else feed 0 and empty string
    else
    {
        return manageInput("");
    }
}