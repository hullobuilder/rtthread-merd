#include "bsp_key.h"

// TODO 需要支持按键按下回调

void local_key_init(void)
{
#if (BOARD_TYPE == STM32F103C8Tx)  

#elif (BOARD_TYPE == STM32F103ZETx)
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LOCAL_KEY0_RCC_CLOCK|LOCAL_KEYUP_RCC_CLOCK, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = LOCAL_KEY1_GPIO_PIN | LOCAL_KEY0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉
    GPIO_Init(LOCAL_KEY0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = LOCAL_KEYUP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉
    GPIO_Init(LOCAL_KEYUP_GPIO_PORT, &GPIO_InitStructure);
#endif
}

// if(KEY_ON == key_scan(KEY_FUNC_GPIO, KEY_FUNC_PORT))

// u8 key_scan(GPIO_TypeDef *GPIOx, u16 GPIO_Pin)
// {
//     if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
//     {
//         delay_ms(10);    // 这里需要使用OS的延时
//         if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
//         {
//             while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
//             return KEY_ON;
//         }
//         else
//             return KEY_OFF;
//     }
//     else
//         return KEY_OFF;
// }