#include "rt_user.h"

extern int Image$$RW_IRAM1$$ZI$$Limit;
ALIGN(4)
static rt_uint8_t led1_stack[128];
static struct rt_thread led1_thread;
static void led1_thread_entry(void* parameter)
{
  led_init();

  while (1)
  {
    /* led1 on */
    led_on(0);
    rt_thread_delay( RT_TICK_PER_SECOND/2 ); /* sleep 0.5 second and switch to other thread */

    /* led1 off */
    led_off(0);
    rt_thread_delay( RT_TICK_PER_SECOND/2 );
  }
}

ALIGN(4)
static rt_uint8_t led2_stack[128];
static struct rt_thread led2_thread;
static void led2_thread_entry(void* parameter)
{
  led_init();

  while (1)
  {
    /* led1 on */
    led_on(1);
    rt_thread_delay( RT_TICK_PER_SECOND/4 ); /* sleep 0.25 second and switch to other thread */

    /* led1 off */
    led_off(1);
    rt_thread_delay( RT_TICK_PER_SECOND/4 );
  }
}

static rt_thread_t led3_thread = RT_NULL;
static void led3_thread_entry(void* parameter)
{
  led_init();

  while (1)
  {
    /* led1 on */
    led_on(2);
    rt_thread_delay( RT_TICK_PER_SECOND); /* sleep 1 second and switch to other thread */

    /* led1 off */
    led_off(2);
    rt_thread_delay( RT_TICK_PER_SECOND);
  }
}

/**
* This is RT-Thread  application initialize.
*
*/
int rt_user_application_init(void)
{
  rt_err_t result;

  /* init led1 thread */
  result = rt_thread_init(&led1_thread, "led1", led1_thread_entry,  RT_NULL,
                         (rt_uint8_t*)&led1_stack[0],sizeof(led1_stack), 20, 5);
  if (result == RT_EOK)
    rt_thread_startup(&led1_thread);
  else 
    rt_thread_detach (&led1_thread);

  /* init led2 thread */
  result = rt_thread_init(&led2_thread, "led2", led2_thread_entry,RT_NULL,
                         (rt_uint8_t*)&led2_stack[0], sizeof(led2_stack), 10, 5);
  if (result == RT_EOK)
    rt_thread_startup(&led2_thread);  
  else 
    rt_thread_detach (&led2_thread);

  /* init led3 thread */
  led3_thread = rt_thread_create("led3", led3_thread_entry,(void*)3, 256, 25, 5); 
                
  if(led3_thread != RT_NULL)
    rt_thread_startup(led3_thread);
  else 
    rt_thread_delete(led3_thread);

  return 0;
}
/**
* This is system clock configuration.
*
*/
void SystemClock_Config(void)
{
  RCC_DeInit();
  RCC_HSI_Enable(ENABLE);
  while(RCC_GetBitState(RCC_FLAG_HSISTB) == RESET);
  
  RCC_PLLConfig(RCC_PLLSOURCE_HSI_DIV2, RCC_PLLMUL_27);//108MHz
	RCC_PLL_Enable(ENABLE); //使能锁相环PLL 
	while(RCC_GetBitState(RCC_FLAG_PLLSTB) == RESET);
  
  RCC_CK_SYSConfig(RCC_SYSCLKSOURCE_PLLCLK);
	while(RCC_GetCK_SYSSource() != 0x08);

	RCC_AHBConfig(RCC_SYSCLK_DIV1);  //108MHz
	RCC_APB1Config(RCC_SYSCLK_DIV2); //54MHz
	RCC_APB2Config(RCC_SYSCLK_DIV1); // 108MHz
}

/**
* This is RT-Thread initialize.
*
*/
void rt_user_os_init(void)
{
  SystemClock_Config();
  
  /*开滴答定时器*/
  SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );

  /*初始化堆空间*/
  rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);

  /* init scheduler system */
  rt_system_scheduler_init();

  /* initialize timer */
  rt_system_timer_init();

  /* init timer thread */
  rt_system_timer_thread_init();

  /* init application */
  rt_user_application_init();

  /* init idle thread */
  rt_thread_idle_init();

  /* start scheduler */
  rt_system_scheduler_start();
}

/**
* This is the timer interrupt service routine. 
*
*/
void SysTick_Handler(void)
{
  /* enter interrupt */
  rt_interrupt_enter();

  rt_tick_increase();

  /* leave interrupt */
  rt_interrupt_leave();
}
