#include "motor.h"
#include "tasker.h"
#include "taskmacros.h"
#include <avr/io.h>
#include <stdint.h>

// Prototypes:
static void
toggle_motor_state (void);

static void
apply_motor_state (void);

static void
motor_hold (void);

static void
motor_handle_on_off_toggling (void);

static void
motor_initiate_startup (void);

static void
motor_initiate_shutdown (void);


_Bool motor_is_on = 0;
uint8_t motor_state = MOTOR_HOLD_BEFORE_RAMPUP_STATE;
static _Bool motor_is_shutting_down_or_starting_up = 0;


void
motor_toggle_speed_control (void)
{
	tsk_set_task_state (TASK_MOTOR_TOGGLE_SPEED_CONTROL, PAUSED);
	if (motor_is_on && !motor_is_shutting_down_or_starting_up)
		toggle_motor_state ();
}

static void
toggle_motor_state (void)
{
	motor_state++;
	if (motor_state == 4)
		motor_state = 0;

	apply_motor_state ();
}

static void
apply_motor_state (void)
{
	switch (motor_state)
	{
	case MOTOR_HOLD_BEFORE_RAMPUP_STATE:
	case MOTOR_HOLD_BEFORE_RAMPDOWN_STATE:
		motor_hold ();
		break;
	case MOTOR_RAMPUP_STATE:
		tsk_set_task_state (TASK_MOTOR_RAMPUP, RUNNABLE);
		break;
	case MOTOR_RAMPDOWN_STATE:
		tsk_set_task_state (TASK_MOTOR_RAMPDOWN, RUNNABLE);
		break;
	default:
		break;
	}
}


static void
motor_hold (void)
{
	tsk_set_task_state (TASK_MOTOR_RAMPUP, PAUSED);
	tsk_set_task_state (TASK_MOTOR_RAMPDOWN, PAUSED);
}


void
motor_rampup (void)
{
	OCR1A += 1;
	if (OCR1A >= MOTOR_MAX_SPEED)
	{
		tsk_set_task_state (TASK_MOTOR_RAMPUP, PAUSED);
		motor_state = MOTOR_HOLD_BEFORE_RAMPDOWN_STATE;
	}
}


void
motor_rampdown (void)
{
	OCR1A -= 1;
	if (OCR1A <= MOTOR_MIN_SPEED)
	{
		tsk_set_task_state (TASK_MOTOR_RAMPDOWN, PAUSED);
		motor_state = MOTOR_HOLD_BEFORE_RAMPUP_STATE;
	}
}






void
motor_toggle_on_off (void)
{
	tsk_set_task_state (TASK_MOTOR_TOGGLE_ON_OFF, PAUSED);
	if (!motor_is_shutting_down_or_starting_up)
		motor_handle_on_off_toggling ();
}

static void
motor_handle_on_off_toggling (void)
{
	if (motor_is_on)
		motor_initiate_shutdown ();

	else
		motor_initiate_startup ();
}


static void
motor_initiate_startup (void)
{
	motor_is_on = 1;
	motor_is_shutting_down_or_starting_up = 1;
	motor_state = MOTOR_RAMPUP_STATE;
	tsk_set_task_state (TASK_BLINK, RUNNABLE);
	tsk_set_task_state (TASK_MOTOR_STARTUP, RUNNABLE);
}

void
motor_startup (void)
{
	OCR1A += 1;
	if (OCR1A >= MOTOR_MIN_SPEED)
	{
		tsk_set_task_state (TASK_MOTOR_STARTUP, PAUSED);
		motor_is_shutting_down_or_starting_up = 0;
		motor_state = MOTOR_HOLD_BEFORE_RAMPUP_STATE;
	}
}


static void
motor_initiate_shutdown (void)
{
	motor_state = MOTOR_RAMPDOWN_STATE;
	motor_is_shutting_down_or_starting_up = 1;
	tsk_set_task_state (TASK_MOTOR_RAMPUP, PAUSED);
	tsk_set_task_state (TASK_MOTOR_RAMPDOWN, PAUSED);
	tsk_set_task_state (TASK_MOTOR_SHUTDOWN, RUNNABLE);
}

void
motor_shutdown (void)
{
	OCR1A -= 1;
	if (OCR1A == 0)
	{
		tsk_set_task_state (TASK_MOTOR_SHUTDOWN, PAUSED);
		motor_is_shutting_down_or_starting_up = 0;
		motor_is_on = 0;
	}
}
