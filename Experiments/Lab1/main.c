/*

* Author: Umang Chhaparia

* Description: Use switch SW1 to Turn on Red LED on first switch press, Green LED on second switch press and Blue LED on third switch press.

* Filename: main.c

* Functions: setup(), io_pin_config(), main()

* Global Variables: none

*/
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

/*

* Function Name: io_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set PORTF Pin 4 as input. On this pin SW1 are connected.

* Example Call: io_pin_config();

*/

void io_pin_config(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //To configure Pin 1,2,3 as Output LED pins
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);   //Set Pin 4 as input to which SW1 is connected
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);   //To set input pins as high inpedance
}


int main(void)
{
	uint8_t ui8LED = 2;   // Variable controls the colour of the LED
	int32_t i32Val;    // Reads the input from the switch

	setup();   //Set crystal frequency and enable GPIO Peripherals

	io_pin_config();   // Configure I/O ports

	while(1)
	{
		// Turn on the LED
		i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);  // Reads the value of the switch into the 32 bit register
		if(i32Val!=16)  // if switch is presses, change the colour of the LED
		{
			if (ui8LED == 8)
					{
						ui8LED = 2;
					}
					else
					{
						ui8LED = ui8LED*2;
					}
		}

		while(i32Val!=16)  // To keep the LED on till the switch is pressed
		{
			i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		}

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0); // To switch off the LED once the switch is released

	}
}
