/*
 * main.c
 * a simple tunable co-operative
 * task scheduler. capable of
 * running all the tasks in one cycle.
 */

#include <avr/io.h>
#define MAIN_C
#include "config.h"

int main (void)
{
    registers_init();
    sei();
    while (1)
        tsk_task_runner();
}
