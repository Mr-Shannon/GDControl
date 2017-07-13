#include <rtthread.h>
#include "rtconfig.h"
#include "gd32f10x.h"
#include "rt_user.h"
#include "led.h"

extern rt_base_t rt_hw_interrupt_disable();

int main(void)
{
  rt_hw_interrupt_disable();
  rt_user_os_init();
	return 0;
}
