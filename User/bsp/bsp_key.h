#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "bsp_cfg.h"

#define KEY_ON 0
#define KEY_OFF 1

#if (BOARD_TYPE == STM32F103C8Tx)


#elif (BOARD_TYPE == STM32F103ZETx)
    #define LOCAL_KEY0_RCC_CLOCK RCC_APB2Periph_GPIOE
    #define LOCAL_KEY0_GPIO_PORT GPIOE
    #define LOCAL_KEY0_GPIO_PIN  GPIO_Pin_4

    #define LOCAL_KEY1_RCC_CLOCK RCC_APB2Periph_GPIOE
    #define LOCAL_KEY1_GPIO_PORT GPIOE
    #define LOCAL_KEY1_GPIO_PIN  GPIO_Pin_3

    #define LOCAL_KEYUP_RCC_CLOCK RCC_APB2Periph_GPIOA
    #define LOCAL_KEYUP_GPIO_PORT GPIOA
    #define LOCAL_KEYUP_GPIO_PIN  GPIO_Pin_0
#endif

void local_key_init(void);
u8 key_scan(GPIO_TypeDef *GPIOx, u16 GPIO_Pin);

#endif /* __KEY_H */
