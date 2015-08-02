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
        user is prompted for new values for hh, mm, ss
*/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>

float time = 0;   //in seconds

int tc_count = 0;
int16 sec = 0, min = 0, hr = 0;
int resetFlag = 0;

CY_ISR(uartISR){
    //code to read and handle uart input
    uint32 rxData;
    rxData = UART_1_UartGetChar();
    UART_1_UartPutChar(rxData);      //echo back to terminal
    
    resetFlag = 1;
    
    /*
    if(rxData == 'c'){
        UART_1_UartPutString("\n \rTimer Interrupted. Please enter new values.\n \rhours: ");
        rxData = UART_1_UartGetChar();
        sec = rxData - '0';
        
        UART_1_UartPutString("minutes: ");
        rxData = UART_1_UartGetChar();
        min = rxData - '0';
        
        UART_1_UartPutString("seconds: ");
        rxData = UART_1_UartGetChar();
        sec = rxData - '0';
    }  */
    //clear interrupt
    UART_1_ClearRxInterruptSource(UART_1_GetRxInterruptSource());
}

CY_ISR(timerISR){
    //code to handle tc interrupt
    
    tc_count++;

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

int main(){
    uint32 rxData;
    UART_1_Start();
    
    CyGlobalIntDisable;
    
        UART_1_UartPutString("Established connection to terminal. \n \r");
        
        UART_1_UartPutString("\n\r Hours: ");
        hr = ReadInt();
        UART_1_UartPutString("\n\r Minutes: ");
        min = ReadInt();
        UART_1_UartPutString("\n\r Seconds: ");
        sec = ReadInt();
        
        Uart_Int_Start();
        Uart_Int_SetVector(uartISR);
        
        myTimer_Int_StartEx(timerISR); 
    CyGlobalIntEnable;

    TIMER_1_Start();
    
    for(;;){
        if(tc_count >= 1000)
        {
            tc_count = 0;
            sec++;
            
            if(sec > 59){
                sec = 0;
                min++;
            }
            if(min > 59){
                min = 0;
                hr++;
            }
            
            char buffer[64];
            sprintf(buffer, "%02d:%02d:%02d \n \r",hr,min,sec);
            UART_1_UartPutString(buffer);
        }
        if(resetFlag == 1){
            TIMER_1_Stop();
            UART_1_UartPutString("\n\r Hours: ");
            hr = ReadInt();
            UART_1_UartPutString("\n\r Minutes: ");
            min = ReadInt();
            UART_1_UartPutString("\n\r Seconds: ");
            sec = ReadInt();
        }
            TIMER_1_Start();  
    }
}

