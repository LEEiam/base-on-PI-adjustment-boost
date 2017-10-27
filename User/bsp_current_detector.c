#include "bsp.h" 

void Bsp_Current_Detector_Init(uint8_t current_channel)
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //配置ADC CRR寄存器 结构体
     GPIO_InitTypeDef GPIO_InitStructure;
    /*5A量程电流检测*/
    if(current_channel==5)
    {
         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   //使能ADC2时钟
        
        /*配置GPIOA1为模拟输入，对应ADC2_Channel_1*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        
         /* ADC Common Configuration --> CRR Register Configuration ----------------------------------------------------------*/
        ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
        ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
        ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //应用在多重模式下
        ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟(三重模式或双重模式下使用)
        ADC_CommonInit(&ADC_CommonInitStructure);
        
        /*ADC2 Initialize Configuration  --> CR1 and CR2 Register Configuration*/
        ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
        ADC_InitStructure.ADC_ScanConvMode =DISABLE;                                 //使能扫描模式(多通道ADC采集要用扫描模式)
        ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;                           //使能连续转换
        ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //触发源  使用软件触发
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //定时器事件2触发ADC
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
        ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //规则序列通道数量 1
        ADC_Init(ADC2, &ADC_InitStructure);
        
        //ADC2 规则通道1 配置为等级1 15个采样周期
        ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1,ADC_SampleTime_15Cycles);
        
         ADC_Cmd(ADC2, ENABLE);//使能ADC2，对ADC上电
         
         ADC_SoftwareStartConv(ADC2);//ADC2转换开始  ADC2_CR2_SWSTART写1
    }
}

float Current_Convert_Value(uint8_t current_channel)
{
    uint32_t temp=0,i=10;
    float Current_Convert_Value,Votage_temp;
    if(current_channel==5)
    {
        while(i--)
        {    
            temp+=ADC_GetConversionValue(ADC2);  //获得10次ADC_DR寄存器的值
            1;
        }
       
        Votage_temp=(temp*1.0/10)*(3.0/4095);    //求temp平均值后转换出电压值
//        printf("\r\nVotage_temp=%f\n\r",Votage_temp);
        //Votage_temp*2后获得真正的ACS712的输出电压值，因为使用了分压电路，然后减去2.5V的0电流时的基准值后得到的电压值乘以精度
        Current_Convert_Value = ( Votage_temp*2 - 2.5) / 0.185; 
//        printf("\r\nCurrent_Convert_Value=%f\n\r",Current_Convert_Value);
        return Current_Convert_Value;
    }
}