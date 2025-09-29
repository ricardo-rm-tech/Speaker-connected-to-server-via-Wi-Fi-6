#include "com.h"

static osThreadId_t Tid_Control_Com_recive;        
static osThreadId_t Tid_Control_Com_send;

osMessageQueueId_t Q_recive;
osMessageQueueId_t Q_send;

extern ARM_DRIVER_USART Driver_USART3;
void Com_Callback(uint32_t event);
static ARM_DRIVER_USART *USARTdrv = &Driver_USART3;

static char cmd;

static const osThreadAttr_t thread1_attr_com = {
  .stack_size = 1024                            // Create the thread stack with a size of 512 bytes
};


int Init_ThCom (void)
{
	
  Tid_Control_Com_recive = osThreadNew(ThControlComRecepcion, NULL, &thread1_attr_com); 
	Tid_Control_Com_send = osThreadNew(ThControlComTransmision, NULL, &thread1_attr_com);
	
  if (tid_Control_Com_recepcion == NULL)
  {
    return(-1);
  }
	if (tid_Control_Com_transmision == NULL)
  {
    return(-1);
  }
  return(0);
}

void ThControlComRecepcion(void *argument){

	Estado_t estado = InitState;
	char bufferDatos[BUFFER_SIZE];
	uint16_t i=1;
	Q_recive = osMessageQueueNew(4, sizeof(bufferDatos), NULL); 
	
	ARM_DRIVER_VERSION version;
	ARM_USART_CAPABILITIES drv_capabilities;
	USARTdrv->Initialize(Com_Callback);
	USARTdrv->PowerControl(ARM_POWER_FULL);
	USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE, 115200);
	USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
	USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
	
	while (1) {
		
		if (USARTdrv->Receive(&cmd, 1) != ARM_DRIVER_OK) {
    // Manejar el error
			osDelay(1000);
		}
		
		osThreadFlagsWait(Flag_Recibido, osFlagsWaitAny, osWaitForever);

		switch(estado){
		
			case InitState:
				if(cmd == SOH){
					bufferDatos[0]= SOH;
					estado = DefaultState;
				}
			break;
			
			case DefaultState:			
				if(cmd == EOT){
					if(bufferDatos[2] == (i+1)){
						bufferDatos[i] = EOT;
						osMessageQueuePut(Q_recive, &bufferDatos, NULL, 0U);
					}
					
					i=1;
					estado = InitState;
				}
				else{
				bufferDatos[i] = cmd;
				i++;
			}
			break;
			
		}	
	}
}

void ThControlComTransmision (void* argument){

	char buffer_datos_entrada[BUFFER_SIZE];
	Q_send = osMessageQueueNew(10, sizeof(buffer_datos_entrada), NULL);
	
	uint16_t i=0;
	while(1){
		osMessageQueueGet(Q_send, &buffer_datos_entrada, NULL, osWaitForever);
		
		do{
			USARTdrv->Send(&buffer_datos_entrada[i], 1);
			i++;
			osThreadFlagsWait(Flag_Transmitido, osFlagsWaitAny, osWaitForever);
		}while(buffer_datos_entrada[i]!= EOT);
		
		USARTdrv->Send(&buffer_datos_entrada[i], 1);
		osThreadFlagsWait(Flag_Transmitido, osFlagsWaitAny, osWaitForever);
		i=0;
	}

}



void Com_Callback(uint32_t event) {
    // Manejo de eventos del driver
	uint32_t mask;
	uint32_t mask2;
	mask = ARM_USART_EVENT_RECEIVE_COMPLETE;
	mask2= ARM_USART_EVENT_TRANSFER_COMPLETE | ARM_USART_EVENT_SEND_COMPLETE | ARM_USART_EVENT_TX_COMPLETE;
	if(event & mask) {
		osThreadFlagsSet(Tid_Control_Com_recive, Flag_Recibido);
	}
	if(event & mask2){
		osThreadFlagsSet(Tid_Control_Com_send, Flag_Transmitido);
	}
	
}
