#include "bsp_led.h"   


void local_led_init(void)
{
#if (BOARD_TYPE == STM32F103C8Tx)  
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( LOCAL_LED_RCC_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LOCAL_LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LOCAL_LED_GPIO_PORT, &GPIO_InitStructure); 
    GPIO_SetBits(LOCAL_LED_GPIO_PORT, LOCAL_LED_GPIO_PIN );
#elif (BOARD_TYPE == STM32F103ZETx)
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( LOCAL_LED1_RCC_CLOCK | LOCAL_LED2_RCC_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LOCAL_LED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LOCAL_LED_GPIO_PORT, &GPIO_InitStructure); 
    GPIO_SetBits(LOCAL_LED_GPIO_PORT, LOCAL_LED_GPIO_PIN );
#endif
}


