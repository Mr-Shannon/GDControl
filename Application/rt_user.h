#ifndef __RTT_APP_H__
#define __RTT_APP_H__

#include <rtthread.h>
#include "gd32f10x.h"
#include "led.h"

#define STM32_SRAM_SIZE         20
#define STM32_SRAM_END          (0x20000000 + STM32_SRAM_SIZE * 1024)

void rt_user_os_init(void);        

#endif
