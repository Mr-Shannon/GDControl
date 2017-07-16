#include "serial.h"


rt_inline int _serial_poll_rx(struct rt_serial_device *serial, rt_uint8_t *data, int length)
{
    int ch;
    int size;

    RT_ASSERT(serial != RT_NULL);
    size = length;

    while (length)
    {
        ch = serial->ops->getc(serial);
        if (ch == -1) break;

        *data = ch;
        data ++; length --;

        if (ch == '\n') break;
    }

    return size - length;
}

rt_inline int _serial_poll_tx(struct rt_serial_device *serial, const rt_uint8_t *data, int length)
{
    int size;
    RT_ASSERT(serial != RT_NULL);

    size = length;
    while (length)
    {
        /*
         * to be polite with serial console add a line feed
         * to the carriage return character
         */
        if (*data == '\n' && (serial->parent.open_flag & RT_DEVICE_FLAG_STREAM))
        {
            serial->ops->putc(serial, '\r');
        }

        serial->ops->putc(serial, *data);

        ++ data;
        -- length;
    }

    return size - length;
}

rt_err_t rt_serial_init(rt_device_t dev)
{
  rt_err_t result = RT_EOK;
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  serial = (struct rt_serial_device *)dev;

  /* initialize rx/tx */
  serial->serial_rx = RT_NULL;
  serial->serial_tx = RT_NULL;

  /* apply configuration */
  if (serial->ops->configure)
      result = serial->ops->configure(serial, &serial->config);

  return result;
}

rt_err_t rt_serial_open(rt_device_t dev, rt_uint16_t oflag)
{
  
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  serial = (struct rt_serial_device *)dev;

  /* check device flag with the open flag */
  if ((oflag & RT_DEVICE_FLAG_DMA_RX) && !(dev->flag & RT_DEVICE_FLAG_DMA_RX))
      return -RT_EIO;
  if ((oflag & RT_DEVICE_FLAG_DMA_TX) && !(dev->flag & RT_DEVICE_FLAG_DMA_TX))
      return -RT_EIO;
  if ((oflag & RT_DEVICE_FLAG_INT_RX) && !(dev->flag & RT_DEVICE_FLAG_INT_RX))
      return -RT_EIO;
  if ((oflag & RT_DEVICE_FLAG_INT_TX) && !(dev->flag & RT_DEVICE_FLAG_INT_TX))
      return -RT_EIO;

  /* get open flags */
  dev->open_flag = oflag & 0xff;

  /* initialize the Rx/Tx structure according to open flag */
  if (serial->serial_rx == RT_NULL)
  {
    serial->serial_rx = RT_NULL;
  }

  if (serial->serial_tx == RT_NULL)
  {
    serial->serial_tx = RT_NULL;
  }

  return RT_EOK;
}

rt_err_t rt_serial_close(rt_device_t dev)
{
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  serial = (struct rt_serial_device *)dev;

  /* this device has more reference count */
  if (dev->ref_count > 1) return RT_EOK;

  return RT_EOK;
}
  
rt_size_t rt_serial_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  if (size == 0) return 0;

  serial = (struct rt_serial_device *)dev;

  return _serial_poll_rx(serial, buffer, size);
}

rt_size_t rt_serial_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
  
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  if (size == 0) return 0;

  serial = (struct rt_serial_device *)dev;
  
  return _serial_poll_tx(serial, buffer, size);
}

rt_err_t rt_serial_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
  struct rt_serial_device *serial;

  RT_ASSERT(dev != RT_NULL);
  serial = (struct rt_serial_device *)dev;

  switch (cmd)
  {
  case RT_DEVICE_CTRL_SUSPEND:
    /* suspend device */
    dev->flag |= RT_DEVICE_FLAG_SUSPENDED;
    break;

  case RT_DEVICE_CTRL_RESUME:
    /* resume device */
    dev->flag &= ~RT_DEVICE_FLAG_SUSPENDED;
    break;

  case RT_DEVICE_CTRL_CONFIG:
    if (args)
    {
      /* set serial configure */
      serial->config = *(struct usart_config *)args;

      if (dev->ref_count)
      {
        /* serial device has been opened, to configure it */
        serial->ops->configure(serial, (struct usart_config *)args);
      }
    }
    break;

  default :
    /* control device */
    serial->ops->control(serial, cmd, args);
    break;
  }
  return RT_EOK;
}

rt_err_t rt_serial_register(struct rt_serial_device *serial,
                               const char              *name,
                               rt_uint32_t              flag,
                               void                    *data)
{
    struct rt_device *device;
    RT_ASSERT(serial != RT_NULL);

    device = &(serial->parent);

    device->type        = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init        = rt_serial_init;
    device->open        = rt_serial_open;
    device->close       = rt_serial_close;
    device->read        = rt_serial_read;
    device->write       = rt_serial_write;
    device->control     = rt_serial_control;
    device->user_data   = data;

    /* register a character device */
    return rt_device_register(device, name, flag);
}
