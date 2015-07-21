/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_BUFFER_SIZE 64

int size(char *str)
{
	int count = 0;
	while(*str != '\0')
	{
		count++;
        str++;
	}
	return -1;
}

void ReadString(char *buffer)
{
    uint32 rxData;
    char *ptr = buffer;
    
    for(;;)
    {
        /*Check if a character is aviailabe in the UART buffer. The function returns
        a char where 1 to 255 are valid ASCII characters and 0 indicates an error
        or no data present.*/
        rxData = UART_1_UartGetChar();
        if(rxData){
            UART_1_UartPutChar(rxData);//echo char back to terminal
            if(rxData == '\r')
            {
                *ptr = '\r';
                break;
            }
            /* Store the character into the current buffer location and incriment the pointer to the next location */
            *ptr = rxData;
            ptr++;
        }
    }
}

void WriteInt(int num)
{
    char buffer[64];
    sprintf(buffer, "\n \r %d \n \r", num);
    UART_1_UartPutString(buffer);
}

void WriteHex(int hex)
{
    char buffer[64];
    sprintf(buffer, "0x%x", hex);
    UART_1_UartPutString(buffer);
}

int countBits(int v)
{
    //insert code here
    //count the number of 1s
    int count;
    
    for(count = 0; v != 0; count++)
    {
        v &= v-1;
    }
    
    return count;
}

//good
int compareStrings(char *str1, char *str2)
{
    while(*str1 != '\0')
    {
        if(*str1 != *str2)
            return *str1 - *str2;
        str1++; str2++;
    }
    return 0;
}


int searchForChar(char *str1, uint32 ch)
{
    int count = 0;
    while(*str1 != '\0')
    {
        if(*str1 == ch)
            return count;
        str1++; count++;
    }
    return -1;
}

int searchSubString(char *str1, char *subStr)
{

	char *str1_backup = str1;
	int subsize = size(subStr);
	int i;

	//loop through all of str1
	while(*str1 != '\0'){
		if(*str1 == *subStr){
			for(i = 1; i < subsize; i++){
				if(*(str1 + i) != *(subStr + i))
						break;
			}
			return str1 - str1_backup;
		}
		str1++;
	}
	return -1;
}

int countWords(char *str)
{
	int count = 0;
	int wordFound = 0;
	while(*str != '\0'){
		//find first char
		if(*str != ' ' && wordFound == 0){
			wordFound = 1;
			count = 1;
		}
		else if(*str == ' ')
			count++;
		str++;
	}
	return count;
}

int longestWord(char *str){
	int max = 0, maxWordBegin, maxWordEnd;
	int currWordBegin = 0;
	int i;

	for(i = 0; 1 == 1; i++)
	{
		if(*(str+i) == ' ' || *(str+i) == '\0'){
			if((i - currWordBegin) > max ){
				max = i - currWordBegin;
				maxWordBegin = currWordBegin;
				maxWordEnd = i - 1;				//make sure this behaves as expected
				if(*(str+i) == '\0')
					break;
			}
			currWordBegin = i + 1;
		}
	}
    return max;
}

int mostVowels(char *str){
	int max = 0, curr = 0;

	for(;;){
		if(*str == ' ' || *str == '\0'){
			if(curr > max)
				max = curr;
			if(*str == '\0')
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

int exponent(int base, int exp){
	int i, result = base;
	for(i = 0; i < exp - 1; i++)
		result *= base;
	return result;
}

int validateLuhn(int num)
{
	int sum = 0, i;
	for(i = 1; num > 0; i += 2){
		sum += num%exponent(10,i);
		sum += 2*(num%exponent(10,i+1))%10 + 2*(num%exponent(10, i+1))/10;
		if(num < 100) break;
		else num /= 100;
	}
	if(sum % 10 == 0) return 1; //VALID
	else return 0;				//INVALID
}

int checkPalindrome(char *word){
	int length = size(word);
	int i;

	for(i = 0; i < (size(word)/2); i++){
		if(*(word+i) != *(word+length-i-1))
			return 0;	//palindrome
	}
	return 1; //not palindrome
}

int main(){
    ///// PART A
    CyGlobalIntEnable;      /* Enable global interrupts */
    UART_1_Start();
    
    int choice, input_num;
    int8 status, status2, status3;
    uint32 ch, tmpChar;
    int count = 0;
    char str1[MAX_BUFFER_SIZE], str2[MAX_BUFFER_SIZE];
    
    while(1){
        //UART_1_UartPutString("\n \r Function to excecute (1 for string) \ compare, 2 for character serach, 3 for substring \ search, or -1 to exit): ");
        tmpChar = UART_1_UartGetChar(); //&choice??
        //choice = tmpChar - '0';
        
        if(choice == -1) break;
        
        if(tmpChar)
        {
            choice = tmpChar - '0';
            switch(choice){
                case 1: //compare strings
					UART_1_UartPutString("\n \r Str1: "); 
					ReadString(str1);
                    
					UART_1_UartPutString("\n \r Str2: ");
                    ReadString(str2);
					
					status = compareStrings(str1, str2);
					WriteInt(status);
                break;
                
                case 2: //check for char
                    UART_1_UartPutString("\n \r Check for Char \n \r Str1: ");
                    ReadString(str1);

                    UART_1_UartPutString("\n \r Char: ");
                    ch = UART_1_UartGetChar();
                    
					do{
                        ch = UART_1_UartGetChar();
                    }while(!ch);
                    
					//print the accepted character back to the terminal
					UART_1_UartPutChar(ch);
                    
					status = searchForChar(str1, ch);
					UART_1_UartPutString("\n \r Location of char: ");
                    WriteInt(status);
                break;
                
		case 3: //substring check
					UART_1_UartPutString("\n\rCheck for Substring\n\rStr1: ");
                    ReadString(str1);
                    UART_1_UartPutString("\n \r SubStr: ");
                    ReadString(str2);
                    //str1 = "Test";
                    //str2 = "Te";

					status = searchSubString(str1, str2);
					UART_1_UartPutString("\n\rFound at: ");
                    WriteInt(status);
                break;
		
		case 4: //count, longest word, most vowels
			UART_1_UartPutString("\n\rCount, longest, most vowels:\n\rStr1:"); 
            ReadString(str1);

			//valid for the same string???
			//add string output infront of return values
			status = countWords(str1);
			UART_1_UartPutString("\n\rWord Count: ");
			WriteInt(status);
			
            status2 = longestWord(str1);
            UART_1_UartPutString("\n\rLongest Word: ");
			WriteInt(status2);
			
            status3 = mostVowels(str1);
            UART_1_UartPutString("\n\rMost vowels: ");
			WriteInt(status3);
		break;

		case 7: //luhn checksum validation
			
		//READ INT FROM TERMINAL
		
			status = validateLuhn(input_num);
			WriteInt(status);
		break;

		case 8: //palindrome
			UART_1_UartPutString("\n \r Str1: ");
			ReadString(str1);

			status = checkPalindrome(str1);			
			WriteInt(status);
		break;

                default:
                UART_1_UartPutString("\n \r Erroneous entry. Try again. \n \r");
            };
        }
    }
    
    UART_1_UartPutString("\n \r All done. Idling the CPU.");
    
    for(;;)
    {
    }
}
