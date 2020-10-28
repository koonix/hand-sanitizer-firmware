/*
 * scheduler.c
 * a simple tunable co-operative
 * task scheduler. capable of
 * running all the tasks in one cycle.
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "tasker.h"
#include "registers.h"
#include "taskmacros.h"

#define F_CPU 8000000UL

int
main (void)
{
	registers_init ();
	sei ();

	tsk_task_create (debounce, 1, PENDING); // 0 4
	tsk_task_create (button_event_handler, 13, PENDING); // 1 50

	tsk_task_create (motor_startup, 5, SUSPENDED); // 2 35
	tsk_task_create (motor_shutdown, 9, SUSPENDED); // 3 67
	tsk_task_create (motor_rampup, 9, SUSPENDED); // 4 67
	tsk_task_create (motor_rampdown, 9, SUSPENDED); // 5 67

	tsk_task_create (blink, 27, PENDING); // 6 100
	tsk_task_create (blink_upper, 85, SUSPENDED); // 7 340
	tsk_task_create (blink_upper_secondary, 1, SUSPENDED); // 8 4

	tsk_task_create (blink_lower, 14, SUSPENDED); // 9 55
	tsk_task_create (blink_lower_secondary, 1, SUSPENDED); // 10 3

	tsk_task_create (motor_toggle_speed_control, 69, SUSPENDED); // 11 np
	tsk_task_create (motor_toggle_on_off, 69, SUSPENDED); // 12 np

	while (1)
		tsk_task_runner ();
}


ISR (TIMER0_OVF_vect)
{
	TCNT0 = 0x83;
	tsk_task_time_manager ();
}
