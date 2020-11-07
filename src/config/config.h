#include <avr/io.h>
#include "neomacros.h"
#include "tasker.h"
#include "taskmacros.h"


#define BUTTON       D,3
#define LED_DOWN     D,2
#define LED_UP       D,4


static void
io_init (void) {
    in(BUTTON);
    out(LED_UP);
    out(LED_DOWN);
}


#define MAX_TASK_COUNT ((uint8_t)20u) /* Max number of tasks that can be registered. */
#define TASK_MAX_PERIOD ((uint16_t)10000u) /* Max allowed task run period. */
#define TASK_MIN_PERIOD ((uint8_t)1u) /* Minimum allowed task run period. */

static Task task_array[] = {
    { debounce,                 RUNNABLE,        1  },
    { button_event_handler,     SUSPENDED,       13 },
    { motor_startup,            SUSPENDED,       5  },
    { motor_shutdown,           SUSPENDED,       9  },
    { motor_rampup,             SUSPENDED,       9  },
    { motor_rampdown,           SUSPENDED,       9  },

    {   blink,
        SUSPENDED,
        27
    },
    {   blink_upper,
        SUSPENDED,
        85
    },
    {   blink_lower,
        SUSPENDED,
        1
    },
    {   blink_upper_secondary,
        SUSPENDED,
        14
    },
    {   blink_lower_secondary,
        SUSPENDED,
        1
    },
    {   motor_toggle_speed_control,
        SUSPENDED,
        69
    },
    {   motor_toggle_on_off,
        SUSPENDED,
        69
    },

    { blink_upper,             SUSPENDED,        1 },
    { motor_rampdown,           SUSPENDED,       1 },
    { motor_rampdown,           SUSPENDED,       1 },
};

#define TCNT0_VALUE 0x83
static void
timer0_init (void) {
	TCCR0 =
        (1 << CS02) |
        (0 << CS01) |
        (0 << CS00);
    TCNT0 = 0x83; /* top = 0x83; timer increments to 256 - 131 = 125. */
}

static void
timer1_init (void) {
	TCCR1A =
        (1 << COM1A1) |
        (1 << COM1A0) |
        (0 << COM1B1) |
        (0 << COM1B0) |
	    (0 << FOC1A)  |
        (1 << FOC1B)  |
        (0 << WGM11)  |
        (0 << WGM10);

	TCCR1B =
        (0 << ICNC1) |
        (0 << ICES1) |
        (1 << WGM13) |
        (0 << WGM12) |
	    (0 << CS12)  |
        (0 << CS11)  |
        (1 << CS10);

	TCNT1 = 0;
	ICR1 = 100;  /* TOP = 100; */
	OCR1A = 0;
	OCR1B = 0;
}

static void
timer_interrupts_init (void) {
	TIMSK =
        (0 << OCIE2)  |
        (0 << TOIE2)  |
        (0 << TICIE1) |
        (0 << OCIE1A) |
	    (0 << OCIE1B) |
        (0 << TOIE1)  |
        (1 << TOIE0);
}

void
registers_init (void)
{
	io_init ();
	timer0_init ();
	timer1_init ();
	timer_interrupts_init ();
}













/*
 * low-level macros for I/O manipulation.
 * try not to actually use these in your code,
 * but use them to define more useful macros down below.
 */

#define SETBIT(ADDRESS, BIT)   (ADDRESS |= (1 << BIT))
#define CLEARBIT(ADDRESS, BIT) (ADDRESS &= ~(1 << BIT))
#define FLIPBIT(ADDRESS, BIT)  (ADDRESS ^= (1 << BIT))
#define GETBIT(ADDRESS, BIT)   (ADDRESS & (1 << BIT))

/*
 * set useful names for I/O pins.
 * example:
 *
 *  #define BUTTON1_PORT PINB
 *  #define BUTTON1_PIN  PINB4
 *  #define LED_PORT     PORTC
 *  #define LED_PIN      PORTC3
*/
#define BUTTON_PORT   PIND
#define BUTTON_PIN    PIND3
#define LED_DOWN_PORT PORTD
#define LED_DOWN_PIN  PORTD2
#define LED_UP_PORT   PORTD
#define LED_UP_PIN    PORTD4

/*
 * and finally, define some nice, meaningful I/O actions.
 *  example:
 *
 *  #define READ_BUTTON1 GETBIT(BUTTON1_PORT,BUTTON1_PIN)
 *  #define SET_LED      SETBIT(LED_PORT,LED_PIN)
*/
#define READ_BUTTON       GETBIT (BUTTON_PORT,   BUTTON_PIN)
#define LED_DOWN_ON       SETBIT (LED_DOWN_PORT, LED_DOWN_PIN)
#define LED_DOWN_OFF    CLEARBIT (LED_DOWN_PORT, LED_DOWN_PIN)
#define LED_UP_ON         SETBIT (LED_UP_PORT,   LED_UP_PIN)
#define LED_UP_OFF      CLEARBIT (LED_UP_PORT,   LED_UP_PIN)
#define LED_UP_TOGGLE    FLIPBIT (LED_UP_PORT,   LED_UP_PIN)
#define LED_DOWN_TOGGLE  FLIPBIT (LED_DOWN_PORT, LED_DOWN_PIN)
