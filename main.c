

#include <msp430.h>				

#define TX BIT3
#define LED0 BIT0
#define Switch1 BIT2

void send_rf(unsigned char, unsigned char*);
void zero(unsigned char);
void one(unsigned char);
void f(unsigned char);

unsigned char TXdata[12] = {0,2,2,2,2,2,0,0,1,0,0,0};
unsigned char ON =0;
unsigned char oldON =0;
unsigned char ii=0;
int ONCount =0;
int ledcount =0;


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  P1DIR |= (LED0);
	  P2DIR |= TX;// Set all pins but RXD to output
	  P2OUT &= ~TX;// Set all pins but RXD to output
	  P1REN |= Switch1;
	  P1OUT |= Switch1;

	  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts
	while(1)
	{
		ONCount=0;
		for(ii=10;ii>0;ii--)
		             {
		           	    if((P1IN & Switch1) == Switch1)
		           	    {
		           	    	ONCount++;
		           	    }
		           	    _delay_cycles(10);
		             }

		        if(ONCount >8)
		        {
		        ON = 1;
		        }
		        else
		        {
		        	ON =0;
		        }


       if(oldON != ON)
       {
    	   if(ON == 1 )
    	   {
		       _delay_cycles(10000);
    	   }
      		send_rf(TX, TXdata);
      		send_rf(TX, TXdata);
      		send_rf(TX, TXdata);
      		send_rf(TX, TXdata);
      		P1OUT |= LED0;
      		ledcount=0;
      		if(ON == 0)
      		{
          		_delay_cycles(100000);
      		}
       }
       ledcount++;
       if(ledcount> 1000)
       {
    	   P1OUT &= ~LED0;
    	   ledcount =1000;
       }
           		oldON = ON;
	}
}

void send_rf(unsigned char pin, unsigned char* data)
{
unsigned char i=0;

for(i=0;i<12;i++)
{
if(data[i] == 0)
{
	zero(pin);
}
else if(data[i] ==1)
{
	one(pin);
}
else
{
	f(pin);
}

}

P2OUT |= pin;
_delay_cycles(250);
P2OUT &= ~pin;
_delay_cycles(7000);
}

void zero(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(690);
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(690);
}
void one(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(210);
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(210);
}
void f(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(250);
	P2OUT &= ~pin;
	_delay_cycles(700);
	P2OUT |= pin;
	_delay_cycles(810);
	P2OUT &= ~pin;
	_delay_cycles(200);
}

