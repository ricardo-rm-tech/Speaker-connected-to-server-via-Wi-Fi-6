#include "DAC.h"

osThreadId_t Th_DAC;        
osMessageQueueId_t Q_DAC;




const osThreadAttr_t thread1_attr_DAC = {
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


}



