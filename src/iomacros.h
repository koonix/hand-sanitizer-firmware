//***********************************************************************************
//
// iomacros.h	(rev 1)
//
// Author:	Jeremy Greenwood
//
// Description:
//      Includes basic macro functions for input and output tasks. Intended for
//      use with AVR microcontrollers compiled with GCC.
//
//      To use this in a project:
//
//      A)  1. Add this file to project   or   B)   1.  Add this file to avr/include
//          2. #include "avriomacros.h"             2.  #include <avriomacros.h>
//
// Example usage to turn an LED on with a pushbutton:
/*

#include <avr/io.h>
#include "iomacros.h"

#define LED     B,3             // declare LED pin and port (always specify pin first)
#define BUTTON  B,2             // declare BUTTON pin and port

int main (void) {

    OUTPUT(LED);                // set LED as an output
    INPUT(BUTTON);              // set BUTTON as an input
    PULLUP(BUTTON);             // activate internal pullup resistor for BUTTON

    while(1) {                  // forever loop

        if(GET(BUTTON) == 0)    // if button is pressed...
            ON(LED);            //     turn on LED
        else                    // otherwise
            OFF(LED);           //     turn off LED
    }
}

*/
//***********************************************************************************
#ifndef IOMACROS_H
#define IOMACROS_H

#define SETBIT(address, bit)   (address |=  (1 << bit))
#define CLEARBIT(address, bit) (address &= ~(1 << bit))
#define FLIPBIT(address, bit)  (address ^=  (1 << bit))
#define GETBIT(address, bit)   (address &   (1 << bit))

/* Macro function to set a pin as an output. */
#define   OUTPUT(X)          _OUTPUT(X)
#define  _OUTPUT(port,pin)   SETBIT(DDR##port, DDR##port##pin)

/* Macro function to set a pin as an input. */
#define   INPUT(x)           _INPUT(x)
#define  _INPUT(port,pin)    CLEARBIT(DDR##port, DDR##port##pin)

/* Macro function to set an output pin high. */
#define   ON(x)              _ON(x)
#define  _ON(port,pin)       SETBIT(PORT##port, PORT##port##pin)

/* Macro function to set an output pin low. */
#define   OFF(x)             _OFF(x)
#define  _OFF(port,pin)      CLEARBIT(PORT##port, PORT##port##pin)

/* Macro function to toggle an output pin */
#define   FLIP(x)            _FLIP(x)
#define  _FLIP(port,pin)     FLIPBIT(PORT##port, PORT##port##pin)

/* Macro function to set internal pullup resistor of input pin (same as "on" macro) */
#define   PULLUP(x)          _ON(x)
#define   NOPULLUP(x)        _OFF(x)

/* Macro function to get state of input pin */
#define   GET(x)             _GET(x)
#define  _GET(port,pin)      GETBIT(PIN##port, PIN##port##pin)

#endif /* IOMACROS_H */
