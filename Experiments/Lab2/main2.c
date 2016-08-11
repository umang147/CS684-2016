/*
 * main.c
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
#include "inc/hw_gpio.h"

//uint8_t ui8LED = 2;
uint32_t i32Val;
int sw2Status = 0;


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

* Function Name: SW_Press_inc()

* Input: none

* Output: none

* Description: Increses sw2States after Switch debouncing

* Example Call: SW_Press_inc();

*/

void SW_Press_inc(void)
{
	i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

	if (i32Val != 1)  //If the Switch is still pressed after sw2Status is increased
		sw2Status++;
}

int main(void)
{
	int flag = 0;

	setup();   //Set crystal frequency and enable GPIO Peripherals

	io_pin_config();  // Configure I/O ports

	timer_config();  //Configure Timer

	while(1)
	{
		i32Val = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

		if (i32Val != 1 && flag==1)  //Checks for switch press, If ON, Puts the timer ON
		TimerEnable(TIMER0_BASE, TIMER_A);

		if (i32Val != 1)
			flag=0;
		else
			flag=1;

	}
}

void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Clear the timer interrupt

	SW_Press_inc();
}
