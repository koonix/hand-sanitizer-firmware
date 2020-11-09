#ifndef TASKMACROS_H
#define TASKMACROS_H

#include "blink.h"
#include "button.h"
#include "motor.h"

#define MKTASK(fn, stat, period)

#define TASK_DEBOUNCE 0
#define TASK_BUTTON_EVENT_HANDLER 1

#define TASK_MOTOR_STARTUP 2
#define TASK_MOTOR_SHUTDOWN 3
#define TASK_MOTOR_RAMPUP 4
#define TASK_MOTOR_RAMPDOWN 5

#define TASK_BLINK 6
#define TASK_BLINK_UPPER 7
#define TASK_BLINK_UPPER_SECONDARY 8
#define TASK_BLINK_LOWER 9
#define TASK_BLINK_LOWER_SECONDARY 10

#define TASK_MOTOR_TOGGLE_SPEED_CONTROL 11
#define TASK_MOTOR_TOGGLE_ON_OFF 12

#endif /* TASKMACROS_H */
