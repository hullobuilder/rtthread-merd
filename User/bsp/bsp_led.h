#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

#define LED_ON  1
#define LED_OFF 0

// 直接操作寄存器的方法控制IO
#define	digitalHi(p,i)          {p->BSRR=i;}    //输出为高电平
#define digitalLo(p,i)          {p->BRR=i;}     //输出低电平
#define digitalToggle(p,i)      {p->ODR ^=i;}   //输出反转状态

enum BOARD_TYPE{
    STM32F103C8Tx = 1,
    STM32F103ZETx
};
#define BOARD_TYPE STM32F103C8Tx


#if (BOARD_TYPE == STM32F103C8Tx)
    #define LOCAL_LED_RCC_CLOCK RCC_APB2Periph_GPIOC
    #define LOCAL_LED_GPIO_PORT GPIOC
    #define LOCAL_LED_GPIO_PIN  GPIO_Pin_13

    #define LOCAL_LED_TOGGLE digitalToggle(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED_OFF    digitalHi(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED_ON     digitalLo(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)

#elif (BOARD_TYPE == STM32F103ZETx)
    #define LOCAL_LED1_RCC_CLOCK RCC_APB2Periph_GPIOC
    #define LOCAL_LED1_GPIO_PORT GPIOC
    #define LOCAL_LED1_GPIO_PIN  GPIO_Pin_13

    #define LOCAL_LED2_RCC_CLOCK RCC_APB2Periph_GPIOC
    #define LOCAL_LED2_GPIO_PORT GPIOC
    #define LOCAL_LED2_GPIO_PIN  GPIO_Pin_13

    #define LOCAL_LED1_TOGGLE digitalToggle(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED1_OFF    digitalHi(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED1_ON     digitalLo(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)

    #define LOCAL_LED2_TOGGLE digitalToggle(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED2_OFF    digitalHi(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED2_ON     digitalLo(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
#endif


void local_led_init(void);

#endif /* __LED_H */
