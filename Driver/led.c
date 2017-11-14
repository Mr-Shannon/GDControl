#include "LED.h"

void led_init(void)
{
  static uint8_t LED_init_flag = 0;

  if(!LED_init_flag)
  { 
    LED_init_flag = 1;
    GPIO_InitPara GPIO_InitStruct;

    RCC_APB2PeriphClock_Enable(LED1_RCC|LED2_RCC, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE);
     
    GPIO_InitStruct.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_SPEED_50MHZ;

    GPIO_InitStruct.GPIO_Pin   = LED1_PIN;
    GPIO_Init(LED1_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = LED2_PIN;
    GPIO_Init(LED2_GPIO, &GPIO_InitStruct);
      
    GPIO_InitStruct.GPIO_Pin   = LED3_PIN;
    GPIO_Init(LED3_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = LEDR_PIN;
    GPIO_Init(LEDR_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = LEDG_PIN;
    GPIO_Init(LEDG_GPIO, &GPIO_InitStruct);
      
    GPIO_InitStruct.GPIO_Pin   = LEDB_PIN;
    GPIO_Init(LEDB_GPIO, &GPIO_InitStruct);
    
    GPIO_PinRemapConfig(GPIO_REMAP_SWJ_JTAGDISABLE, ENABLE);

    GPIO_ResetBits(LEDR_GPIO,LEDR_PIN);
    GPIO_ResetBits(LEDG_GPIO,LEDG_PIN);
    GPIO_ResetBits(LEDB_GPIO,LEDB_PIN);
 
  }
}

void led_on(rt_uint32_t n)
{
  switch (n)
  {
  case 0:
    GPIO_ResetBits(LED1_GPIO, LED1_PIN);
    break;
  case 1:
    GPIO_ResetBits(LED2_GPIO, LED2_PIN);
    break;
  case 2:
    GPIO_ResetBits(LED3_GPIO, LED3_PIN);
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
    GPIO_SetBits(LED1_GPIO, LED1_PIN);
    break;
  case 1:
    GPIO_SetBits(LED2_GPIO, LED2_PIN);
    break;
  case 2:
    GPIO_SetBits(LED3_GPIO, LED3_PIN);
    break;
  default:
    break;
  }
}

