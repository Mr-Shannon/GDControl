#ifndef __LED_H__
#define __LED_H__

#include <rtthread.h>
#include "gd32f10x.h"

// led define
#define led1_rcc                    RCC_APB2PERIPH_GPIOA
#define led1_gpio                   GPIOA
#define led1_pin                    (GPIO_PIN_15)

#define led2_rcc                    RCC_APB2PERIPH_GPIOB
#define led2_gpio                   GPIOB
#define led2_pin                    (GPIO_PIN_3)

#define led3_rcc                    RCC_APB2PERIPH_GPIOB
#define led3_gpio                   GPIOB
#define led3_pin                    (GPIO_PIN_4)

#define ledr_rcc                    RCC_APB2PERIPH_GPIOA
#define ledr_gpio                   GPIOA
#define ledr_pin                    (GPIO_PIN_8)

#define ledg_rcc                    RCC_APB2PERIPH_GPIOB
#define ledg_gpio                   GPIOB
#define ledg_pin                    (GPIO_PIN_6)

#define ledb_rcc                    RCC_APB2PERIPH_GPIOB
#define ledb_gpio                   GPIOB
#define ledb_pin                    (GPIO_PIN_7)

void led_init(void);
void led_on(rt_uint32_t led);
void led_off(rt_uint32_t led);

#endif
