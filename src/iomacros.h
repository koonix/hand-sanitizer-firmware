/**
 * iomacros.h
 * customizable macros for I/O manipulation
 */

#ifndef IOMACROS_H
#define IOMACROS_H

#include <avr/io.h>

// macros:
#define SETBIT(ADDRESS, BIT) (ADDRESS |= (1 << BIT))
#define CLEARBIT(ADDRESS, BIT) (ADDRESS &= ~(1 << BIT))
#define FLIPBIT(ADDRESS, BIT) (ADDRESS ^= (1 << BIT))
#define GETBIT(ADDRESS, BIT) (ADDRESS & (1 << BIT))

/*
    example for "definitions":

    #define BUTTON1_PORT PINB
    #define BUTTON1_PIN PINB4
    #define LED_PORT PORTC
    #define LED_PIN PORTC3
*/
// definitions:
#define BUTTON_PORT PIND
#define BUTTON_PIN PIND3
#define LED_DOWN_PORT PORTD
#define LED_DOWN_PIN PORTD2
#define LED_UP_PORT PORTD
#define LED_UP_PIN PORTD4


/*
    example for "combo definitions":
    #define READ_BUTTON1 GETBIT(BUTTON1_PORT,BUTTON1_PIN)
    #define SET_LED SETBIT(LED_PORT,LED_PIN)
*/
// combo definitions:
#define READ_BUTTON GETBIT (BUTTON_PORT, BUTTON_PIN)
#define LED_DOWN_ON SETBIT (LED_DOWN_PORT, LED_DOWN_PIN)
#define LED_DOWN_OFF CLEARBIT (LED_DOWN_PORT, LED_DOWN_PIN)
#define LED_UP_ON SETBIT (LED_UP_PORT, LED_UP_PIN)
#define LED_UP_OFF CLEARBIT (LED_UP_PORT, LED_UP_PIN)
#define LED_UP_TOGGLE FLIPBIT (LED_UP_PORT, LED_UP_PIN)
#define LED_DOWN_TOGGLE FLIPBIT (LED_DOWN_PORT, LED_DOWN_PIN)

#endif /* IOMACROS_H */
