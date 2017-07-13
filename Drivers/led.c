#include "led.h"

void led_init(void)
{
  static uint8_t led_init_flag = 0;

  if(!led_init_flag)
  { 
    led_init_flag = 1;
    GPIO_InitPara GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(led1_rcc|led2_rcc, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE);
     
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;

    GPIO_InitStructure.GPIO_Pin   = led1_pin;
    GPIO_Init(led1_gpio, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = led2_pin;
    GPIO_Init(led2_gpio, &GPIO_InitStructure);
      
    GPIO_InitStructure.GPIO_Pin   = led3_pin;
    GPIO_Init(led3_gpio, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = ledr_pin;
    GPIO_Init(ledr_gpio, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = ledg_pin;
    GPIO_Init(ledg_gpio, &GPIO_InitStructure);
      
    GPIO_InitStructure.GPIO_Pin   = ledb_pin;
    GPIO_Init(ledb_gpio, &GPIO_InitStructure);
    
    GPIO_PinRemapConfig(GPIO_REMAP_SWJ_JTAGDISABLE, ENABLE);

    GPIO_ResetBits(ledr_gpio,ledr_pin);
    GPIO_ResetBits(ledg_gpio,ledg_pin);
    GPIO_ResetBits(ledb_gpio,ledb_pin);
 
  }
}

void led_on(rt_uint32_t n)
{
  switch (n)
  {
  case 0:
    GPIO_ResetBits(led1_gpio, led1_pin);
    break;
  case 1:
    GPIO_ResetBits(led2_gpio, led2_pin);
    break;
  case 2:
    GPIO_ResetBits(led3_gpio, led3_pin);
    break;
  default:
    break;
  }
}

void led_off(rt_uint32_t n)
{
  switch (n)
  {
  case 0:
    GPIO_SetBits(led1_gpio, led1_pin);
    break;
  case 1:
    GPIO_SetBits(led2_gpio, led2_pin);
    break;
  case 2:
    GPIO_SetBits(led3_gpio, led3_pin);
    break;
  default:
    break;
  }
}

