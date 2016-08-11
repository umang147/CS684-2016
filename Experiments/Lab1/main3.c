/*

* Author: Umang Chhaparia

* Description: Configure SW1 and SW2 such that:
Every time SW1 is pressed toggle delay of LED should cycle through approximately 0.5s, 1s, 2s (Of any one color).
Every time SW2 is pressed color of LED should cycle through Red, Green and Blue.

* Filename: main3.c

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
#include "inc/hw_gpio.h"


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
* 			   Set PORTF Pin 0, Pin 4 as input. On this pin SW1 and SW2 are connected.

* Example Call: io_pin_config();

*/

void io_pin_config(void)
{
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  //For Unlocking critical function GPIO Protection
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;  //For Unlocking critical function GPIO Protection
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;  //For Unlocking critical function GPIO Protection
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);  //Set Pin 0 & Pin 4 as input to which SW2 and SW1 is connected respectively
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //To set input pins as high inpedance
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //To configure Pin 1,2,3 as Output LED pins


}


int main(void)
{
	uint8_t ui8LED = 2;  //ui8LED is used to decide the colour of LED
	int32_t i32Val;  //Corresponds to the value read form PORTF for the configured input pins
	int sw2Status=1, flag = 0, flag1 = 0; //flag & flag1 ensure that switch press is counted just once

	setup();   //Set crystal frequency and enable GPIO Peripherals

	io_pin_config();  // Configure I/O ports

	while(1)
	{
		i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);   // Reads the value of the switch into the 32 bit register

		if (i32Val == 1)  //Checks SW2 press
			flag1=1;

		if (i32Val == 1)   //This block sets the mode for the LED
		{
			if(sw2Status==3)
				sw2Status=0;

			sw2Status++;
			flag1=0;

		}

		if(i32Val==16)  //Checks SW1 press
			flag=1;

		if(i32Val==16)  //Sets the colour of LED
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

		if (sw2Status==1)  // Binks colour 1
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);  //LED ON
			SysCtlDelay(6700000);  //Sets a delay of 0.5s
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);  //LED OFF
			SysCtlDelay(6700000);  //Sets a delay of 0.5s
		}

		else if (sw2Status==2)  // Blinks Colour 2
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);  //LED ON
			SysCtlDelay(13400000);  //Sets a delay of 1s
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);  //LED OFF
			SysCtlDelay(13400000);  //Sets a delay of 1s
		}

		else if (sw2Status==3)  //Blinks Colour 3
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);  //LED ON
			SysCtlDelay(23800000);  //Sets a delay of 2s
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);  //LED OFF
			SysCtlDelay(23800000);  //Sets a delay of 2s
		}
	}
}

