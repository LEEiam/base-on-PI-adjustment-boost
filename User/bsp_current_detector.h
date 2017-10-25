#ifndef __BSP_CURRENT_DETECTOR_H
#define __BSP_CURRENT_DETECTOR_H

#include "stm32f4xx.h"


void Bsp_Current_Detector_Init(uint8_t current_channel);

float Current_Convert_Value();

#endif
