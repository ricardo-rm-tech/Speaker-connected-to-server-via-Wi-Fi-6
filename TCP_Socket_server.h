#ifndef __TCP_SOCKET_SERVER_H
#define __TCP_SOCKET_SERVER_H

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "rl_net.h"

//defines
#define TCP_Port 2000
#define RingSize (64*1024)
#define DAC_Samples 2048
#define UseMonoSound 0

//headers
uint32_t tcp_cb_server (int32_t socket, netTCP_Event event,
                        const NET_ADDR *addr, const uint8_t *buf, uint32_t len); 


#endif
