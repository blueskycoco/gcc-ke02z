//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include "sysinit.h"
//#include "inc/hw_types.h"

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// External declaration for the interrupt handler used by the application.
//
//*****************************************************************************
extern void USART1_IRQHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static uint32_t pui32Stack[64];
#define BootRAM ((void *)(0xF108F85F))
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))((uint32_t)pui32Stack + sizeof(pui32Stack)),
	// The initial stack pointer
	ResetISR, //Reset Handler
	NmiSR, //NMI Handler
	FaultISR, //Reserved
	0, //Reserved
	0, //Reserved
	0, //Reserved
	0, //Reserved
	0, //Reserved
	0, //Reserved
	0, //Reserved
	IntDefaultHandler,//SVC_Handler, //SVCall Handler
	0, //Reserved
	0, //Reserved
	IntDefaultHandler,//PendSV_Handler, //PendSV Handler
	IntDefaultHandler,//SysTick_Handler, //SysTick Handler

	//; External Interrupts
	IntDefaultHandler,//Reserved16_IRQHandler, //Reserved interrupt 16
	IntDefaultHandler,//Reserved17_IRQHandler, //Reserved interrupt 17
	IntDefaultHandler,//Reserved18_IRQHandler, //Reserved interrupt 18
	IntDefaultHandler,//Reserved19_IRQHandler, //Reserved interrupt 19
	IntDefaultHandler,//Reserved20_IRQHandler, //Reserved interrupt 20
	IntDefaultHandler,//FTMRH_IRQHandler, //FTMRH command complete/read collision interrupt
	IntDefaultHandler,//LVD_LVW_IRQHandler, //Low Voltage Detect, Low Voltage Warning
	IntDefaultHandler,//IRQ_IRQHandler, //External interrupt
	IntDefaultHandler,//I2C0_IRQHandler, //I2C0 interrupt
	IntDefaultHandler,//Reserved25_IRQHandler, //Reserved interrupt 25
	IntDefaultHandler,//SPI0_IRQHandler, //SPI0 interrupt
	IntDefaultHandler,//SPI1_IRQHandler, //SPI1 interrupt
	IntDefaultHandler,//UART0_IRQHandler, //UART0 status/error interrupt
	IntDefaultHandler,//UART1_IRQHandler, //UART1 status/error interrupt
	IntDefaultHandler,//UART2_IRQHandler, //UART2 status/error interrupt
	IntDefaultHandler,//ADC0_IRQHandler, //ADC0 interrupt
	IntDefaultHandler,//ACMP0_IRQHandler, //ACMP0 interrupt
	IntDefaultHandler,//FTM0_IRQHandler, //FTM0 Single interrupt vector for all sources
	IntDefaultHandler,//FTM1_IRQHandler, //FTM1 Single interrupt vector for all sources
	IntDefaultHandler,//FTM2_IRQHandler, //FTM2 Single interrupt vector for all sources
	IntDefaultHandler,//RTC_IRQHandler, //RTC overflow
	IntDefaultHandler,//ACMP1_IRQHandler, //ACMP1 interrupt
	IntDefaultHandler,//PIT_CH0_IRQHandler, //PIT CH0 overflow
	IntDefaultHandler,//PIT_CH1_IRQHandler, //PIT CH1 overflow
	IntDefaultHandler,//KBI0_IRQHandler, //Keyboard interrupt 0
	IntDefaultHandler,//KBI1_IRQHandler, //Keyboard interrupt 1
	IntDefaultHandler,//Reserved42_IRQHandler, //Reserved interrupt 42
	IntDefaultHandler,//ICS_IRQHandler, //MCG interrupt
	IntDefaultHandler,//Watchdog_IRQHandler, //WDOG Interrupt
	IntDefaultHandler,//Reserved45_IRQHandler, //Reserved interrupt 45
	IntDefaultHandler,//Reserved46_IRQHandler, //Reserved interrupt 46
	IntDefaultHandler//Reserved47_IRQHandler //Reserved interrupt 47
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
	void
ResetISR(void)
{
	uint32_t *pui32Src, *pui32Dest;

	//
	// Copy the data segment initializers from flash to SRAM.
	//
	pui32Src = &_etext;
	for(pui32Dest = &_data; pui32Dest < &_edata; )
	{
		*pui32Dest++ = *pui32Src++;
	}

	//
	// Zero fill the bss segment.
	//
	for(pui32Dest = &_bss; pui32Dest < &_ebss; )
	{
		*pui32Dest++ = 0;
	}

	//
	// Call the application's entry point.
	//
	SystemInit();
	main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
	static void
NmiSR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
	static void
FaultISR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
	static void
IntDefaultHandler(void)
{
	//
	// Go into an infinite loop.
	//
	while(1)
	{
	}
}
