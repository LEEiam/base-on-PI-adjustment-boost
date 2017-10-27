#include "bsp.h" 

void Bsp_Current_Detector_Init(uint8_t current_channel)
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //����ADC CRR�Ĵ��� �ṹ��
     GPIO_InitTypeDef GPIO_InitStructure;
    /*5A���̵������*/
    if(current_channel==5)
    {
         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   //ʹ��ADC2ʱ��
        
        /*����GPIOA1Ϊģ�����룬��ӦADC2_Channel_1*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        
         /* ADC Common Configuration --> CRR Register Configuration ----------------------------------------------------------*/
        ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //����ģʽ
        ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
        ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //Ӧ���ڶ���ģʽ��
        ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���������׶�֮����ӳ�5��ʱ��(����ģʽ��˫��ģʽ��ʹ��)
        ADC_CommonInit(&ADC_CommonInitStructure);
        
        /*ADC2 Initialize Configuration  --> CR1 and CR2 Register Configuration*/
        ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
        ADC_InitStructure.ADC_ScanConvMode =DISABLE;                                 //ʹ��ɨ��ģʽ(��ͨ��ADC�ɼ�Ҫ��ɨ��ģʽ)
        ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;                           //ʹ������ת��
        ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Դ  ʹ���������
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //��ʱ���¼�2����ADC
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���
        ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //��������ͨ������ 1
        ADC_Init(ADC2, &ADC_InitStructure);
        
        //ADC2 ����ͨ��1 ����Ϊ�ȼ�1 15����������
        ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1,ADC_SampleTime_15Cycles);
        
         ADC_Cmd(ADC2, ENABLE);//ʹ��ADC2����ADC�ϵ�
         
         ADC_SoftwareStartConv(ADC2);//ADC2ת����ʼ  ADC2_CR2_SWSTARTд1
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
            temp+=ADC_GetConversionValue(ADC2);  //���10��ADC_DR�Ĵ�����ֵ
            1;
        }
       
        Votage_temp=(temp*1.0/10)*(3.0/4095);    //��tempƽ��ֵ��ת������ѹֵ
//        printf("\r\nVotage_temp=%f\n\r",Votage_temp);
        //Votage_temp*2����������ACS712�������ѹֵ����Ϊʹ���˷�ѹ��·��Ȼ���ȥ2.5V��0����ʱ�Ļ�׼ֵ��õ��ĵ�ѹֵ���Ծ���
        Current_Convert_Value = ( Votage_temp*2 - 2.5) / 0.185; 
//        printf("\r\nCurrent_Convert_Value=%f\n\r",Current_Convert_Value);
        return Current_Convert_Value;
    }
}