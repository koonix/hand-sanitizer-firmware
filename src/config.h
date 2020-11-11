/*
 * config.h
 * most of the project's configuration is done here.
 * you just have to write the tasks, and put
 * interrupt service routines in main.c.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <stdint.h>
#include "iomacros.h"
#include "blink.h"
#include "button.h"
#include "motor.h"

#define F_CPU 8000000UL

// =====================
// = Input/Output
// =====================

/* Put meaningful names on I/O pins */
#define BUTTON   D,3
#define LED_DOWN D,2
#define LED_UP   D,4

// =====================
// = Tasks
// =====================

/* how often the tsk_task_time_manager runs (in milliseconds) */
#define TASK_TIME_INTERVAL_MSEC 4
#define MSEC(t) (((TaskTime)t/TASK_TIME_INTERVAL_MSEC)+1)

#ifdef MAIN_C

#include "tasker.h"

/* Define tasks, their initial state and their run period */
static void tasks_init(void)
{
    tsk_task_create ( blink_upper,                PAUSED,   MSEC(4)  );
    tsk_task_create ( debounce,                 RUNNABLE,   MSEC(4)  );
    tsk_task_create ( motor_startup,              PAUSED,   MSEC(20) );
    tsk_task_create ( motor_shutdown,             PAUSED,   MSEC(36) );
    tsk_task_create ( motor_rampup,               PAUSED,   MSEC(36) );
    tsk_task_create ( motor_rampdown,             PAUSED,   MSEC(36) );
    tsk_task_create ( motor_toggle_speed_control, PAUSED,   MSEC(24) );
    tsk_task_create ( motor_toggle_on_off,        PAUSED,   MSEC(24) );
    tsk_task_create ( button_event_handler,       PAUSED,   MSEC(4)  );
    tsk_task_create ( blink,                      PAUSED,   MSEC(8)  );
    tsk_task_create ( blink_upper,                PAUSED,   MSEC(32) );
    tsk_task_create ( blink_lower,                PAUSED,   MSEC(4)  );
    tsk_task_create ( blink_upper_secondary,      PAUSED,   MSEC(4)  );
    tsk_task_create ( blink_lower_secondary,      PAUSED,   MSEC(4)  );
}

// =====================
// = Registers
// =====================

/* Define I/O pins as inputs or outputs, and other I/O settings */
static void io_init(void)
{
    INPUT(BUTTON);
    PULLUP(BUTTON);

    OUTPUT(LED_UP);
    OFF(LED_UP);

    OUTPUT(LED_DOWN);
    ON(LED_DOWN);
}

/* This is the timer-counter's initial value.
   The timer-counter register shall be initialiez to this value
   on startup (in this function) and in timer's ovf interrupt. */
#define TCNT0_VALUE 0x83

static void timer0_init(void)
{
    TCCR0 =
        (1 << CS02) |
        (0 << CS01) |
        (0 << CS00);

    TCNT0 = TCNT0_VALUE;
}

static void timer1_init(void)
{
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
    ICR1  = 100; /* TOP = 100; */
    OCR1A = 0;
    OCR1B = 0;
}

static void timer_interrupts_init(void)
{
    TIMSK =
        (0 << OCIE2)  |
        (0 << TOIE2)  |
        (0 << TICIE1) |
        (0 << OCIE1A) |
        (0 << OCIE1B) |
        (0 << TOIE1)  |
        (1 << TOIE0);
}

static void registers_init(void)
{
    io_init();
    timer0_init();
    timer1_init();
    timer_interrupts_init();
}

#endif /* MAIN_C */
#endif /* CONFIG_H */
