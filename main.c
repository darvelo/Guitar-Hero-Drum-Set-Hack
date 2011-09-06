#include <avr/io.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */


#define F_CPU 1000000

#define PedalPressed			bit_is_set(PINB, PB1)


// Barebones intterupt test failed.. commented out in favor of polling method.
/*ISR(TIMER1_CAPT_vect) {

	PORTB ^= (1<<PB2);
}*/


int main (void) {

	DDRB = 0x04;   // Port B pin 2 is output, pin 1 is input
//	TIMSK1 = ((1<<ICIE1) );  // enable input capture interrupt
//	TCCR1B = (  (1<<ICNC1) | (1<<ICES1) );  // input capture noise canceling, edge select: rising edge
//	sei();   // enable global interrupts

	while (1){
	
		if (PedalPressed) {

		 	PORTB |= (1<<PB2);    // signal turn on
			_delay_ms(40);        // wait 40ms
			PORTB &= ~(1<<PB2);   // signal turn off
			while (PedalPressed);  // wait until pedal is released
		}
	}

}


// Initial method of using interrupts.  Abandoned due to unknown 4-second block until input capture trigger.

/*ISR(TIMER1_CAPT_vect)
{
//	if ( TCCR1B & (1<<ICES1) ) {
		
//		TCCR1B &= ~(1<<ICES1); // set capture interrupt edge select to falling edge
//		PORTB &= ~(1<<PB2);
//		TIMSK1 &= ~(1<<ICIE1); //disable this intterupt
//	}else{
//		TCCR1B |= (1<<ICES1);
//		PORTB |= (1<<PB2);
//	}
			PORTB |= (1<<PB2);
		TCCR1B |= (1<<CS10);  // start timer1 prescale of 8 ( @ 1MHz = 8uS per clock )
		TCCR1C |= (1<<FOC1A);  // send signal for as long until timer compare, which shuts it off

}

ISR(TIMER1_COMPA_vect)
{
	
//	TIMSK1 |= (1<<ICIE1);
	TCCR1B &= ~(1<<CS10);  // turn off timer1
	PORTB &= ~(1<<PB2);

}

void timerSetup () {

	TCCR1A = (1<<COM1A0); // toggle OC1A on timer1 output compare match
	TCCR1B = (  (1<<ICNC1) | (1<<ICES1) | (1<<WGM12) );  // input capture noise canceling, edge select: rising edge, clear on timer compare (CTC)
	TCNT1 = 0x0000;  // reset timer
	OCR1A = 0xFFF0;//0x1388;  // 5000 ticks in hex = 0.04s at 1MHz clock and timer prescale of 8
	TIMSK1 = ((1<<ICIE1) | (1<<OCIE1A) );  // enable input capture interrupt and output compare interrupt
}

int main (void) {

	//PB1 = 0C1A
	DDRB = 0x06; // set PB1 to output mode to enable use of OC1A timer1 output compare signal
	timerSetup();
	sei();
	
	while (1){
	
	}

}*/
