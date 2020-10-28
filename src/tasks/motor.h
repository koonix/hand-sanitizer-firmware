#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define MOTOR_MAX_SPEED 99
#define MOTOR_MIN_SPEED 20

#define MOTOR_HOLD_BEFORE_RAMPUP_STATE 0
#define MOTOR_HOLD_BEFORE_RAMPDOWN_STATE 2
#define MOTOR_RAMPUP_STATE 1
#define MOTOR_RAMPDOWN_STATE 3

extern _Bool motor_is_on;
extern uint8_t motor_state;

void
motor_toggle_speed_control (void);

void
motor_toggle_on_off (void);

void
motor_startup (void);

void
motor_shutdown (void);

void
motor_rampup (void);

void
motor_rampdown (void);

#endif /* MOTOR_H */
