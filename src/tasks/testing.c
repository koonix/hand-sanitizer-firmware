#include <string.h>
/* #include <stdio.h> */
#include "testing.h"
#include "config.h"
#include "spi.h"

static void adc_select(uint8_t channel);
static uint8_t admux_nomux = 0 | (0 << REFS1) | (1 << REFS0) | (0 << ADLAR);

void adc_sample_test(void)
{
    task_set_state(adc_sample, RUNNABLE);
}

void adc_sample(void)
{
    static uint8_t cycle = 0;
    /*static uint16_t adc_result[2];*/
    char string[SPI_BUFFSIZE] = {0};


    if (ADC_CONVERSION_IN_PROGRESS)
        return;
    cycle++;
    switch (cycle) {
        case 1:
            adc_select(ADC_AMBIENT);
            break;
        case 2:
        case 4:
            ADC_START_CONVERSION;
            break;
        case 3:
            /*adc_result[0] = ADC;*/
            adc_select(ADC_PROXIMA);
            break;
        case 5:
            /* adc_result[1] = ADC; */
            /*snprintf(string, SPI_BUFFSIZE,
                    "Amb: %4u, Pxm: %4u", adc_result[0], adc_result[1]);*/
            spi_send(string);
            cycle = 0;
            break;
        default:
            cycle = 0;
            break;
    }
}

static void adc_select(uint8_t channel)
{
    ADMUX = channel;
    ADMUX |= admux_nomux;
}
