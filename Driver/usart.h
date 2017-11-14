#ifndef __USART_H__
#define __USART_H__

#include <rtthread.h>
#include "gd32f10x.h"
#include "serial.h"

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_0_5                   0
#define STOP_BITS_1                     1
#define STOP_BITS_1_5                   2
#define STOP_BITS_2                     3

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#define BIT_ORDER_LSB                   0
#define BIT_ORDER_MSB                   1

#define NRZ_NORMAL                      0       /* Non Return to Zero : normal mode */
#define NRZ_INVERTED                    1       /* Non Return to Zero : inverted mode */

#define RT_SERIAL_RB_BUFSZ              64

#define USART_RCC                       RCC_APB2PERIPH_GPIOA
#define USART_GPIO                      GPIOA
#define USART_PIN_TX                    GPIO_PIN_9
#define USART_PIN_RX                    GPIO_PIN_10
 
#define UART_ENABLE_IRQ(n)              NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)             NVIC_DisableIRQ((n))

#define RT_SERIAL_CONFIG_DEFAULT           \
{                                          \
    BAUD_RATE_115200, /* 115200 bits/s */  \
    DATA_BITS_8,      /* 8 databits */     \
    STOP_BITS_1,      /* 1 stopbit */      \
    PARITY_NONE,      /* No parity  */     \
    BIT_ORDER_LSB,    /* LSB first sent */ \
    NRZ_NORMAL,       /* Normal mode */    \
    RT_SERIAL_RB_BUFSZ, /* Buffer size */  \
    0                                      \
}

#endif

