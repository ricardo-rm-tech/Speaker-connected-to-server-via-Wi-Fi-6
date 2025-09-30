/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    TCP_Socket_Server.c
 * Purpose: TCP Socket Server Code Template
 * Rev.:    V7.0.0
 *----------------------------------------------------------------------------*/
//! [code_TCP_Socket_Server]
#include "TCP_Socket_server.h"

static volatile bool Server_con = false;
static volatile bool Header_ok= false;
static size_t Header_len =0;
static uint8_t Ring[RingSize];
static char Buffer_Datos[64];

// Notify the user application about TCP socket events.
uint32_t tcp_cb_server (int32_t socket, netTCP_Event event,
                        const NET_ADDR *addr, const uint8_t *buf, uint32_t len) {
 
  switch (event) {
    case netTCP_EventConnect:
      // Connect request received
     
      if (addr->addr_type == NET_ADDR_IP4) {
        // IPv4 client
        if (addr->addr[0] == 192  &&
            addr->addr[1] == 168  &&
            addr->addr[2] == 0    &&
            addr->addr[3] == 1) {
          // Accept connection from client at 192.168.0.1
          return (1);
        }
      }
      
      // Deny connection.
      return (0);
			
			
    case netTCP_EventEstablished:
      // Connection established
			Server_con = true;
			Header_ok = false;
			Header_len = 0;
      break;
 
    case netTCP_EventClosed:
			Server_con = false;
			Header_ok = false;
			Header_len = 0;
      break;
    case netTCP_EventAborted:
      // Connection is for some reason aborted
			Server_con = false;
			Header_ok = false;
			Header_len = 0;
      break;
 
    case netTCP_EventACK:
      // Previously sent data acknowledged
      break;
 
    case netTCP_EventData:
      if(!Header_ok){
				for(uint32_t i = 0; i< len && Header_len < sizeof(Buffer_Datos)-1; i++){
					Buffer_Datos[Header_len++] = buf[i];
					if(buf[i]== '\n'){
						Buffer_Datos[Header_len] = 0;
						Header_ok = parse_header_line(Buffer_Datos);
					}
				
				}
			}
      break;
  }
  return (0);
}
 
// Allocate and initialize the socket.
/* Example
int main (void) {
  int32_t tcp_sock;
 
  netInitialize ();
 
  // Initialize TCP Socket and start listening on port 2000
  tcp_sock = netTCP_GetSocket (tcp_cb_server);
  if (tcp_sock > 0) {
    netTCP_Listen (tcp_sock, 2000);
  }
}
*/
//! [code_TCP_Socket_Server]

