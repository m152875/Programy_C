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

struct contactS{
    char name[101];
    char number[101];
};
struct contactS contact; 

void readLine(char *dest, int maxSize)
{
    int indx = 0;
    char c;
    while ((c = getchar()) != '\n' && indx < maxSize)
    {
        dest[indx] = c;
        indx++;
    }
}

void readContact()
{
    //get Name
    readLine(contact.name, 101);

    //get Number
    readLine(contact.number, 101);
}
void eraseContact()
{
    //erase Contact
    memset(contact.name,0,strlen(contact.name));

    //erase Number
    memset(contact.number,0,strlen(contact.number));
}

void manageInput(int size, char seekedNumbers[])
{
    readContact();

    printf("Meno: %s\n", contact.name);
    printf("Cislo: %s\n", contact.number);

    while((int)strlen(contact.name) == 0)
    {
        //for each letter in Name
        for(int nameIndex = 0; nameIndex < (int)strlen(contact.name)-size; nameIndex++)
        {
            int isSame = 1;
            for (int i = 0; i < size; i++)
            {
                if (strToInt(contact.name[nameIndex+i]) != seekedNumbers[i])
                {
                    isSame = 0;
                    continue;
                }
            }
            if(isSame == 1)
            {
                printf("%s, %s\n", contact.name, contact.number);
                continue;
            }
        }

        //for each digit in Number
        for(int numberIndex = 0; numberIndex < (int)strlen(contact.number)-size; numberIndex++)
        {
            int isSame = 1;
            for (int i = 0; i < size; i++)
            {
                if (contact.number[numberIndex+i] != seekedNumbers[i])
                {
                    isSame = 0;
                    continue;
                }
            }
            if(isSame == 1)
            {
                printf("%s, %s\n", contact.name, contact.number);
                continue;
            }
        }
        
        eraseContact();
        readContact();
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