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

//char* comb = "24135"; //default password
int comb = 24135;
int tc_count = 0;
//int currentState = -1;

int attempts = 0;
int permLock = 0;
int unlocked = 1;
int blinkLed = 0;
int ledOn = 0;

CY_ISR(timerISR){
    //code to handle tc interrupt
    
    tc_count++;
    
    if(tc_count >= 250 && blinkLed == 1)
    {
        Pin_RED_Write(!(Pin_RED_Read()));
        tc_count = 0;
    }
    else
    {
        Pin_RED_Write(unlocked);
    }

    TIMER_1_ReadStatusRegister();
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

int ResetPasscode()
{
    //make LED blink
    if(permLock == 0){
        int tmp1, tmp2;
        UART_1_UartPutString("Enter New Key: ");
        tmp1 = ReadInt();
        UART_1_UartPutString("\n\rEnter Key Again to Confirm: ");
        tmp2 = ReadInt();
        if(tmp1 == tmp2){
            comb = tmp1;
            return 1;
        }
    }
    return 0;
}

CY_ISR(uartISR){
    uint32 rxData;
    rxData = UART_1_UartGetChar();
    //UART_1_UartPutChar(rxData);      //echo back to terminal
    
    if(rxData == '1' && unlocked == 1){
        unlocked = 0;
        UART_1_UartPutString("\n\rSYSTEM LOCEKD\n\r\n\r");
    }
    if(rxData == '2' && unlocked == 1)
    {
        blinkLed = 1;
        if(ResetPasscode()){
            unlocked = 0;
            UART_1_UartPutString("\n\rSYSTEM LOCEKD\n\r\n\r");
        }
        else
        {
            UART_1_UartPutString("\n\rKey Mismatch!\n\r");
        }
        blinkLed = 0;
    }
    
    //clear interrupt
    UART_1_ClearRxInterruptSource(UART_1_GetRxInterruptSource());
}

uint32 GetChar()
{
    uint32 rxData;
    for(;;)
    {
        rxData = UART_1_UartGetChar();
        if(rxData){
            if(rxData == '\r') break;
            UART_1_UartPutChar(rxData);
            return rxData;
        }
    }
    
    return rxData;
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
    //UART_1_UartPutString("getCombination");
	//char key1 = comb[0];
	int currentState = -1;
	int input;
	for(;;){
		//get button pushed by user via UART
		input = GetChar();
		while(input == '\r')
			input = UART_1_UartGetChar();
			
		if(input == (comb/10000)%10+'0'){
			if(currentState == -1)
				currentState = 1;
			else
				return 0;
		}
		else if(input == (comb/1000)%10+'0'){
			if(currentState == 1)
				currentState = 2;
			else
				return 0;
		}
		else if(input == (comb/100)%10+'0'){
			if(currentState == 2)
				currentState = 3;
			else
				return 0;
		}
		else if(input == (comb/10)%10+'0'){
			if(currentState == 3)
				currentState = 4;
			else
				return 0;
		}
		else if(input == (comb)%10+'0'){
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
    UART_1_UartPutString("Combination Lock \n\r\n\r");
	Uart_Int_Start();
    Uart_Int_SetVector(uartISR);
    
    myTimer_Int_StartEx(timerISR);
    CyGlobalIntEnable;
    
    TIMER_1_Start();
    
    //char key1 = comb[0];
    char input;
        
    for(;;){
        
		if(permLock == 1 || unlocked == 1){}
		else if(permLock == 0 && attempts >= 3){
			permLock = 1;
			UART_1_UartPutString("\n\rToo Many Attempts!\n\rPerm Lock Set \n \r");
		}
		else if(attempts < 3 && getCombination()){
			unlocked = 1;
            attempts = 0;
            
			UART_1_UartPutString("\n\rACCESS GRANTED\n\r\n\r");
		}
		else if(attempts < 3){
			attempts++;
			UART_1_UartPutString("\n\rACCESS DENIED\n\r\n\r");
		}
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
