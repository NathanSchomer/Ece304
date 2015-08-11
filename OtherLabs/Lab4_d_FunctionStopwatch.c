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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MATRIX_SIZE 15
float tc;
float elapsedTime;
float begin, end;

void WriteInt(int num)
{
    char buffer[64];
    sprintf(buffer, "\n \r %d ms\n \r", num);
    UART_1_UartPutString(buffer);
}

CY_ISR(uartISR){
    //your ISR code
    
    //clear interrupt
    UART_1_ClearRxInterruptSource(UART_1_GetRxInterruptSource());
}

CY_ISR(myTimerISR){
    tc = tc + 1;
    //your ISR code
    
    //reset interrupt
    TIMER_1_ReadStatusRegister();
}

void tic(void){
    begin = tc;
}

void toc(void){
    elapsedTime = (float)(tc - begin) / 10.0;
}

void populateMatrix(float M[MATRIX_SIZE][MATRIX_SIZE]){
    int8 i, j;
    for(i = 0; i < MATRIX_SIZE; i ++){
        for(j = 0; j < MATRIX_SIZE; j++){
            M[i][j] = (float)rand()/(float)RAND_MAX;
        }
    }
}

void matrixMult(float A[MATRIX_SIZE][MATRIX_SIZE], \
                float B[MATRIX_SIZE][MATRIX_SIZE], \
                float C[MATRIX_SIZE][MATRIX_SIZE])
{
    int8 i, j, k;
    float temp;
    for(i = 0; i < MATRIX_SIZE; i++){
        for(j = 0; j < MATRIX_SIZE; j++){
            temp = 0.0;
            for(k = 0; k < MATRIX_SIZE; k++){
                temp += A[i][k] * B[k][j];
            }
            C[i][j] = temp;
        }
    }
}

int main()
{
    CyGlobalIntDisable;
    Uart_Int_Start();
    Uart_Int_SetVector(uartISR);
    
    myTimer_Int_Start();
    myTimer_Int_SetVector(myTimerISR);

    CyGlobalIntEnable;      /* Enable global interrupts */

    UART_1_Start();
    TIMER_1_Start();
    
    srand(rand());  //initialize random number generator
    
    float A[MATRIX_SIZE][MATRIX_SIZE];
    float B[MATRIX_SIZE][MATRIX_SIZE];
    float C[MATRIX_SIZE][MATRIX_SIZE];
    for(;;)
    {
        populateMatrix(A);
        populateMatrix(B);
        
        tic();
        matrixMult(A,B,C);
        toc();
        
        WriteInt(elapsedTime);
        
        //Display elapesed time in terminal
        
        CyDelay(1000);
        
        // Place your application code here
    }
}

