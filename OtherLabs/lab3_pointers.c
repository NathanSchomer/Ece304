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

int compareStrings(char *str1, char *str2)
{
    while(*str1 != '\0' && *str2 != '\0')
    {
        if(*str1 != *str2)
            return 0;
        str1++; str2++;
    }
    return 1;
}

int searchForChar(char *str1, uint32 ch)
{
    while(*str1 != '\0')
    {
        if(*str1 == ch)
            return 1;
        str1++;
    }
    return 0;
}

int searchSubString(char *str1, char *subStr)
{
    char *original = subStr;
    
    while(*str1 != '\0')
    {
        while(*str1 == *subStr && *str1 != '\0'){ //If 
             str1++; subStr++;
             if(*subStr == '\0')
                return 1;
        }
        subStr = original;  //reset subString pointer back to base address
        str1++;
    }
    return 0;
}

int main()
{
    ///// PART A
    CyGlobalIntEnable;      /* Enable global interrupts */
    UART_1_Start();
    
    int choice;
    int8 status;
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
                case 1:
                    //read in the strings and store them in str1 and str2
                    UART_1_UartPutString("\n \r Str1: ");
                    ReadString(str1);
                    UART_1_UartPutString("\n \r Str2: ");
                    ReadString(str2);
                    status = compareStrings(str1, str2);
                    WriteInt(status);
                break;
                
                case 2:
                    //read in the string ans store in str1. Read in char and store in ch
                    UART_1_UartPutString("\n \r Str1: ");
                    ReadString(str1);
                    UART_1_UartPutString("\n \r Char: ");
                    
                    ch = UART_1_UartGetChar();
                    do{
                        ch = UART_1_UartGetChar();
                    }while(!ch);
                    
                    status = searchForChar(str1, ch);
                    WriteInt(status);
                break;
                
                case 3:
                    //Read in the string, store in str1. Read in substring and store in str2
                    UART_1_UartPutString("\n \r Str1: ");
                    ReadString(str1);
                    UART_1_UartPutString("\n \r SubStr: ");
                    ReadString(str2);
                    //str1 = "Test";
                    //str2 = "Te";
                    status = searchSubString(str1, str2);
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

