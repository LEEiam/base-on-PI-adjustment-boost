

#include "bsp.h"
#include "stm32f4xx.h"                  // Device header


//extern uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx);  //得到GPIOX的时钟
//extern uint16_t bsp_GetPinSource(uint16_t gpio_pin);    //由GPIO_Pin_X 得到 GPIO_PinSource
//extern uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIM1);   //由定时器TIM1得到对应的时钟
//extern uint8_t bsp_GetAFofTIM(TIM_TypeDef* TIM1);    //由定时器TIM1得到对用复用AF参数
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
    usPeriod=uiTIM1CLK / _ulFreq -1;    //ARR的值
   
    /*初始化GPIOA8，连接到TIM_1_CH1*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
    
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
    
    /*初始化GPIOE8，连接到TIM_1_CH1N*/
     GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1); 
    
    
     /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
    
    /*配置定时器TIM1的基础参数*/
    TIM_TimeBaseStructure.TIM_Prescaler=0;//预分频系数 0
    TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //重复溢出计数 对应 TIM->RCR
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM 总线时钟分频
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    
    /*配置PWM输出参数*/
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //输出PWM1 模式
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //高电平有效
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //使能通道输出
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //高电平有效，N通道须与对应通道同为高或者低才可以互补输出
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //使能N通道输出
    TIM_OCInitStructure.TIM_Pulse=_ulDutyCycle*(SystemCoreClock/_ulFreq-1);  //_ulDutyCycle*(SystemCoreClock/_ulFreq-1);   //占空比  TIM -> CRR
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  空闲时（MOE=0）通道输出状态
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  空闲时（MOE=0）通道N输出状态
    
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);          //通道预装载使能
    
//    /*互补PWM死区与断路设置*/
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
    
    
     NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_TIM10_IRQn ;  //TIM1 中断  
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //先占优先级 0 级 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级 3 级 
      NVIC_Init(&NVIC_InitStructure);   //④初始化 NVIC 寄存器  
//      TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
      
     TIM_CCPreloadControl(TIM1,ENABLE);
     TIM_CtrlPWMOutputs(TIM1, ENABLE);  //写入MOE=1;
//     TIM_Cmd(TIM1, ENABLE);
    
}
