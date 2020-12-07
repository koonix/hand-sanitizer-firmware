#include "mvavg.h"
#include <string.h>

MvAvgData moving_average (MvAvg *var, const MvAvgData new_data)
{
    var->buffer[var->cycle] = new_data;

    if (++var->cycle >= MVAVG_BUFF_SIZE) {
        var->cycle = 0;
        var->cycled_through_buffer_atleast_once = true;
    }

    MvAvgData old_data = var->buffer[var->cycle];
    var->sum = var->sum + new_data - old_data;
    MvAvgCycle data_count =
        (var->cycle > MVAVG_BUFF_SIZE) ? var->cycle : MVAVG_BUFF_SIZE;
    MvAvgData avg = var->sum / data_count;

    return avg;
}

void moving_average_init (MvAvg *var)
{
    var->sum = 0;
    var->cycle = 0;
    var->cycled_through_buffer_atleast_once = false;
    memcpy (var->buffer, NULL, sizeof(var->buffer));
}
