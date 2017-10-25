#include "bsp.h"
#include "arm_math.h"

#define ADC1_DR_ADDRESS          ((uint32_t)0X4001204C)

//uint16_t ADC_Converted_Value[10];


void bsp_InitADC()
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //配置ADC CRR寄存器 结构体
	DMA_InitTypeDef        DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    /*使能DMA.ADC,GPIO时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    ADC_DeInit();// 复位ADC的所有寄存器
    
    /*配置GPIOA1为模拟输入，对应ADC1_Channel_1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 
    
//    /*配置DMA2 ADC1 对应 数据流0 通道0 */
//    	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;  //外设数据地址
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_Converted_Value;;  //内存数据地址
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //外设到内存
//	DMA_InitStructure.DMA_BufferSize = 10;  //数据总长度
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址递增不使能
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存递增递增使能
//    
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据大小 半字长 16bit
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;         //内存地址数据大小   半字长 16bit
//    
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //循环模式
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;            //等级非常高
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         //不使能FIFO
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;   
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    //单次模式
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  //单次模拟
//	DMA_Init(DMA2_Stream0, &DMA_InitStructure);  //DMA2_Stream0 初始化
//    
//    DMA_Cmd(DMA2_Stream0, ENABLE);// DMA2_Stream0 使能 
    
    /* ADC Common   配置   CRR寄存器配置----------------------------------------------------------*/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //应用在多重模式下
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟(三重模式或双重模式下使用)
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    
    /* ADC1 通道1 规则通道配置 ---------------------------------------------------------*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;                                 //使能扫描模式(多通道ADC采集要用扫描模式)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                           //使能连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //触发源  使用软件触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //定时器事件2触发ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //规则序列通道数量 1
    ADC_Init(ADC1, &ADC_InitStructure);
    
     
//    ADC_DMACmd(ADC1, ENABLE); //使能ADC1 DMA ADC->CR2-DMA位写1
    
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);//规则通道配置  ADC1_Channel1配置为等级1 ，采样时间15个周期
    
//     ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  //发生数据转换后就产生DMA请求  ADC->CR2-DDS 位写1
    
    ADC_Cmd(ADC1, ENABLE);//使能ADC1，对ADC上电
    ;
//    
//        ADC1->CR2|=ADC_CR2_EOCS;
    
//     ADC_SoftwareStartConv(ADC1);//ADC1转换开始  ADC_CR2_SWSTART写1
    
}



