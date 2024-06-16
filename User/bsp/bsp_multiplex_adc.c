#include "bsp_multiplex_adc.h"
#include "stm32f10x.h"

// ADC_Value:   [0][0]      [0][1]      [0][2]              [0][M-1]
//              [1][0]      [1][1]      [1][2]              [1][M-1]
//              ...         ...         ...                 ...
//              [N-1][0]    [N-1][1]    [N-1][2]            [N-1][M-1]
// ����ÿ�д���һ��ͨ��һ�β�����ֵ��ÿ�б�ʾN�β���������

// u16 ADC_Value[N][M];
u16 ADC_SamplingArray[N][M];// ADCת��ֵ��ԭʼ����
u16 ADC_SamplingChannel[M]; // ����ͨ��������

void get_sampling_results(void )
{
    int sum = 0;
    // MΪͨ������NΪ���ô�����ȡƽ��ֵ
    for(int count_M = 0; count_M < M; count_M++)
    {
        for(int count_N = 0; count_N < N; count_N++)
        {
            sum += ADC_SamplingArray[count_N][count_M];
        }
        ADC_SamplingChannel[count_M] = sum / N;
        sum = 0;
    }
}

// ʹ��ʾ����
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV1);
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV2);
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV3);
uint8_t get_devn_status(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}

void multiplex_adc_init(void)
{
    multiadc_gpio_config();                 // ADC ��ص����ų�ʼ��
    multiadc_channel_config();              // ����Ϊ��ͨ���ɼ�ģʽ
    multiadc_dma_configuration();           // ����DMA
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ��ʼADCת��
    DMA_Cmd(DMA1_Channel1, ENABLE);         // ����DMAͨ��
}

void multiplex_ioaccess_init(void){
    GPIO_InitTypeDef GPIO_IOACCESS;
    RCC_APB2PeriphClockCmd(IOACCESS_RCC,ENABLE);
    GPIO_IOACCESS.GPIO_Pin  = GPIO_IOACCESS_PIN_DEV1 | GPIO_IOACCESS_PIN_DEV2 | GPIO_IOACCESS_PIN_DEV3;
    GPIO_IOACCESS.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������ģʽ
    GPIO_Init(GPIO_IOACCESS_PORT, &GPIO_IOACCESS);
}

void multiadc_gpio_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		// ʹ��DMA��ADC��GPIOʱ��

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    //����ģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = ADC1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// ��������Ϊģ������ģʽ
    GPIO_Init(ADC1_GPIOX, & GPIO_InitStructure);            // ����ʱ������������

    //����ģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = ADC2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// ��������Ϊģ������ģʽ
    GPIO_Init(ADC2_GPIOX, & GPIO_InitStructure);            // ����ʱ������������

    //����ģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = ADC3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// ��������Ϊģ������ģʽ
    GPIO_Init(ADC3_GPIOX, & GPIO_InitStructure);            // ����ʱ������������
}

void multiadc_channel_config(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_DeInit(ADC1);                                                   //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ����ת���ر�
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = M;                             // ˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, & ADC_InitStructure);                                // ����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    // ����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    // ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );


    ADC_DMACmd(ADC1, ENABLE);               // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_Cmd(ADC1, ENABLE);                  // ʹ��ָ����ADC1
    ADC_ResetCalibration(ADC1);             // ��λָ����ADC1��У׼�Ĵ���
    ADC_ResetCalibration(ADC1);             // ��λָ����ADC1��У׼�Ĵ���
    ADC_StartCalibration(ADC1);             // ��ʼָ��ADC1��У׼״̬
    while(ADC_GetCalibrationStatus(ADC1));  // ��ȡָ��ADC1��У׼����,����״̬��ȴ�
}

void multiadc_dma_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //ʹ��DMA����
    DMA_DeInit(DMA1_Channel1);                                                  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32) & ADC1->DR;               //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32) & ADC_SamplingArray;           //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = N * M;                                   //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����λ���16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //���ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMAͨ��xû������Ϊ�ڴ浽�ڴ�
    DMA_Init(DMA1_Channel1, & DMA_InitStructure);                               //����DMA_InitStruct��ָ���Ĳ���DMAͨ��
}