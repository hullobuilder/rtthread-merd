#ifndef __MULTIPLEX_ADC_H
#define	__MULTIPLEX_ADC_H

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "bsp_cfg.h"

#define M	3       // ˳����й���ת����ADCͨ������Ŀ
#define N	10      // ˳�������ADC����(��������)
// ��һ������5�֣�ÿ�ֲ���3��ͨ����ADC����


// ���ö�ͨ������������
// ģ����
#define ADC1_PIN    GPIO_Pin_0
#define ADC1_GPIOX  GPIOA
#define ADC2_PIN    GPIO_Pin_1
#define ADC2_GPIOX  GPIOA
#define ADC3_PIN    GPIO_Pin_2
#define ADC3_GPIOX  GPIOA


// ������
#define IOACCESS_RCC             RCC_APB2Periph_GPIOB
#define GPIO_IOACCESS_PORT       GPIOB
#define GPIO_IOACCESS_PIN_DEV1   GPIO_Pin_12
#define GPIO_IOACCESS_PIN_DEV2   GPIO_Pin_13
#define GPIO_IOACCESS_PIN_DEV3   GPIO_Pin_14


extern u16 ADC_SamplingArray[N][M];
extern u16 ADC_SamplingChannel[M];

void get_sampling_results(void );
uint8_t get_devn_status(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void multiadc_gpio_config(void);
void multiadc_channel_config(void);
void multiadc_dma_configuration(void);

void multiplex_adc_init(void);
void multiplex_ioaccess_init(void);
#endif /* __MULTIPLEX_ADC_H */
