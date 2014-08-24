
#include    "stdbool.h"
#include    "msp430g2452.h"

//Define our pins 
#define DATA BIT2 //2
#define CLOCK BIT0 //0
#define ROW0 BIT4 //0
#define ROW1 BIT7 //3
#define ROW2 BIT6 //4
#define ROW3 BIT3 //5
#define ROW4 BIT1 //6
#define ROW5 BIT5 //7
#define ROW6 BIT0 //1
#define	RXD BIT2 //2

#define		Bit_time		104	// 9600 Baud, SMCLK=1MHz (1MHz/9600)=104  1666
#define		Bit_time_5		52	// Time for half a bit.                        833

#define		MODE1			0x31
#define		MODE2			0x32
#define		MODE3			0x33
#define		MODE4			0x34

unsigned char BitCnt;		// Bit count, used when transmitting byte
unsigned int RXByte;		// Value recieved once hasRecieved is set
unsigned int TXByte;

unsigned int i;			// for loop variable
unsigned int mystate = 0;
unsigned int plek = 0;

bool isReceiving;		// Status for when the device is receiving
bool hasReceived;		// Lets the program know when a byte is received
bool arrayactive = false;

void Receive(void);

unsigned char tijd = 1;

/*space=10
 * {126, 17, 17, 17, 126, 0, 127, 2, 4, 4, 2, 127}
 * 
 * a = 11
 * p = 12
 * m = 13
 * : = 14
 * 
 * 
 * 
 */
 
unsigned char tekst[8] = {0,5,4,6,14,8,1,6};
unsigned char tekstbuffer[8] = {0,5,4,6,14,8,1,6};

#include "tekens.h"
#include "functions.h"

int main( void )
{
    
    WDTCTL = WDTPW + WDTHOLD; 	// Stop WDT
    
    BCSCTL1 = CALBC1_1MHZ; // Set range
    DCOCTL = CALDCO_1MHZ; // SMCLK = DCO = 1MHz
     
    P1IES |= RXD;				// RXD Hi/lo edge interrupt
    P1IFG &= ~RXD;				// Clear RXD (flag) before enabling interrupt
    P1IE |= RXD;
    
    P1DIR |= (ROW0 | ROW1 | ROW2 | ROW3 | ROW4 | ROW5 | ROW6); 
    P2DIR |= (CLOCK | DATA);
  
    isReceiving = false;			// Set initial values
    hasReceived = false;
  
    __enable_interrupt();				// Enable interrupts

    __bis_SR_register(GIE);			// interrupts enabled\
  
  while(1)
  {
  	if (hasReceived)		// If the device has recieved a value
		{
			Receive();
		}

  	send();
  }
}

void Receive()
{
	hasReceived = false;
	
	if (arrayactive == true)
	{
		if (RXByte >= 0x30 && RXByte <= 0x39)
		{
			tekst[plek] = (RXByte - 48);
		}
		else if (RXByte == 0x2c)// ,
		{
			plek = plek + 1;
		}
		else if (RXByte == 0x7d) // }
		{
			arrayactive = false;
		}
	}
	else if (RXByte == 0x7b) // {
	{
		plek = 0;
		arrayactive = true;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	if (P1IFG & 0x08)
	{
		P1IFG &= ~0x08;
		if (mystate < 3)
		{
			mystate++;
		}
		else
		{
			mystate = 0;
		}
	}
	else
	{
	isReceiving = true;

	P1IE &= ~RXD;			// Disable RXD interrupt
	P1IFG &= ~RXD;			// Clear RXD IFG (interrupt flag)

  	TACTL = TASSEL_2 + MC_2;	// SMCLK, continuous mode
  	//CCR0 = TAR;			// Initialize compare register
  	//CCR0 += Bit_time_5;		// Set time till first bit
	CCTL0 = OUTMOD1 + CCIE;		// Dissable TX and enable interrupts

	RXByte = 0;			// Initialize RXByte
	BitCnt = 0x9;			// Load Bit counter, 8 bits + ST
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	if(!isReceiving)
	{
		CCR0 += Bit_time;				// Add Offset to CCR0
		if ( BitCnt == 0)				// If all bits TXed
		{
  			TACTL = TASSEL_2;			// SMCLK, timer off (for power consumption)
			CCTL0 &= ~ CCIE ;			// Disable interrupt
		}
		else
		{
			CCTL0 |=  OUTMOD2;			// Set TX bit to 0
			if (TXByte & 0x01)
				CCTL0 &= ~ OUTMOD2;		// If it should be 1, set it to 1
			TXByte = TXByte >> 1;
			BitCnt --;
		}
	}
	else
	{
		CCR0 += Bit_time;						// Add Offset to CCR0
		if ( BitCnt == 0)
		{
  			TACTL = TASSEL_2;					// SMCLK, timer off (for power consumption)
			CCTL0 &= ~ CCIE ;					// Disable interrupt

			isReceiving = false;

			P1IFG &= ~RXD;						// clear RXD IFG (interrupt flag)
			P1IE |= RXD;						// enabled RXD interrupt

			if ( (RXByte & 0x201) == 0x200)		// Validate the start and stop bits are correct
			{
				RXByte = RXByte >> 1;			// Remove start bit
				RXByte &= 0xFF;					// Remove stop bit
				hasReceived = true;
			}
  			__bic_SR_register_on_exit(CPUOFF);	// Enable CPU so the main while loop continues
		}
		else
		{
			if ( (P1IN & RXD) == RXD)			// If bit is set?
				RXByte |= 0x400;			// Set the value in the RXByte
			RXByte = RXByte >> 1;				// Shift the bits down
			BitCnt --;
		}
	}
}
