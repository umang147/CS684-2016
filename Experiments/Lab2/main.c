/*

* Author: Umang Chhaparia

* Description: Use sw1 to change the color of the led (R -> G -> B -> R. . . .) where you should press the switch just once instead of long press in Lab 1.

* Filename: main.c

* Functions: setup(), io_pin_config(), main()

* Global Variables: ui8LED, i32val

*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

uint8_t ui8LED = 2;  //ui8LED is used to decide the colour of LED
uint32_t i32Val; ////Corresponds to the value read form PORTF for the configured input pins

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
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
	HGPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  //Set Pin 0 & Pin 4 as input to which SW2 and SW1 is connected respectively
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

/*

* Function Name: timer_config()

* Input: none

* Output: none

* Description: Sets the timer of 10 ms

* Example Call: timer_config();

*/

void timer_config(void)
{
	uint32_t ui32Period;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
	ui32Period = (SysCtlClockGet() / 100);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
}

/*

* Function Name: LED_toggle()

* Input: none

* Output: none

* Description: Toggle the LED if after Switch debouncing

* Example Call: LED_toggle();

*/

void LED_toggle(void)
{
	if (i32Val != 16) //If the Switch is still pressed after 10 ms, LED is togggled
	{

		if (ui8LED == 8)
		{
			ui8LED = 2;
		}

		else
		{
			ui8LED = ui8LED*2;
		}

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);

	}
}

int main(void)
{
	int flag = 0; //flag ensures that switch press is counted just once

	setup();   //Set crystal frequency and enable GPIO Peripherals

	io_pin_config();  // Configure I/O ports

	timer_config();  //Configure Timer

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0); //Switch off LED

	while(1)
	{
		i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
		if (i32Val != 16 && flag==1) //Checks for switch press, If ON, Puts the timer ON
		TimerEnable(TIMER0_BASE, TIMER_A);
		if (i32Val != 16)
			flag=0;
		else
			flag=1;

	}
}

void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Clear the timer interrupt

	LED_toggle();
}
