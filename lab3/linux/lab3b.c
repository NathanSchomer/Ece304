#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
	char str3[64], str4[64], str5[64];

	//Count words
	printf("\n\nCount Words: "); readString(str3);
	printf("Word Count: %d", countWords(str3));

	//Find longest word
	printf("\n\nFind longest word: "); readString(str4);
	printf("Longest Word: %d", longestWord(str4));

	//Most number of vowels
	printf("\n\nFind most vowels: "); readString(str5);
	printf("Most vowels: %d", mostVowels(str5));
	return 0;
}
