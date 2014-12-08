#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "sysinit.h"
#include "ke02_config.h"
void delay(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
	for(i=0;i<100000;i++)
		j=0;
}
//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
	RED_Init();
	GREEN_Init();
	BLUE_Init();
    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {
		RED_Set();GREEN_Set();BLUE_Set();
		delay(100);
		BLUE_Clear();GREEN_Clear();RED_Clear();
		delay(100);
    }
}
