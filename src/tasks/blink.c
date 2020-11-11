#include "tasker.h"
#include "taskmacros.h"
#include "iomacros.h"
#include "motor.h"
#include "blink.h"

// Prototypes:
static void
blink_handler (void);

static uint16_t
speed2period (uint16_t speed);


void
blink (void)
{
	if (motor_is_on)
		blink_handler ();
	else
	{
		tsk_set_task_state (TASK_BLINK_UPPER, PAUSED);
		tsk_set_task_state (TASK_BLINK_LOWER, PAUSED);
	}
}


static void
blink_handler (void)
{
	tsk_set_task_period (TASK_BLINK_UPPER, speed2period (OCR1A));
	tsk_set_task_period (TASK_BLINK_LOWER, speed2period (OCR1A));

	switch (motor_state)
	{
	case MOTOR_RAMPUP_STATE:
	case MOTOR_HOLD_BEFORE_RAMPUP_STATE:
		tsk_set_task_state (TASK_BLINK_UPPER, RUNNABLE);
		tsk_set_task_state (TASK_BLINK_LOWER, PAUSED);
		break;
	default:
		tsk_set_task_state (TASK_BLINK_LOWER, RUNNABLE);
		tsk_set_task_state (TASK_BLINK_UPPER, PAUSED);
		break;
	}
}

void
blink_upper (void)
{
	LED_UP_ON;
	tsk_set_task_state (TASK_BLINK_UPPER_SECONDARY, RUNNABLE);
}

void
blink_lower (void)
{
	LED_DOWN_ON;
	tsk_set_task_state (TASK_BLINK_LOWER_SECONDARY, RUNNABLE);
}

void
blink_upper_secondary (void)
{
	LED_UP_OFF;
	tsk_set_task_state (TASK_BLINK_UPPER_SECONDARY, PAUSED);
}

void
blink_lower_secondary (void)
{
	LED_DOWN_OFF;
	tsk_set_task_state (TASK_BLINK_LOWER_SECONDARY, PAUSED);
}


static uint16_t
speed2period (uint16_t speed)
{
	return (((MOTOR_MAX_SPEED - speed) * 3) + 55) / 4;
}
