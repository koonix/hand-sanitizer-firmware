#ifndef MVAVG_H
#define MVAVG_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#ifndef MVAVG_BUFF_SIZE
  #warning "MVAVG_BUFF_SIZE not defined for <mvavg.h>; defaults to 10."
  #define MVAVG_BUFF_SIZE 10
#endif

#ifndef MVAVG_DATA_TYPE
  #warning "MVAVG_DATA_TYPE not defined for <mvavg.h>; defaults to uint16_t."
  #define MVAVG_DATA_TYPE uint16_t
#endif

#ifndef MVAVG_SUM_TYPE
  #warning "MVAVG_SUM_TYPE not defined for <mvavg.h>; defaults to uint32_t."
  #define MVAVG_SUM_TYPE uint32_t
#endif

#ifndef MVAVG_CYCLE_TYPE
  #if MVAVG_BUFF_SIZE <= UCHAR_MAX
    #define MVAVG_CYCLE_TYPE uint8_t
  #elif MVAVG_BUFF_SIZE <= UINT_MAX
    #define MVAVG_CYCLE_TYPE uint16_t
  #else
    #warning MVAVG_BUFF_SIZE has unknown range; MVAVG_CYCLE_TYPE set to uint32_t."
  #endif
#endif

#ifndef MVAVG_SUM_TYPE
# warning "MVAVG_SUM_TYPE not defined for <mvavg.h>; defaults to uint32_t"
#define MVAVG_SUM_TYPE uint32_t
#endif

typedef MVAVG_DATA_TYPE  MvAvgData;
typedef MVAVG_SUM_TYPE   MvAvgSum;
typedef MVAVG_CYCLE_TYPE MvAvgCycle;

typedef struct {
    MvAvgData  buffer[MVAVG_BUFF_SIZE];
    MvAvgSum   sum;
    MvAvgCycle cycle;
    bool       cycled_through_buffer_atleast_once;
} MvAvg;

void moving_average_init (MvAvg *var);
MvAvgData moving_average (MvAvg *var, const MvAvgData new_data);

#endif /* MVAVG_H */
