#include "bsp.h" 

void Bsp_Current_Detector_Init(uint8_t current_channel)
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //配置ADC CRR寄存器 结构体
     GPIO_InitTypeDef GPIO_InitStructure;
    /*5A量程电流检测*/
    if(current_channel==1)
    {
        
    }
}