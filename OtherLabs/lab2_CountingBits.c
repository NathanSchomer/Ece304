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

void WriteInt(int num)
{
    char buffer[64];
    sprintf(buffer, "%d \n \r", num);
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

int isPowerOfTwo(uint v)
{
    return countBits(v) == 1;
}

uint reverseBits(uint v)
{
    uint r = v;
    int s = sizeof(v) * CHAR_BIT - 1; //used to shift in any extra 0's necessary
    for(v >>= 1; v != 0; v >>= 1)
    {
        r = r << 1; //shift 1 into r
        r = r | (v&1);
        s--;
    }
    r = r << s;
    return r;
}

uint8 rearrangeBits(uint8 v)
{
    return (~0) << countBits(v);
}

uint32 interleaveBits(uint16 x, uint16 y)
{
    uint32 z;
    int i; 
    //z = x;
    //z <<= y;
    for (i = 0; i < sizeof(x) * CHAR_BIT; i++)
    {
        //create a mask for x&y with a 1 shifted i number of times left
        int x_mask = (x & (1 << i));
        int y_mask = (y & (1 << i));
        
        //"or" the mask with z and shift i number of times left
        z = z | x_mask << i;
        z = z | y_mask << (i+1);
    }
    return z;
}

int main()
{
    CyGlobalIntEnable;
    
    UART_1_Start();
    UART_1_UartPutString("\n \rEstablished Connection \n \r");
    
    uint16 rxData;
    rxData = UART_1_UartGetChar();
    UART_1_UartPutChar(rxData);
    
    /*uint16 x = 0x55, y = 0xaa;
    WriteHex(interleaveBits(x, y));*/
    for(;;){}
}

/*
//2e
int main()
{
    CyGlobalIntEnable;
    UART_1_Start();
    UART_1_UartPutString("\n \rEstablished Communication. \n \r");
    
    uint v4 = 0xd1;
    WriteHex(reverseBits(v4));
    
    for(;;){}
}
*/
/*
//Part 3
int main()
{
    CyGlobalIntEnable;
    UART_1_Start();
    UART_1_UartPutString("\n \rEstablished Communication. \n \r");
    
    uint8 v2 = 0x56;
    WriteHex(rearrangeBits(v2));
    
    
    for(;;){}
}
*/
/*
//Second Part
int main()
{
    char buffer[64];
    CyGlobalIntEnable;
    UART_1_Start();
    UART_1_UartPutString("\n \rEstablished Communication. \n \r");
    
    uint v3 = 0x1000;
    int flag = isPowerOfTwo(v3);
    
    sprintf(buffer, "Is Power of 2: %d \n \r", flag);
    UART_1_UartPutString(buffer);
    
    for(;;){}
}
*/
/*
//Part1
int main()
{    
    char buffer[64];
    CyGlobalIntEnable;      // Enable global interrupts 
    
    UART_1_Start();
    UART_1_UartPutString("\n \rEstablished Communication. \n \r");
    
    int v = 0x5; //hex representation of integer
    int count = countBits(v);
    
    sprintf(buffer, "Number of 1's: %d \n \r", count);
    UART_1_UartPutString(buffer);
    
    for(;;){}
}*/