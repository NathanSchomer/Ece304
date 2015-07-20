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

int countWords(char *str)
{
	int count = 0;
	int wordFound = 0;
	while(*str != 10)
	{
		//find first char
		if(*str != ' ' && wordFound == 0)
		{
			wordFound = 1;
			count = 1;
		}
		else if(*str == ' ')
			count++;
		str++;
	}
	return count;
}

int longestWord(char *str)
{
	int max = 0, maxWordBegin, maxWordEnd;
	int currWordBegin = 0;
	int i;

	for(i = 0;; i++)
	{
		if(*(str+i) == ' ' || *(str+i) == 10){
			if((i - currWordBegin) > max ){
				max = i - currWordBegin;
				maxWordBegin = currWordBegin;
				maxWordEnd = i - 1;
				if(*(str+i) == 10)
					break;
			}
			currWordBegin = i + 1;
		}
	}
	return max;
}

int mostVowels(char *str)
{
	int max = 0, curr = 0;
	
	for(;;)
	{
		if(*str == ' ' || *str == 10){
			if(curr > max)
				max = curr;
			if(*str == 10)
				return max;
			curr = 0;	
		}	
		else
		{
			if(*str == 'a' || *str == 'A' ||
			   *str == 'e' || *str == 'E' ||
			   *str == 'i' || *str == 'I' ||
			   *str == 'o' || *str == 'O' ||
			   *str == 'u' || *str == 'U' )
			{
				curr++;
			}
		}
		str++;
	}
	return max;
}

main()
{
	char str1[64], str2[64], str3[64], str4[64], str5[64], str6[64];
	char ch;

	/*
	 *string comparison
         *printf("\nString Compare:\nstr1: "); readString(str1);
	 *printf("str2: "); readString(str2);
	 *printf("Are equal? %d\n\n", compareStrings(str1, str2));
	 *
	 *[>//substring check<]
	 *printf("Substring Check:\nstr1: "); readString(str1);
	 *printf("str2: "); readString(str2);
	 *printf("Is Substring? %d\n\n", searchSubString(str1, str2));
	 *
	 *printf("Search for char:\nstr1: "); readString(str1);
	 *printf("char: "); ch = getchar();
	 *printf("Found char? %d", searchForChar(str1, ch)); 
	 */

	//bottom section needs to be run in a seprate excecutable!!	
	printf("\n\ncount words: "); readstring(str3);
	printf("word count: %d", countwords(str3));

	printf("\n\nfind longest word: "); readstring(str4);
	printf("longest word: %d", longestword(str4));

	printf("\n\nfind most vowels: "); readstring(str5);
	printf("most vowels: %d", mostvowels(str5));
	return 0;
}
