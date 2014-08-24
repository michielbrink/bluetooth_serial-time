void delay ( unsigned int );
void pulseClock ( void );
void sendline ( unsigned char val );
void send ( void );
void pinWrite ( unsigned int, unsigned char );

void delay(unsigned int ms)
{
 while (ms--)
    {
        __delay_cycles(16000); // set for 16Mhz change it to 1000 for 1 Mhz       16000
    }
}
 
void sendline(unsigned char val)
{
  char a;
  char b;
  
  for (a = 8; a > 0; a--)
  {
  	for (b = 8; b > 0; b--)
	{
  	pinWrite(DATA, ((getallen[tekst[a-1]][val] >> (b-1)) & 1));
   	pulseClock();
   	}
  }
}

void send(void)
{
  	  sendline(0);
  	  P1OUT |= ROW0;
  	  delay(tijd);
 	  P1OUT ^= ROW0;
 	  
 	  sendline(1);
 	  P1OUT |= ROW1;
 	  delay(tijd);
 	  P1OUT ^= ROW1;
 	  
 	  sendline(2);
 	  P1OUT |= ROW2;
 	  delay(tijd);
 	  P1OUT ^= ROW2;
 	  
 	  sendline(3);
 	  P1OUT |= ROW3;
 	  delay(tijd);
 	  P1OUT ^= ROW3;
 	  
 	  sendline(4);
 	  P1OUT |= ROW4;
 	  delay(tijd);
 	  P1OUT ^= ROW4;
 	  
 	  sendline(5);
 	  P1OUT |= ROW5;
 	  delay(tijd);
 	  P1OUT ^= ROW5;
 	  
 	  sendline(6);
 	  P1OUT |= ROW6;
 	  delay(tijd);
 	  P1OUT ^= ROW6;
}

// Writes a value to the specified bitmask/pin. Use built in defines
// when calling this, as the shiftOut() function does.
// All nonzero values are treated as "high" and zero is "low"
void pinWrite( unsigned int bit, unsigned char val )
{
  if (val){
    P2OUT |= bit;
  } else {
    P2OUT &= ~bit;
  }
}
 
// Pulse the clock pin
void pulseClock( void )
{
  P2OUT |= CLOCK;
  P2OUT ^= CLOCK;
}
