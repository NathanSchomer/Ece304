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

/* 1. must allow user to set initial hh, mm, ss
 2. allow user to reset various fields during operation...
        when c is pressed on keyboard, clock is stopped and
        user is prompte:qd for new values for hh, mm, ss
*/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>

char* comb = "24135"; //default password
//int currentState = -1;

int attempts = 0;
int permLock = 0;
int unlocked = 0;

void ResetPasscode()
{
    UART_1_UartPutString("Please reset passcod \n \r");
}

CY_ISR(uartISR){
    //code to read and handle uart input
    uint32 rxData;
    rxData = UART_1_UartGetChar();
    //UART_1_UartPutChar(rxData);      //echo back to terminal
    
    if(rxData == '1'){
        unlocked = 0;
        UART_1_UartPutString("SYSTEM LOCEKD \n \r");
    }
    else if(rxData == 'c')
    {
        ResetPasscode();
    }
    
    //clear interrupt
    UART_1_ClearRxInterruptSource(UART_1_GetRxInterruptSource());
}

int ReadInt()
{
    uint32 rxData;
    int result = 0;
    
    int digitCount;
    
    for(;;)
    {
        rxData = UART_1_UartGetChar();
        if(rxData){
            if(rxData == '\r') break;
            UART_1_UartPutChar(rxData);
            result *= 10;
            result += rxData - '0';
        }
    }
    
    return result;
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
                *ptr = '\0';
                break;
            }
            /* Store the character into the current buffer location and incriment the pointer to the next location */
            *ptr = rxData;
            ptr++;
        }
    }
}

int getCombination()
{
	char key1 = comb[0];
	int currentState = -1;
	char input;
	while(1){
		//get button pushed by user via UART
		input = UART_1_UartGetChar();
		while(input == '\n')
			input = getchar();
			
		if(input == comb[0]){
			if(currentState == -1)
				currentState = 1;
			else
				return 0;
		}
		else if(input == comb[1]){
			if(currentState == 1)
				currentState = 2;
			else
				return 0;
		}
		else if(input == comb[2]){
			if(currentState == 2)
				currentState = 3;
			else
				return 0;
		}
		else if(input == comb[3]){
			if(currentState == 3)
				currentState = 4;
			else
				return 0;
		}
		else if(input == comb[4]){
			if(currentState == 4)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	
}

int main(){
    //uint32 rxData;
    UART_1_Start();
    
    CyGlobalIntDisable;
    UART_1_UartPutString("Established connection to terminal. \n \r"); 
    Uart_Int_Start();
    Uart_Int_SetVector(uartISR);
    CyGlobalIntEnable;
    
    char key1 = comb[0];
    char input;
        
    for(;;){
        
    }
}

/*


//Combination Lock
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//add 5 second timeout

char* comb = "24135";

int getCombination()
{
	char key1 = comb[0];
	int currentState = -1;
	char input;
	while(1){
		//get button pushed by user via UART
		input = getchar();
		while(input == '\n')
			input = getchar();
			
		if(input == comb[0]){
			if(currentState == -1)
				currentState = 1;
			else
				return 0;
		}
		else if(input == comb[1]){
			if(currentState == 1)
				currentState = 2;
			else
				return 0;
		}
		else if(input == comb[2]){
			if(currentState == 2)
				currentState = 3;
			else
				return 0;
		}
		else if(input == comb[3]){
			if(currentState == 3)
				currentState = 4;
			else
				return 0;
		}
		else if(input == comb[4]){
			if(currentState == 4)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	
}

main()
{
	printf("Combination Lock\n");
	int attempts = 0;
	int permLock = 0;
	int unlocked = 0;
	for(;;){	//user gets 3 attempts to try
		if(unlocked == 1)
		{
			if(getchar() == 'c'){
				printf("New 5-digit combination: ");
				comb = "12345"; //modify value of comb here
				//getchar();
				//getline(comb, 6, stdin);
				printf("SYSTEM LOCKED!\n");
				unlocked = 0;
			}
		}
		else if(attempts < 3 && getCombination()){
			attempts = 0;
			unlocked = 1;
			printf("ACCESS GRANTED\n");
		}
		else if(attempts < 3){
			attempts++;
			printf("ACCESS DENIED\n");
		}
		else if(permLock == 0){
			permLock = 1;
			printf("\nTOO MANY ATTEMPTS.\n");
			printf("SYSTEM IS PERMANATELY LOCKED.\n");
		}
		else{
		}
	}
	return 0;
}
*/