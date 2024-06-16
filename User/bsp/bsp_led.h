#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "bsp_cfg.h"

#define LED_ON  1
#define LED_OFF 0

// ֱ�Ӳ����Ĵ����ķ�������IO
#define	digitalHi(p,i)          {p->BSRR = i;}    //���Ϊ�ߵ�ƽ
#define digitalLo(p,i)          {p->BRR = i;}     //����͵�ƽ
#define digitalToggle(p,i)      {p->ODR ^= i;}   //�����ת״̬

#if (BOARD_TYPE == STM32F103C8Tx)
    #define LOCAL_LED_RCC_CLOCK RCC_APB2Periph_GPIOC
    #define LOCAL_LED_GPIO_PORT GPIOC
    #define LOCAL_LED_GPIO_PIN  GPIO_Pin_13

    #define LOCAL_LED_TOGGLE digitalToggle(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED_OFF    digitalHi(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
    #define LOCAL_LED_ON     digitalLo(LOCAL_LED_GPIO_PORT,LOCAL_LED_GPIO_PIN)
#elif (BOARD_TYPE == STM32F103ZETx)
    #define LOCAL_LED0_RCC_CLOCK RCC_APB2Periph_GPIOB
    #define LOCAL_LED0_GPIO_PORT GPIOB
    #define LOCAL_LED0_GPIO_PIN  GPIO_Pin_5

    #define LOCAL_LED1_RCC_CLOCK RCC_APB2Periph_GPIOE
    #define LOCAL_LED1_GPIO_PORT GPIOE
    #define LOCAL_LED1_GPIO_PIN  GPIO_Pin_5

    #define LOCAL_LED0_TOGGLE digitalToggle(LOCAL_LED0_GPIO_PORT,LOCAL_LED0_GPIO_PIN)
    #define LOCAL_LED0_OFF    digitalHi(LOCAL_LED0_GPIO_PORT,LOCAL_LED0_GPIO_PIN)
    #define LOCAL_LED0_ON     digitalLo(LOCAL_LED0_GPIO_PORT,LOCAL_LED0_GPIO_PIN)

    #define LOCAL_LED1_TOGGLE digitalToggle(LOCAL_LED1_GPIO_PORT,LOCAL_LED1_GPIO_PIN)
    #define LOCAL_LED1_OFF    digitalHi(LOCAL_LED1_GPIO_PORT,LOCAL_LED1_GPIO_PIN)
    #define LOCAL_LED1_ON     digitalLo(LOCAL_LED1_GPIO_PORT,LOCAL_LED1_GPIO_PIN)
#endif


void local_led_init(void);

#endif /* __LED_H */
