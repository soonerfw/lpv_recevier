
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_types.h"



#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3


//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void Timer0IntHandler(void)
{

	static uint32_t tickCount=0;
    //
    // Clear the timer interrupt.
    //
  MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		
	tickCount++;

	if(tickCount%25 ==0)
	{
		    if(GPIOPinRead(GPIO_PORTF_BASE,GREEN_LED)) 
				{				
					// Turn off the LED.
					//
					GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, 0x0);
				}
		    else					
				{			
					// Turn on the LED.
					//
					GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, GREEN_LED);					
					
				}
	}	

}


