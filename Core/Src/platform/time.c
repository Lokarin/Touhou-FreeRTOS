#include "time.h"
#include "stm32f4xx_hal.h"

uint32_t time_get_ms(void)
{
    return HAL_GetTick();
}
