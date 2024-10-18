#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

// struct for storing contacts from the input
struct contactS{
    char name[101];
    char number[101];
};

void readLine(char *dest, int maxSize)
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
    dest[indx] = '\0';
}

void readContact(struct contactS *_contact)
{
    //read name from the first line and store it in the contacts
    readLine(_contact->name, 101);

    //read the number from the second line and store it in the contacts
    readLine(_contact->number, 101);
}

void eraseContact(struct contactS *_contact)
{
    //erase the name from the contact 
    memset(_contact->name,0, sizeof(_contact->name));

    //erase the name from the contact 
    memset(_contact->number,0,sizeof(_contact->number));
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
            // look for equality
            if (inpChar != seekedNumbers[i]) 
            {
                isSame = 0;
                continue;
            }
            
        }
        if(isSame == 1)
            return 1;
    }
    return 0;
}

void manageInput(int size, char seekedNumbers[])
{
    struct contactS contact;
    int foundCount = 0;

    //read the first contact from the file to get the while running
    readContact(&contact);

    while((int)strlen(contact.name) != 0)
    {
        // if there are no inputed numbers print all of the contacts in the file
        if(size == 0)
        {
            printf("%s, %s\n", contact.name, contact.number);
            eraseContact(&contact); // erase the values in the contact variable
            readContact(&contact); // read the next contact
        }
        // else try to find the matching numbers
        else
        {
            //for each letter in Name look for equality in the numbers[]
            if (containsNumbers(contact.name, (int)strlen(contact.name), seekedNumbers, size) == 1)
            {
                foundCount++;// add 1 to the foundCount
                printf("%s, %s\n", contact.name, contact.number);
            }

            //for each digit in Number look for equality in the numbers[]
            if (containsNumbers(contact.number, (int)strlen(contact.number), seekedNumbers, size) == 1)
            {
                foundCount++;// add 1 to the foundCount
                printf("found %s, %s\n", contact.name, contact.number);
            }
            // erase the last contact and read a new one
            eraseContact(&contact);
            readContact(&contact);
        }
    }
    //if the wasn`t any match in the file and if there were some arguments given print "Not found"
    if(foundCount == 0 && size != 0)
    {
        printf("Not found\n");
    }
}

int main(int argc, char *argv[])
{
    //if there are any args feed them to manageInput function
    if(argc > 1)
    {
        manageInput(strlen(argv[1]), argv[1]);
    }
    //else feed 0 and empty string
    else
    {
        manageInput(0,"");
    }
}