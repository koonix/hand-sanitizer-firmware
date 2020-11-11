
/*
 * file:     tasker.h
 *
 * author:   Ferenc Nemeth,
 *           Refactored and improved by koonix (https://github.com/soystemd)
 *
 * date:     21 Jul 2018
 *
 * brief: This is a really simple, non-preemptive (co-operative) task scheduler.
 * You can register tasks with their runnable function and the periodic
 * time you want to call them. With a help of a timer the tasks get into READY
 * state after every time period (except if they are PAUSED) and they get
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

/* Function pointer for registering tasks. */
typedef void (*TaskFunctionPtr)(void);
/* task counter type */
typedef uint16_t TaskTime;

/* Possible states for tasks. */
typedef enum {
  /* In the RUNNABLE state the task waits for the timer to put it into READY state. */
  RUNNABLE = 0,
  /* In the READY state the task is ready to be called and executed in the main function. */
  READY,
  /* In the PAUSED state the task is ignored by the timer and executer. */
  PAUSED
} TaskState;

/* Variables of the tasks. */
typedef struct {
  TaskFunctionPtr run; /* This is the task that gets called periodically. */
  TaskState state;     /* The current state of the task. */
  TaskTime period;     /* The period we want to call it. */
  TaskTime
      counter; /* Counter, if it reaches the period, then the timer puts it into READY state. */
} Task;

void tsk_task_create(TaskFunctionPtr function, TaskState state, TaskTime period);
void tsk_task_time_manager(void);
void tsk_task_runner(void);
TaskState tsk_get_task_state(TaskFunctionPtr task);
TaskTime tsk_get_task_period(TaskFunctionPtr task);
TaskTime tsk_get_task_counter(TaskFunctionPtr task);
void tsk_set_task_state(TaskFunctionPtr task, TaskState new_state);
void tsk_set_task_period(TaskFunctionPtr task, TaskTime new_period);
void tsk_set_task_counter(TaskFunctionPtr task, TaskTime new_counter);

#endif /* TASKER_H */
