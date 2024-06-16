#include "bsp_multiplex_adc.h"
#include "stm32f10x.h"

// ADC_Value:   [0][0]      [0][1]      [0][2]              [0][M-1]
//              [1][0]      [1][1]      [1][2]              [1][M-1]
//              ...         ...         ...                 ...
//              [N-1][0]    [N-1][1]    [N-1][2]            [N-1][M-1]
// 即：每列代表一个通道一次采样的值，每行表示N次采样的数据

// u16 ADC_Value[N][M];
u16 ADC_SamplingArray[N][M];// ADC转化值，原始数组
u16 ADC_SamplingChannel[M]; // 单个通道的数组

void get_sampling_results(void )
{
    int sum = 0;
    // M为通道数，N为采用次数，取平均值
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

// 使用示例：
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV1);
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV2);
// get_devn_status(GPIO_IOACCESS_PORT, GPIO_IOACCESS_PIN_DEV3);
uint8_t get_devn_status(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}

void multiplex_adc_init(void)
{
    multiadc_gpio_config();                 // ADC 相关的引脚初始化
    multiadc_channel_config();              // 配置为多通道采集模式
    multiadc_dma_configuration();           // 配置DMA
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 开始ADC转换
    DMA_Cmd(DMA1_Channel1, ENABLE);         // 启动DMA通道
}

void multiplex_ioaccess_init(void){
    GPIO_InitTypeDef GPIO_IOACCESS;
    RCC_APB2PeriphClockCmd(IOACCESS_RCC,ENABLE);
    GPIO_IOACCESS.GPIO_Pin  = GPIO_IOACCESS_PIN_DEV1 | GPIO_IOACCESS_PIN_DEV2 | GPIO_IOACCESS_PIN_DEV3;
    GPIO_IOACCESS.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入模式
    GPIO_Init(GPIO_IOACCESS_PORT, &GPIO_IOACCESS);
}

void multiadc_gpio_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		// 使能DMA、ADC、GPIO时钟

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    //配置模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = ADC1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// 配置引脚为模拟输入模式
    GPIO_Init(ADC1_GPIOX, & GPIO_InitStructure);            // 输入时不用设置速率

    //配置模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = ADC2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// 配置引脚为模拟输入模式
    GPIO_Init(ADC2_GPIOX, & GPIO_InitStructure);            // 输入时不用设置速率

    //配置模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = ADC3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// 配置引脚为模拟输入模式
    GPIO_Init(ADC3_GPIOX, & GPIO_InitStructure);            // 输入时不用设置速率
}

void multiadc_channel_config(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_DeInit(ADC1);                                                   //将外设 ADC1 的全部寄存器重设为缺省值
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 模数转换工作在连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换关闭
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = M;                             // 顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, & ADC_InitStructure);                                // 根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    // 设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    // ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );


    ADC_DMACmd(ADC1, ENABLE);               // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_Cmd(ADC1, ENABLE);                  // 使能指定的ADC1
    ADC_ResetCalibration(ADC1);             // 复位指定的ADC1的校准寄存器
    ADC_ResetCalibration(ADC1);             // 复位指定的ADC1的校准寄存器
    ADC_StartCalibration(ADC1);             // 开始指定ADC1的校准状态
    while(ADC_GetCalibrationStatus(ADC1));  // 获取指定ADC1的校准程序,设置状态则等待
}

void multiadc_dma_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //使能DMA传输
    DMA_DeInit(DMA1_Channel1);                                                  //将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32) & ADC1->DR;               //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32) & ADC_SamplingArray;           //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = N * M;                                   //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据位宽度16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMA通道x没有设置为内存到内存
    DMA_Init(DMA1_Channel1, & DMA_InitStructure);                               //根据DMA_InitStruct中指定的参数DMA通道
}