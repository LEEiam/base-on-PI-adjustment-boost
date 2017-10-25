

#include "bsp.h"
#include "stm32f4xx.h"                  // Device header


//extern uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx);  //�õ�GPIOX��ʱ��
//extern uint16_t bsp_GetPinSource(uint16_t gpio_pin);    //��GPIO_Pin_X �õ� GPIO_PinSource
//extern uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIM1);   //�ɶ�ʱ��TIM1�õ���Ӧ��ʱ��
//extern uint8_t bsp_GetAFofTIM(TIM_TypeDef* TIM1);    //�ɶ�ʱ��TIM1�õ����ø���AF����
//extern void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinX, TIM_TypeDef* TIM1, uint8_t _ucChannel); 


void bsp_ADVANCED_TIMOutPWM(TIM_TypeDef* TIMx, uint8_t _ucChannel,uint32_t _ulFreq, float _ulDutyCycle)
{
     
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;
//    TIM_BDTRInitTypeDef         TIM_BDTRInitStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
    
    uint32_t uiTIM1CLK=SystemCoreClock;
    uint16_t usPeriod;
    usPeriod=uiTIM1CLK / _ulFreq -1;    //ARR��ֵ
   
    /*��ʼ��GPIOA8�����ӵ�TIM_1_CH1*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
    
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
    
    /*��ʼ��GPIOE8�����ӵ�TIM_1_CH1N*/
     GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1); 
    
    
     /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
    
    /*���ö�ʱ��TIM1�Ļ�������*/
    TIM_TimeBaseStructure.TIM_Prescaler=0;//Ԥ��Ƶϵ�� 0
    TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //�ظ�������� ��Ӧ TIM->RCR
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM ����ʱ�ӷ�Ƶ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    
    /*����PWM�������*/
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //���PWM1 ģʽ
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //�ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //ʹ��ͨ�����
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //�ߵ�ƽ��Ч��Nͨ�������Ӧͨ��ͬΪ�߻��ߵͲſ��Ի������
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //ʹ��Nͨ�����
    TIM_OCInitStructure.TIM_Pulse=_ulDutyCycle*(SystemCoreClock/_ulFreq-1);  //_ulDutyCycle*(SystemCoreClock/_ulFreq-1);   //ռ�ձ�  TIM -> CRR
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  ����ʱ��MOE=0��ͨ�����״̬
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  ����ʱ��MOE=0��ͨ��N���״̬
    
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);          //ͨ��Ԥװ��ʹ��
    
//    /*����PWM�������·����*/
//    TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;  
//    TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;
//    TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
//    TIM_BDTRInitStructure.TIM_DeadTime=50;// DT*(1/168M)
//    TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
//    TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;
//    TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;
//    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
    
    TIM_ARRPreloadConfig(TIM1, ENABLE);
//       TIM_UpdateDisableConfig(TIM1, DISABLE);
//    TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular);
    
    
     NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_TIM10_IRQn ;  //TIM1 �ж�  
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //��ռ���ȼ� 0 �� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ� 3 �� 
      NVIC_Init(&NVIC_InitStructure);   //�ܳ�ʼ�� NVIC �Ĵ���  
//      TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
      
     TIM_CCPreloadControl(TIM1,ENABLE);
     TIM_CtrlPWMOutputs(TIM1, ENABLE);  //д��MOE=1;
//     TIM_Cmd(TIM1, ENABLE);
    
}
