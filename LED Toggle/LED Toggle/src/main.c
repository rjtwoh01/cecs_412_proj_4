#include <avr/interrupt.h>
#include <delay.h>
#define sbi(ADDRESS,BIT) ADDRESS |= (1<<BIT) // set Bit
#define cbi(ADDRESS,BIT) ADDRESS &= ~(1<<BIT) // clear Bit
#define toggle(ADDRESS,BIT) ADDRESS ^= (1<<BIT) // change bit
void initialize(void)
{
	//Disable Interrupts Globally
	cli();
	// set PORTF pin for input, connected to push button (see schematics)
	cbi(PORTF.DIR,1);
	// set PORTF input Pin for interrupting mode
	sbi(PORTF.INT0MASK,1);
	// pullup the PORTF input pin, sense for falling edge trigger
	PORTF.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc | PORT_ISC_FALLING_gc;
	// set PORTF and its INT0MASK for LO level priority
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc;
	//set Interrupt Priority Level Configuration
	PMIC.CTRL|=PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;
	//set data direction for PORT R to all outputs, one drives the LED
	PORTR.DIRSET = 0xFF;
	//Enable Interrupts Globally
	sei();
}
ISR(PORTF_INT0_vect)
{
	delay_ms(100);
	toggle(PORTR.OUT,0); // LED toggle
	toggle(PORTR.OUT, 1);
}
int main (void)
{
	initialize(); // Load controller setup
	toggle(PORTR.OUT, 1);
	while(1)
	{
	}
}