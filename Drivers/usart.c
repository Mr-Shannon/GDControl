#include "usart.h"

void usart_gpio_configure(void)
{
  GPIO_InitPara GPIO_InitStruct;
  
  GPIO_InitStruct.GPIO_Speed = GPIO_SPEED_50MHZ;

  /* Configure USART Rx/Tx PIN */
  GPIO_InitStruct.GPIO_Mode = GPIO_MODE_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = USART_PIN_RX;
  GPIO_Init(USART_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.GPIO_Pin = USART_PIN_TX;
  GPIO_Init(USART_GPIO, &GPIO_InitStruct);
}

rt_err_t usart_configure(struct rt_serial_device *uart, struct usart_config *cfg)
{
  USART_InitPara USART_InitStruct;
  
  USART_InitStruct.USART_BRR = cfg->baud_rate;

  if (cfg->data_bits == DATA_BITS_8){
    USART_InitStruct.USART_WL = USART_WL_8B;
  } else if (cfg->data_bits == DATA_BITS_9) {
    USART_InitStruct.USART_WL = USART_WL_9B;
  }

  if (cfg->stop_bits == STOP_BITS_0_5){
    USART_InitStruct.USART_STBits = USART_STBITS_0_5;
  } else if (cfg->stop_bits == STOP_BITS_1){
    USART_InitStruct.USART_STBits = USART_STBITS_1;
  }else if (cfg->stop_bits == STOP_BITS_1_5){
    USART_InitStruct.USART_STBits = USART_STBITS_1_5;
  }else if (cfg->stop_bits == STOP_BITS_2){
    USART_InitStruct.USART_STBits = USART_STBITS_2;
  }

  if (cfg->parity == PARITY_NONE){
    USART_InitStruct.USART_Parity = USART_PARITY_RESET;
  } else if (cfg->parity == PARITY_ODD) {
    USART_InitStruct.USART_Parity = USART_PARITY_SETODD;
  } else if (cfg->parity == PARITY_EVEN) {
    USART_InitStruct.USART_Parity = USART_PARITY_SETEVEN;
  }
  
  USART_InitStruct.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
  USART_InitStruct.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
  USART_Init(USART1, &USART_InitStruct);
  USART_Enable(USART1,ENABLE);
  
  return RT_EOK;
}

static rt_err_t usart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);

    RT_ASSERT(serial != RT_NULL);

    switch (cmd)
    {
        /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        UART_DISABLE_IRQ(USART1_IRQn);
        /* disable interrupt */
        USART_INT_Set(USART1, USART_INT_RBNE, DISABLE);
        break;
        /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        UART_ENABLE_IRQ(USART1_IRQn);
        /* enable interrupt */
        USART_INT_Set(USART1, USART_INT_TC, ENABLE);
        break;
        /* USART config */
    case RT_DEVICE_CTRL_CONFIG :
        
        break;
    }
    return RT_EOK;
}


int usart_putc(struct rt_serial_device *usart, char c)
{
  USART_DataSend(USART1, c);
  while(!USART_GetBitState(USART1, USART_FLAG_TC));

  return 1;
}

int usart_getc(struct rt_serial_device *usart)
{
  int c;
  if (USART_GetBitState(USART1, USART_FLAG_RBNE))
   c = USART_DataReceive(USART1);
  
  return c;
}

static const struct rt_usart_opts usart_ops =
{
    usart_configure,
    usart_control,
    usart_putc,
    usart_getc,
};

struct rt_serial_device serial;
extern rt_err_t rt_serial_register(struct rt_serial_device *serial,
                               const char              *name,
                               rt_uint32_t              flag,
                               void                    *data);
void rt_usart_init(void)
{
  struct usart_config config = RT_SERIAL_CONFIG_DEFAULT;
  
  int data;
  
  RCC_APB2PeriphClock_Enable(USART_RCC, ENABLE);
  RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_USART1 , ENABLE );
  
  usart_gpio_configure();

  config.baud_rate = BAUD_RATE_115200;

  serial.ops    = &usart_ops;
  serial.config = config;

  /* register UART1 device */
  rt_serial_register(&serial, "uart1", RT_DEVICE_FLAG_RDWR ,&data);                       
}




