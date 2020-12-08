#include <string.h>
#include <stdlib.h>
#include "testing.h"
#include "config.h"
#include "spi.h"
#include "mvavg.h"

/* Config */
#define PROXIMITY_SENSORVALUE_DIFF_THRESHOLD   (uint16_t)35
#define HAND_HOLD_ACTIVATION_TIME_100MSEC      (uint8_t )4
#define AVERAGING_SAMPLE_PERIOD_100MSEC        (uint8_t )30
#define AVG_ROLLING_SIZE                       (uint8_t )36
#define FIRSTTIME_QUICKSAMPLE_DURATION_100MSEC (uint8_t )4
#define MINIMUM_RELIABLE_SENSOR_VALUE          (uint16_t)100
#define AVG_DISCARD_DIFF_THRESHOLD             (uint16_t)150

static void check_sanitize(uint16_t adc);
/* static void adc_select(uint8_t channel); */

static uint16_t average = 0;
/* static uint8_t admux_nomux = 0 | (0 << REFS1) | (1 << REFS0) | (0 << ADLAR); */

MvAvg prox_avg;

void adc_sample_runner(void) {
    task_set_state(adc_sample, RUNNABLE);
}
void adc_sample(void)
{
    static uint8_t cycle                 = 0;
    static uint8_t avg_cycle_msec        = 0;
    static uint8_t first_time_avg        = 1;
    static uint8_t first_time_init_mvavg = 1;
    static char string[SPI_BUFFSIZE];

    if (first_time_init_mvavg) {
        first_time_init_mvavg = 0;
        moving_average_init(&prox_avg);
    }

    if (ADC_CONVERSION_IN_PROGRESS)
        return;

    cycle++;
    if (cycle == 1)
        ADC_START_CONVERSION;

    else {
        cycle = 0;
        task_set_state(adc_sample, PAUSED);
        avg_cycle_msec++;

        if (first_time_avg) {
            if (avg_cycle_msec >= FIRSTTIME_QUICKSAMPLE_DURATION_100MSEC)
                first_time_avg = 0;
            average = moving_average(&prox_avg, ADC);
        }
        else if (avg_cycle_msec >= AVERAGING_SAMPLE_PERIOD_100MSEC) {
            avg_cycle_msec = 0;
            average = moving_average(&prox_avg, ADC);
        }

        char str_utoa[6],str_utoa2[6];
        /* formatting the spi string: */
        mstpcpy(mstpcpy(mstpcpy(mstpcpy(mstpcpy(string,
            "Pxm: "  ), utoa(ADC, str_utoa, 10)),
            ", Avg: "), utoa(average, str_utoa2, 10)), "\n\r");
        spi_send(string);

        if (!first_time_avg)
            check_sanitize(ADC);
    }
}


static void check_sanitize(uint16_t adc)
{
    static uint8_t hand_detected_cycles = 0;

    /* if hand is far */
    if(adc > average - PROXIMITY_SENSORVALUE_DIFF_THRESHOLD ||
       adc < (uint16_t)MINIMUM_RELIABLE_SENSOR_VALUE) {
        hand_detected_cycles = 0;
        return;
    }
    /* if hand has been close for just long enough */
    else if (hand_detected_cycles == (uint8_t)HAND_HOLD_ACTIVATION_TIME_100MSEC) {
        hand_detected_cycles++;
        sanitize();
        return;
    }
    else if (hand_detected_cycles > (uint8_t)HAND_HOLD_ACTIVATION_TIME_100MSEC)
        return;

    hand_detected_cycles++;
}


void sanitize(void)
{
    static uint8_t cycle = 0;

    if(cycle == 0)
    {
        task_set_state(sanitize, RUNNABLE);
        task_set_state(adc_sample_runner, PAUSED);
    }

    FLIP(RELAY);
    cycle++;

    if(cycle < 6)
        return;

    OFF(RELAY);
    cycle = 0;
    task_set_state(sanitize, PAUSED);
    task_set_state(adc_sample_runner, RUNNABLE);
}


/* static void adc_select(uint8_t channel) */
/* { */
/*     ADMUX = channel; */
/*     ADMUX |= admux_nomux; */
/* } */

/* static uint16_t diff(uint16_t v1, uint16_t v2) { */
/*     return v1>v2 ? v1-v2 : v2-v1; */
/* } */
