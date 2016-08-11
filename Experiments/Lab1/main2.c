/*

* Author: Umang Chhaparia

* Description: This Use switch SW2 and sw2Status (a variable). Your program should increment sw2Status by one, every time switch is pressed.

* Filename: main2.c

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

* Description: Set PORTF Pin 0 as input. On this pin SW2 are connected.

* Example Call: io_pin_config();

*/

void io_pin_config(void)
{
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; //For Unlocking critical function GPIO Protection
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;  //For Unlocking critical function GPIO Protection
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;   //For Unlocking critical function GPIO Protection
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);  //Set Pin 0 as input to which SW2 is connected
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //To configure Pin 1,2,3 as Output LED pins
}


int main(void)
{
	int32_t i32Val;   // Reads the input from the switch
	int sw2Status=0, flag = 0;  //flag is used to ensure that one switch press is counted just once

	setup();   //Set crystal frequency and enable GPIO Peripherals

	io_pin_config();  // Configure I/O ports

	while(1)
	{
		i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);  // Reads the value of the switch into the 32 bit register

		if ((i32Val & 0x0001) == 0x0001)  //Checks SW2 press
			flag=1;

		if(((i32Val & 0x0001) != 0x0001) && (flag==1))  //This block ensures that one Switch press is counted just once
		{
			sw2Status++;
			falg=0;
		}

	}
}

