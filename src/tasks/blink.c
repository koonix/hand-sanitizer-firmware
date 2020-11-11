#include "tasker.h"
#include "motor.h"
#include "blink.h"
#include "config.h"

// Prototypes:
static void blink_handler(void);

static uint16_t speed2period(uint16_t speed);

void blink(void)
{
    if (motor_is_on)
        blink_handler();
    else {
        tsk_set_task_state(blink_upper, PAUSED);
        tsk_set_task_state(blink_lower, PAUSED);
    }
}

static void blink_handler(void)
{
    tsk_set_task_period(blink_upper, speed2period(OCR1A));
    tsk_set_task_period(blink_lower, speed2period(OCR1A));

    switch (motor_state) {
    case MOTOR_RAMPUP_STATE:
    case MOTOR_HOLD_BEFORE_RAMPUP_STATE:
        tsk_set_task_state(blink_upper, RUNNABLE);
        tsk_set_task_state(blink_lower, PAUSED);
        break;
    default:
        tsk_set_task_state(blink_lower, RUNNABLE);
        tsk_set_task_state(blink_upper, PAUSED);
        break;
    }
}

void blink_upper(void) {
    tsk_set_task_state(blink_upper_secondary, RUNNABLE);
}

void blink_lower(void)
{
    ON(LED_UP);
    tsk_set_task_state(blink_lower_secondary, RUNNABLE);
}

void blink_upper_secondary(void)
{
    OFF(LED_UP);
    tsk_set_task_state(blink_upper_secondary, PAUSED);
}

void blink_lower_secondary(void)
{
    OFF(LED_DOWN);
    tsk_set_task_state(blink_lower_secondary, PAUSED);
}

static uint16_t speed2period(uint16_t speed)
{
    return (((MOTOR_MAX_SPEED - speed) * 3) + 55) / 4;
}
