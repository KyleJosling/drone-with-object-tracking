#include <SPI.h>
//100 bit buffer
char buf[100];
//volatile lets compiler know this variable may change at any time
volatile byte pos;
volatile boolean process_it;

void setup() {

  Serial.begin(115200); //For debug

  //This sets the bit that turns slave mode on
  //SPCR = SPI Control Register
  //SPE is the bit that needs to be enabled to turn on slave mode
  SPCR |= bit (SPE);

  //We have to set master in, slave out
  pinMode(MISO, OUTPUT);

  //Get ready for an interrupt

  pos = 0; //buffer empty
  process_it = false;

  //Turn on interrupts.

  SPI.attachInterrupt();

}
//We make an interrupt service routine for a hardware interrupt (data being sent from master)
//SPI_STC_vect is the interrupt vector for when the serial transfer is complete.
ISR (SPI_STC_vect)
{
  byte c = SPDR; //Grab byte from the SPI data register

  //if there is room in the buffer , add it

  if (pos < (sizeof(buf) - 1))
    buf[pos++] = c;

  //if there is a new line, we must process the buffer.
  if (c=='\n')
    process_it=true;
}
void loop() {

//Wait for a flag to set an interrupt routine

	if (process_it)
	{
		buf[pos] = 0;//Reset 
		Serial.println(buf);
		pos = 0;
		process_it = false;

	}



}
