#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int compareStrings(char *str1, char *str2)
{
	while(*str1 != 10)
	{
		if(*str1 != *str2)
			return *str1 - *str2;
		str1++; str2++;
	}
	return 0;
}

int searchForChar(char *str, char ch)
{
	int count = 0;
	while(*str != 10)
	{
		if(*str == ch)
			return count;	
		count++; str++;
	}
	return -1;
}

int size(char *str1)
{
	int count = 0;
	while(*str1 != 10)
	{
		str1++;
		count++;
	}
	return count;
}

int searchSubString(char *str1, char *subStr)
{
	char *str1_backup = str1;
	int subsize = size(subStr);
	int i;

	//loop through all of str1
	while(*str1 != 10){
		if(*str1 == *subStr){	//check for instance of *subStr[0]
			for(i = 1; i < subsize; i++){		//if found, compare all chars in subStr
				if(*(str1 + i) != *(subStr + i))
					break;
			}
			return str1 - str1_backup; 
		}
		str1++;
	}
	return -1;
}

int readString(char *buffer)
{
	char rxData;
	char *ptr = buffer;

	for(;;)
	{
		rxData = getchar(); //grab char
		if(rxData == '\n')
		{
			*ptr = '\n';
			break;
		}
		*ptr = rxData;
		ptr++;

	}
}

main()
{
	char str1[64], str2[64];
	char ch;

	//string comparison
       	printf("\nString Compare:\nstr1: "); readString(str1);
	printf("str2: "); readString(str2);
	printf("Are equal? %d\n\n", compareStrings(str1, str2));
	
	//Substring Check
	printf("Substring Check:\nstr1: "); readString(str1);
	printf("str2: "); readString(str2);
	printf("Is Substring? %d\n\n", searchSubString(str1, str2));
	
	//Check for char
	printf("Search for char:\nstr1: "); readString(str1);
	printf("char: "); ch = getchar();
	printf("Found char? %d", searchForChar(str1, ch)); 

	return 0;
}
