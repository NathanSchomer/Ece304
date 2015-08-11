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

int rateBuff = 1;
int rate = 1;

CY_ISR(uartISR)
{
    uint32 rxData;
    rxData = UART_1_UartGetChar();   //get char from buffer
    UART_1_UartPutChar(rxData);      //echo back to terminal
    
    if(rxData - '0' > 0 && rxData - '0' <= 5)
    {
        rateBuff = rxData - '0';
    }
    
    //clear interrupt
    UART_1_ClearRxInterruptSource(UART_1_GetRxInterruptSource());
}

CY_ISR(switchISR)
{
    CyDelay(50);
    while(!SW_Read());
    CyDelay(50);
    
    rate = rateBuff;
    
    SW_ClearInterrupt(); //clear interrupt
}

int main()
{
    CyGlobalIntDisable;             //disable global interrupts
    Uart_Int_Start();               //start the ISR associate with uart
    Uart_Int_SetVector(uartISR);    //change vector ISR address
    
    SW_Int_Start();                 //start ISR
    SW_Int_SetVector(switchISR);    //change ISR address in vector table
    CyGlobalIntEnable;                 //enable global interrupts
    
    UART_1_Start();                 //start UART
    
    for(;;){
        Pin_RED_Write(~Pin_RED_Read());
        CyDelay(rate*1000);
    }
}
    

