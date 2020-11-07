/*
 * registers-initialize.c
 * initialize all i/o, timer, adc, ... registers
 */

#include "registers.h"
#include <avr/io.h>

static void
io_init (void)
{
	DDRD =
        (1 << DDRD4) |
        (0 << DDRD3) |
        (1 << DDRD2) |
        (1 << 0);
	PORTD =
        (0 << PORTD4) |
        (1 << PORTD3) |
        (0 << PORTD2) |
        (1 << 0);

	DDRB =
        (1 << DDRB1) |
        (1 << 0);
	PORTB =
        (1 << PORTB1) |
        (1 << 0);
}

/*
    Timer/Counter 0 initialization
    Clock source: System Clock
    Clock value: 31.25 kHz
    Timer Period : 4 ms
*/
static void
timer0_init (void)
{
	/*
	    clkio/64 (prescaler = 256)
	    clktimer0 = 31.25 KHz
	*/
	TCCR0 = (1 << CS02) | (0 << CS01) | (0 << CS00);

	/* top = 0x83; timer increments to 256 - 131 = 125. */
	TCNT0 = 0x83;
}

/*
    Timer/Counter 1 initialization
    Clock source: System Clock
    Clock value: 8 MHz (CLKio/1)
    Mode: Ph. and Freq. correct PWM top = ICR1
    OC1A output: Inverted PWM
    PWM Frequency = 40KHz
*/
static void
timer1_init (void)
{
	TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) |
	         (0 << FOC1A) | (1 << FOC1B) | (0 << WGM11) | (0 << WGM10);

	TCCR1B = (0 << ICNC1) | (0 << ICES1) | (1 << WGM13) | (0 << WGM12) |
	         (0 << CS12) | (0 << CS11) | (1 << CS10);

	TCNT1 = 0;
	ICR1 = 100; // TOP = 100;
	OCR1A = 0;
	OCR1B = 0;
}

static void
timer_interrupts_init (void)
{
	TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) |
	        (0 << OCIE1B) | (0 << TOIE1) | (1 << TOIE0);
}

void
registers_init (void)
{
	io_init ();
	timer0_init ();
	timer1_init ();
	timer_interrupts_init ();
}
