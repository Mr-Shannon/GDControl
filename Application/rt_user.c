#include "rt_user.h"

extern int Image$$RW_IRAM1$$ZI$$Limit;
ALIGN(4)
static rt_uint8_t led1_stack[512];
static struct rt_thread led1_thread;
static void led1_thread_entry(void* parameter)
{
  rt_device_t device;
  uint8_t open_flag = 0;
  uint8_t uart_rx_buffer[12] = {'A','B','1','2','3','4','s','d','6','8','8','q'};
  
  //打开USART1设备
  device = rt_device_find("uart1");
  if (device != RT_NULL){
    if (rt_device_open(device, RT_DEVICE_OFLAG_RDWR) == RT_EOK)
      open_flag = 1;  
  } 
  
  led_init();
  
  while (1)
  {
    /* led1 on */
    led_on(0);
    if (open_flag)rt_device_write(device, 0, &uart_rx_buffer[0],12);
    rt_kprintf("Hello World!");
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
	while(RCC_GetBitState(RCC_FLAG_PLLSTB) == RESET){};
  
  RCC_CK_SYSConfig(RCC_SYSCLKSOURCE_PLLCLK);
	while(RCC_GetCK_SYSSource() != 0x08);

	RCC_AHBConfig(RCC_SYSCLK_DIV1);  //108MHz
	RCC_APB1Config(RCC_SYSCLK_DIV2); //54MHz
	RCC_APB2Config(RCC_SYSCLK_DIV1); // 108MHz
}

void usart_config(void)
{
  /* Enable GPIOA clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA , ENABLE );
    
    /* Enable USART1 APB clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_USART1 , ENABLE );
    
    /* USART1 Pins configuration **************************************************/
    GPIO_DeInit( GPIOA );
    {
        /* Configure the GPIO ports */
        GPIO_InitPara GPIO_InitStructure;
       
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_9 ;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_SPEED_50MHZ;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_10;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_IN_FLOATING;;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
    }

    { 
        USART_InitPara USART_InitStructure;
        
        USART_DeInit( USART1 );
        USART_InitStructure.USART_BRR           = 115200;
        USART_InitStructure.USART_WL            = USART_WL_8B;
        USART_InitStructure.USART_STBits            = USART_STBITS_1;
        USART_InitStructure.USART_Parity                = USART_PARITY_RESET;
        USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
        USART_InitStructure.USART_RxorTx                = USART_RXORTX_RX | USART_RXORTX_TX;
        USART_Init(USART1, &USART_InitStructure);
    }

    /* USART enable */
    USART_Enable(USART1, ENABLE);
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
  
  usart_config();
  
  extern void rt_usart_init(void);
  rt_usart_init();
  
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
