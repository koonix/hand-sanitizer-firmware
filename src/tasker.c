/*
 * file:     tasker.c
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

#include "tasker.h"

// Static function prototypes:
static void
check_task_counter_and_handle_state (uint8_t task_index);


/* variables and information for every single task. */
static Task task_array[] = { NULL };

/* index of the task that is currently being processed. */
static uint8_t numberof_tasks = 0;


/*
 * This function registers the tasks.
 * At the beginning there is an error check for registration.
 * If everything is good, the input values are saved.
 */
void
tsk_task_create (task_function_ptr function, task_state state, task_counter_t run_period)
{
	/* return_enum exit = ERR_UNKNOWN; */

	/* /1* Null pointer. *1/ */
	/* if (task_array == NULL) */
	/* 	exit = ERR_NULL_PTR; */

	/* /1* Time limit. *1/ */
	/* else if ((run_period < TASK_MIN_PERIOD) || (run_period > TASK_MAX_PERIOD)) */
	/* 	exit = ERR_TIME_LIMIT; */

	/* /1* Task number limit. *1/ */
	/* else if (numberof_tasks >= MAX_TASK_COUNT) */
	/* 	exit = ERR_COUNT_LIMIT; */

	/* /1* Everything is fine, save. *1/ */
	/* else */
	/* { */
    task_array[numberof_tasks].run = function;
	task_array[numberof_tasks].run_period = run_period;
	task_array[numberof_tasks].state = state;
	task_array[numberof_tasks].counter = 1;
	numberof_tasks++;
	/* exit = OK; */
	/* } */

	/* return exit; */
}


void
tsk_init(void)
{
    uint8_t task_count = sizeof(task_array)/sizeof(task_array[0]);
	for (uint8_t task_index = 0; task_index < task_count; task_index++)
    {}
}

uint8_t
find_task_index(task_function_ptr fn)
{
    uint8_t task_index=0;
	for (; task_index < numberof_tasks; task_index++)
        if(task_array[task_index].run == fn)
            break;
    return task_index;
}


/*
 * This function keeps track of the tasks' time and puts them into
 * READY state. This function shall be called in a timer interrupt.
 */
void
tsk_task_time_manager (void)
{
	for (uint8_t task_index = 0; task_index < numberof_tasks; task_index++)
		if (task_array[task_index].state != SUSPENDED)
			check_task_counter_and_handle_state (task_index);
}

/*
 * This is a backend method used in
 * the function "tsk_task_time_manager".
 */
static void
check_task_counter_and_handle_state (uint8_t task_index)
{
	/* Put it into READY state. */
	if (task_array[task_index].counter >= task_array[task_index].run_period)
	{
		task_array[task_index].counter = 1;
		task_array[task_index].state = READY;
	}

	/* Or increment task's counter. */
	else
	{
		task_array[task_index].counter++;
	}
}

/*
 * This function calls the READY tasks and then puts them back into
 * RUNNABLE state. This function SHALL be called in the infinite loop.
 */
void
tsk_task_runner (void)
{
	for (uint8_t task_index = 0; task_index < numberof_tasks; task_index++)
	{
		/* If it is ready, call it.*/
		if (task_array[task_index].state == READY)
		{
			task_array[task_index].state = RUNNABLE;
			task_array[task_index].run ();
		}
	}
}


/*
 * Returns the state of the task.
 */
task_state
tsk_get_task_state (uint8_t task_index)
{
	return task_array[task_index].state;
}



/*
 * Returns the burst time of the task.
 */
task_counter_t
tsk_get_task_period (uint8_t task_index)
{
	return task_array[task_index].run_period;
}



/*
 * Returns the current counter value of the task.
 */
task_counter_t
tsk_get_task_counter (uint8_t task_index)
{
	return task_array[task_index].counter;
}



/*
 * Manually changes the task's state.
 */
void
tsk_set_task_state (uint8_t task_index, task_state new_state)
{
	task_array[task_index].state = new_state;
	if (new_state == SUSPENDED)
		task_array[task_index].counter = 1;
}



/*
 * Manually changes the task's burst time.
 */
void
tsk_set_task_period (uint8_t task_index, task_counter_t new_period)
{
	task_array[task_index].run_period = new_period;
}



/*
 * Manually changes the task's counter.
 */
void
tsk_set_task_counter (uint8_t task_index, task_counter_t new_counter)
{
	task_array[task_index].counter = new_counter;
}
