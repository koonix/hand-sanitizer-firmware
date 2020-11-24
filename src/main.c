/*
 * main.c
 * a simple tunable co-operative
 * task scheduler. capable of
 * running all the tasks in one cycle.
 */

#include <avr/io.h>
/* #include <util/delay.h> */
#define MAIN_C
#include "config.h"

int main (void)
{
    registers_init();
/*    FLIP(RELAY);
    _delay_ms(100);
    FLIP(RELAY);
    _delay_ms(100); */
    sei();
    while (1)
        task_runner();
}
