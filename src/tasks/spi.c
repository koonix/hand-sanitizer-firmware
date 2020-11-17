#include <stdint.h>
#include <avr/io.h>
#include "iomacros.h"
#include "spi.h"

#define IS_SPI_BUSY (GETBIT(SPSR,SPIF))

uint8_t spi(uint8_t data)
{
    SPDR = data;
    return SPDR;
}
