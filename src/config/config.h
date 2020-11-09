#include <avr/io.h>
#include "supermacros.h"
#include "tasker.h"
#include "blink.h"
#include "button.h"
#include "motor.h"

#define F_CPU 8000000


// =====================
// = Tasks
// =====================
#ifdef CFG_TASKS

/* Define tasks, their initial state and their run period */
static Task task_array[] = {
    { debounce,                 RUNNABLE,        1  },
    { button_event_handler,     SUSPENDED,       13 },
    { motor_startup,            SUSPENDED,       5  },
    { motor_shutdown,           SUSPENDED,       9  },
    { motor_rampup,             SUSPENDED,       9  },
    { motor_rampdown,           SUSPENDED,       9  },
    { blink,                    SUSPENDED,       27 },
    { blink_upper,              SUSPENDED,       85 },
    { blink_lower,              SUSPENDED,       1  },
    { blink_upper_secondary,    SUSPENDED,       14 },
    { blink_lower_secondary,    SUSPENDED,       1  },
    { motor_toggle_speed_control, SUSPENDED,     69 },
    { motor_toggle_on_off,      SUSPENDED,       69 },
    { blink_upper,              SUSPENDED,       1  },
    { motor_rampdown,           SUSPENDED,       1  },
    { motor_rampdown,           SUSPENDED,       1  },
};

#endif

// =====================
// = Input/Output
// =====================
#ifdef CFG_IO

/* Put meaningful names on I/O pins */
#define BUTTON       D,3
#define LED_DOWN     D,2
#define LED_UP       D,4

#endif


// =====================
// = Registers
// =====================
#ifdef CFG_REG

/* Define I/O pins as inputs or outputs, and other I/O settings */
static void
io_init (void) {

    INPUT(BUTTON);
    PULLUP(BUTTON);

    OUTPUT(LED_UP);
    OFF(LED_UP);

    OUTPUT(LED_DOWN);
    ON(LED_DOWN);
}


#define TCNT0_VALUE 0x83

static void timer0_init (void) {

	TCCR0 =
        (1 << CS02) |
        (0 << CS01) |
        (0 << CS00);

    TCNT0 = TCNT0_VALUE;
}

static void timer1_init (void) {

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

static void timer_interrupts_init (void) {

	TIMSK =
        (0 << OCIE2)  |
        (0 << TOIE2)  |
        (0 << TICIE1) |
        (0 << OCIE1A) |
	    (0 << OCIE1B) |
        (0 << TOIE1)  |
        (1 << TOIE0);
}

static void registers_init (void) {

	io_init ();
	timer0_init ();
	timer1_init ();
	timer_interrupts_init ();
}

#endif
