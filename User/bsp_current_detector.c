#include "bsp.h" 

void Bsp_Current_Detector_Init(uint8_t current_channel)
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //����ADC CRR�Ĵ��� �ṹ��
     GPIO_InitTypeDef GPIO_InitStructure;
    /*5A���̵������*/
    if(current_channel==1)
    {
        
    }
}