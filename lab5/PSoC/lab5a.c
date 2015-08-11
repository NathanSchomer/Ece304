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
int normComb = 24135;
int superComb = 12345;
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
    
    if(tc_count%250 == 0 && blinkLed == 1)
    {
        Pin_RED_Write(!(Pin_RED_Read()));
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
            normComb = tmp1;
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

int getCombination(int comb)
{
	int currentState = -1;
	int input;
    int begin = 0;
	for(;;){
		//get button pushed by user via UART
		input = GetChar();
		while(input == '\r'){
			input = UART_1_UartGetChar();
        }
			
        if(currentState >= 1 && (tc_count-begin)/500 >= 5)
        {
            UART_1_UartPutString("\n\rPassword timeout.\n\r");
            tc_count = 0;
            return 0;
        }
		else if(input == (comb/10000)%10+'0'){
			if(currentState == -1){
                begin = tc_count;
				currentState = 1;
            }
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
    
    char input;
        
    for(;;){
        if(permLock == 1)
        {
            if(getCombination(superComb) == 1)
            {
                UART_1_UartPutString("\n\r\n\rSUPERVISOR MODE.\n\rSystem is unlocked.\n\r\n\r");
                attempts = 0;
                unlocked = 1;
                permLock = 0;
            }
        }
        else if(unlocked == 1){}
		else if(attempts >= 3){
			permLock = 1;
			UART_1_UartPutString("\n\rToo Many Attempts!\n\rPerm Lock Set \n \r");
		}
		else if(getCombination(normComb)){
			unlocked = 1;
            attempts = 0;
            
			UART_1_UartPutString("\n\rACCESS GRANTED\n\r\n\r");
		}
		else{
			attempts++;
			UART_1_UartPutString("\n\rACCESS DENIED\n\r\n\r");
		}
    }
}