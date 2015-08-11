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

int delay = 1;  //delay in seconds

CY_ISR(switchISR)
{
    //debouncing to eliminate any switch glitches.
    CyDelay(50);
    
    while(!SW_Read()); //wait for switch to be released
    CyDelay(50);
    
    Pin_RED_Write(!(Pin_RED_Read()));         //toggle the LED
    ++delay;
    delay %= 6;                       //limit slowest blink rate to 0.2 Hz
    SW_ClearInterrupt();              //clear the interrupt
}

int main()
{
    CyGlobalIntDisable;
    SW_Int_Start();
    SW_Int_SetVector(switchISR);
    
    CyGlobalIntEnable;
    
    for(;;){
        Pin_RED_Write(~Pin_RED_Read());
        CyDelay(delay*1000);
    }
}