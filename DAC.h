#ifndef __DAC_H
#define __DAC_H

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int Init_ThDAC(void);
void ThControlDAC(void *argument);
void DAC_Conf(void);
void TIM6_Conf(void);

#define N_samples 2048

uint16_t BUFF_DAC[N_samples];

#endif
