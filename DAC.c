#include "DAC.h"

static osThreadId_t Th_DAC;        
osMessageQueueId_t Q_DAC;

static DAC_HandleTypeDef hdac;
static TIM_HandleTypeDef htim6;

static const osThreadAttr_t thread1_attr_DAC = {
  .stack_size = 1024                            // Create the thread stack with a size of 512 bytes
};


int Init_ThDAC(void)
{
	
  Th_DAC = osThreadNew(ThControlDAC, NULL, &thread1_attr_DAC); 
	
	
  if (Th_DAC == NULL)
  {
    return(-1);
  }

  return(0);
}

void ThControlDAC(void *argument){
	DAC_Conf();
	TIM6_Conf();
	Q_DAC = osMessageQueueNew(4, sizeof(BUFF_DAC), NULL);
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t*)BUFF_DAC,N_samples,DAC_ALIGN_12B_R);
	
	while(1){
		osDelay(1000);
	}
}

void DAC_Conf(void){
	DAC_ChannelConfTypeDef DConfig;
	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_DAC_CLK_ENABLE();
	
	// inicializacion
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);
	
	//Configuraremos el audio mono por el DAC 1
	DConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	DConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac,&DConfig,DAC_CHANNEL_1);
	
	// config pin A4 Salida dac2
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}
void TIM6_Conf(void){

	__HAL_RCC_TIM6_CLK_ENABLE();

	// configuramos el timer para el doble de la frecmax que se puede escuchar 48KHz
	htim6.Instance = TIM6;
	htim6.Init.Prescaler=34;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period=49;
	
	TIM_MasterConfigTypeDef Mast_Config;
	Mast_Config.MasterOutputTrigger = TIM_TRGO_UPDATE;
	Mast_Config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim6,&Mast_Config);
	HAL_TIM_Base_Start(&htim6);
	
	

}
