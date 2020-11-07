/*
 * file:     tasker.h
 * author:   Ferenc Nemeth, Refactored by koonix (https://github.com/soystemd)
 * date:     21 Jul 2018
 * brief:
 *
 * This is a really simple, non-preemptive task scheduler.
 * You can register tasks with their runnable function and the periodic
 * time you want to call them. With a help of a timer the tasks get into READY
 * state after every time period (except if they are SUSPENDED) and they get
 * called and executed in the main()'s inifinte loop. After they are finished
 * everything starts over. This Scheduler helps you to keep your tasks and
 * timing organized.
 *
 * Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth/
 *
 */

#ifndef TASKER_H
#define TASKER_H

#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>


/** Maximal task number that can be registered. */
#define MAX_TASK_COUNT ((uint8_t)20u)

/** Maximal time that a task can run (TASK_MAX_PERIOD*time_ticks). */
#define TASK_MAX_PERIOD ((uint16_t)10000u)

/** Minimal time that a task can run (TASK_MIN_PERIOD*time_ticks). */
#define TASK_MIN_PERIOD ((uint8_t)1u)

/**< Function pointer for registering tasks. */
typedef void (*task_function_ptr) (void);


/**
 * States of the tasks.
 */
typedef enum
{
	RUNNABLE = 0, /* In the RUNNABLE state the task waits for the timer to put
	                               it into READY state. */
	READY, /* In the READY state the task is ready to be called and executed
	          in the main function. */
	SUSPENDED /* In the SUSPENDED state the task is ignored by the timer and
	             executer. */
} task_state;

typedef uint16_t task_counter_t;
/**
 * Variables of the tasks.
 */
typedef struct
{
	task_function_ptr run;     /** This is the task that gets called periodically. */
	task_state state; /**< The current state of the task. */
	task_counter_t run_period; /** The period we want to call it. */
	task_counter_t counter;    /** Counter, if it reaches the period, then the
	                        timer puts it into READY state. */
} Task;


/**
 * Feedback and error handling for the task creation.
 */
typedef enum
{
	OK = 0, /* OK:    Everything went as expected. */
	ERR_NULL_PTR, /* ERROR: Null pointer as a task. */
	ERR_TIME_LIMIT, /* ERROR: Task period out of range */
	ERR_COUNT_LIMIT, /* ERROR: Number of registered tasks exceded maximum */
	ERR_UNKNOWN /* ERROR: Something horrible happened, time to panic! */
} return_enum;


/**
 * Functions.
 */
return_enum
tsk_task_create (task_function_ptr function,
                 task_counter_t run_period,
                 task_state state);

void
tsk_task_time_manager (void);

void
tsk_task_runner (void);

task_state
tsk_get_task_state (uint8_t task_number);

task_counter_t
tsk_get_task_period (uint8_t task_number);

task_counter_t
tsk_get_task_counter (uint8_t task_number);

void
tsk_set_task_state (uint8_t task_number, task_state new_state);

void
tsk_set_task_period (uint8_t task_number, task_counter_t new_period);

void
tsk_set_task_counter (uint8_t task_number, task_counter_t new_counter);

#endif /* os_H */
