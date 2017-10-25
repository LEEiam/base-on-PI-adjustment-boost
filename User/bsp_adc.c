#include "bsp.h"
#include "arm_math.h"

#define ADC1_DR_ADDRESS          ((uint32_t)0X4001204C)

//uint16_t ADC_Converted_Value[10];


void bsp_InitADC()
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //����ADC CRR�Ĵ��� �ṹ��
	DMA_InitTypeDef        DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    /*ʹ��DMA.ADC,GPIOʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    ADC_DeInit();// ��λADC�����мĴ���
    
    /*����GPIOA1Ϊģ�����룬��ӦADC1_Channel_1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 
    
//    /*����DMA2 ADC1 ��Ӧ ������0 ͨ��0 */
//    	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;  //�������ݵ�ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_Converted_Value;;  //�ڴ����ݵ�ַ
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //���赽�ڴ�
//	DMA_InitStructure.DMA_BufferSize = 10;  //�����ܳ���
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ������ʹ��
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��������ʹ��
//    
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݴ�С ���ֳ� 16bit
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;         //�ڴ��ַ���ݴ�С   ���ֳ� 16bit
//    
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //ѭ��ģʽ
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;            //�ȼ��ǳ���
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         //��ʹ��FIFO
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;   
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    //����ģʽ
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  //����ģ��
//	DMA_Init(DMA2_Stream0, &DMA_InitStructure);  //DMA2_Stream0 ��ʼ��
//    
//    DMA_Cmd(DMA2_Stream0, ENABLE);// DMA2_Stream0 ʹ�� 
    
    /* ADC Common   ����   CRR�Ĵ�������----------------------------------------------------------*/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //Ӧ���ڶ���ģʽ��
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���������׶�֮����ӳ�5��ʱ��(����ģʽ��˫��ģʽ��ʹ��)
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    
    /* ADC1 ͨ��1 ����ͨ������ ---------------------------------------------------------*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;                                 //ʹ��ɨ��ģʽ(��ͨ��ADC�ɼ�Ҫ��ɨ��ģʽ)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                           //ʹ������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Դ  ʹ���������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //��ʱ���¼�2����ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //��������ͨ������ 1
    ADC_Init(ADC1, &ADC_InitStructure);
    
     
//    ADC_DMACmd(ADC1, ENABLE); //ʹ��ADC1 DMA ADC->CR2-DMAλд1
    
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);//����ͨ������  ADC1_Channel1����Ϊ�ȼ�1 ������ʱ��15������
    
//     ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  //��������ת����Ͳ���DMA����  ADC->CR2-DDS λд1
    
    ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1����ADC�ϵ�
    ;
//    
//        ADC1->CR2|=ADC_CR2_EOCS;
    
//     ADC_SoftwareStartConv(ADC1);//ADC1ת����ʼ  ADC_CR2_SWSTARTд1
    
}



