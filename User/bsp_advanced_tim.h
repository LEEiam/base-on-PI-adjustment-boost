#ifndef __BSP_ADVANCED_TIM_H
#define __BSP_ADVANCED_TIM_H

#include "stm32f4xx.h"

void bsp_ADVANCED_TIMOutPWM(TIM_TypeDef* TIMx, uint8_t _ucChannel,uint32_t _ulFreq, float _ulDutyCycle);





#endif 
