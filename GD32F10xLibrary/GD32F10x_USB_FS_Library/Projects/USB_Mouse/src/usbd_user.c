/**
  ******************************************************************************
  * @file    usbd_user.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   User callback function for USB events management
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_user.h"

/* Private variables ---------------------------------------------------------*/
USBD_User_cb_TypeDef USER_cb =
{
    USBD_USER_DeviceInit,
    USBD_USER_DeviceReset,
    USBD_USER_DeviceConfigured,
    USBD_USER_DeviceSuspended,
    USBD_USER_DeviceResumed,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Device initialization for user
  * @param  None
  * @retval None
  */
void  USBD_USER_DeviceInit (void)
{
    /* Systick used for periodic check(10ms) mouse position */
    SysTick_Config(SystemCoreClock / 100);
}

/**
  * @brief  Device reset handle for user
  * @param  USBSpeed: usb device speed
  * @retval None
  */
void  USBD_USER_DeviceReset (uint8_t USBSpeed)
{
}

/**
  * @brief  Device configuration for user
  * @param  None
  * @retval None
  */
void  USBD_USER_DeviceConfigured (void)
{
}

/**
  * @brief  Device suspend handle for user
  * @param  None
  * @retval None
  */
void  USBD_USER_DeviceSuspended (void)
{
}


/**
  * @brief  Device resume handle for user
  * @param  None
  * @retval None
  */
void  USBD_USER_DeviceResumed (void)
{
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
