//*****************************************************************************
//
// fonttest.c - Simple font testcase.
//
// Copyright (c) 2010-2020 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 2.2.0.295 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

#include "../grlib/grlib.h"

#include "../drivers/Kentec320x240x16_ssd2119_spi.h"


#include "../third_party/fonts/ofl/ofl_fonts.h"
#include "../drivers/frame.h"

#include "funcs.h"
#include "timercmd.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("Runtime error at line %d of %s!\n", ui32Line, pcFilename);
    while(1)
    {
    }
}
#endif

//*****************************************************************************
//
// Send a string to the UART0.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        MAP_UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// Main entry function for the fontview application.
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32SysClock;
	  uint8_t ch;
    uint8_t buff[6];
		tContext sContext;
    //
    // The FPU should be enabled because some compilers will use floating-
    // point registers, even for non-floating-point code.  If the FPU is not
    // enabled this will cause a fault.  This also ensures that floating-
    // point operations could be added to this application and would work
    // correctly and use the hardware floating-point unit.  Finally, lazy
    // stacking is enabled for interrupt handlers.  This allows floating-
    // point instructions to be used within interrupt handlers, but at the
    // expense of extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    //
    // Set the clock to 40Mhz derived from the PLL and the external oscillator
    //
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);

    ui32SysClock = MAP_SysCtlClockGet();

    //

	  //
		// *******************************
		// *    initialize UART 0        *
		// *                             *
		// *******************************		
    // UART0 initialization
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // enable UART0 
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable PIOA, RX, TX Pins
		//
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);                 // configure PA0 - UART0 RxD for mulplex pins
    GPIOPinConfigure(GPIO_PA1_U0TX);                 // configure PA1 - UART0 TxD for mulplex pins
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // configure PA0 - UART0 Rxd, PA1 - UART0 TxD

    //
    // Configure the UART for 115,200, 8-N-1 operation
    //
    MAP_UARTConfigSetExpClk(UART0_BASE, ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));


#if 1
		//
		// *******************************
		// *    initialize UART 1        *
		// *                             *
		// *******************************		
    // UART1 initialization
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); // enable UART1
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // enable PIOB, RX, TX Pins
		//
    // Set GPIO PB0 and PB1 as UART1 pins.
    //
    GPIOPinConfigure(GPIO_PB0_U1RX);                 // configure PB0 - UART1 RxD for mulplex pins
    GPIOPinConfigure(GPIO_PB1_U1TX);                 // configure PB1 - UART1 TxD for mulplex pins
    MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); // configure PB0 - UART1 Rxd, PB1 - UART1 TxD

    //
    // Configure the UART1 for 9600, 8-N-1 operation
    //
    MAP_UARTConfigSetExpClk(UART1_BASE, ui32SysClock, 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

  //
		// *******************************
		// *    initialize UART 3        *
		// *                             *
		// *******************************		
    // UART3 initialization
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3); // enable UART0 
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); // enable PIOA, RX, TX Pins
		//
    // Set GPIO PC6 and PC7 as UART3 pins.
    //
    GPIOPinConfigure(GPIO_PC6_U3RX);                 // configure PC6 - UART3 RxD for mulplex pins
    GPIOPinConfigure(GPIO_PC7_U3TX);                 // configure PC7 - UART3 TxD for mulplex pins
    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7); // configure PC6 - UART3 Rxd, PC7 - UART3 TxD

    //
    // Configure the UART3 for 9600, 8-N-1 operation
    //
    MAP_UARTConfigSetExpClk(UART3_BASE, ui32SysClock, 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

#endif
		// *******************************
		// *    initialize Port F LED    *
		// *    PF3 heart beat           *
		// *******************************
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function. heart beat, flash Green LED
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GREEN_LED);

		//
		// *******************************
		// *    initialize TIMER         *
		// *                             *
		// *                             *
		// *******************************
    //  Timer 0 - periodic time, interrupt enabled, 20 ms period, no split, 
    // Enable the peripherals used by this example.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // timer0 must be enabled first

    //
    // Configure the two 32-bit periodic timers.
    //
    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // periodic timer 0
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, ui32SysClock/TICKS_PER_SEC); // every 20 ms interrupt

#if 0

    // *********************************
		// *                               *
    // * Initialize the display driver *
    // *                               *
		// *********************************
    Kentec320x240x16_SSD2119Init(ui32SysClock);
 
    //
    // Initialize the graphics context.
    //
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);

    //
    // Draw the application frame.
    //
    FrameDraw(&sContext, "LPV Status");

    //
    // Display UART configuration on the display.
    //
    GrStringDraw(&sContext, "Port:",       -1,  70, 70, 0);
    GrStringDraw(&sContext, "Baud:",       -1,  70, 95, 0);
    GrStringDraw(&sContext, "Data:",       -1,  70, 120, 0);
    GrStringDraw(&sContext, "Parity:",     -1,  70, 145, 0);
    GrStringDraw(&sContext, "Stop:",       -1,  70, 170, 0);
    GrStringDraw(&sContext, "Uart 0",      -1, 150, 70, 0);
    GrStringDraw(&sContext, "115,200 bps", -1, 150, 95, 0);
    GrStringDraw(&sContext, "8 Bit",       -1, 150, 120, 0);
    GrStringDraw(&sContext, "None",        -1, 150, 145, 0);
    GrStringDraw(&sContext, "1 Bit",       -1, 150, 170, 0);
#endif 
	  //**********************************
		//*                                *
		//*    enable interrupt only after *
		//*   peripherals are initialized  *
		//*                                *
		//**********************************
    //
    // Setup the interrupts for the timer timeouts.
    //
    // Enable Interrupts
    //
    MAP_IntMasterEnable(); // CPU interrupt primary mask register
    MAP_IntEnable(INT_TIMER0A); // NVIC enable TIMER0A 
    MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // TIMER0 A enable interrupt, and interrupt from time out

    //
    // Enable the timers.
    //
    MAP_TimerEnable(TIMER0_BASE, TIMER_A); // enable timer0 
	  //
    //
    while(1)
    {
        //
        // read UART0, any byte received?, If yes, process it accorrdingly, 
		    if (MAP_UARTCharsAvail(UART0_BASE))
		      {
			 	    ch = MAP_UARTCharGetNonBlocking(UART0_BASE); 
						MAP_UARTCharPut(UART0_BASE,ch);
#if 0						
						GrStringDraw(&sContext, "Port:",       -1,  70, 70, 0);						
						GrStringDraw(&sContext, "Baud:",       -1,  70, 95, 0);
						GrStringDraw(&sContext, "Data:",       -1,  70, 120, 0);
#endif 
		      }		
				 sprintf((char *)buff,"%6.2f",123.4567);
				 UARTSend(buff,6);
				 MAP_UARTCharPut(UART0_BASE,0x0D);
				 MAP_UARTCharPut(UART0_BASE,0x0A);
					
			 // read UART1 and 3, process is accordingly.

		    if (MAP_UARTCharsAvail(UART1_BASE))  // UART1 data available
				{
					
					
				}
		    if (MAP_UARTCharsAvail(UART3_BASE))  // UART3 data available
				{
					
					
				}
				
					
					
    }
}
