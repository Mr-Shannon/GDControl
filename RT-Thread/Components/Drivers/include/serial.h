#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "rtthread.h"
#include "usart.h"

struct usart_config
{
    rt_uint32_t baud_rate;

    rt_uint32_t data_bits               :4;
    rt_uint32_t stop_bits               :2;
    rt_uint32_t parity                  :2;
    rt_uint32_t bit_order               :1;
    rt_uint32_t invert                  :1;
    rt_uint32_t bufsz                   :16;
    rt_uint32_t reserved                :4;
};

struct rt_serial_device
{
  struct rt_device             parent; 
  
  const struct rt_usart_opts   *ops;
  struct usart_config          config;
  
  void *serial_rx;
  void *serial_tx;
};
 
struct rt_usart_opts
{
  rt_err_t (*configure)(struct rt_serial_device *usart, struct usart_config *cfg);
  rt_err_t (*control)(struct rt_serial_device *usart, int cmd, void *arg);
  
  int (*putc)(struct rt_serial_device *usart, char c);
  int (*getc)(struct rt_serial_device *usart);
};

#endif
