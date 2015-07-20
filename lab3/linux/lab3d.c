#include <stdio.h>
#include <stdlib.h>

int readString(char *buffer)
{
	char rxData;
	char *ptr = buffer;

	for(;;)
	{
		rxData = getchar();
		if(rxData == '\n')
		{
			*ptr = '\n';
			break;
		}
		*ptr = rxData;
		ptr++;
	}
}

int size(char *str)
{
	int count = 0;
	while(*str != 10)
	{
		str++;
		count++;
	}
	return count;
}

int checkPalindrome(char *word)
{
	int length = size(word);
	int i;
	for(i = 0; i < (size(word)/2); i++){
		if(*(word+i) != *(word+length-i-1))
			return 0;
	}
	return 1; //not palindrome
}

main()
{
	char word[64];
	printf("Enter word: "); readString(word);
	printf("Is palindrome? %d", checkPalindrome(word));
	return 0;
}
