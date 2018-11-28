/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "opt.h"
#if LWIP_NETCONN

#include "api.h"
#include "sys.h"


#define UDPECHO_THREAD_PRIO  3

static struct netconn *conn;
static struct netbuf *buf;
static struct ip_addr *addr;
static unsigned short port;

static struct netbuf send_buf;

extern INT8U PCCOMMTxBuff[1500];
extern INT16U PCCOMMTxBuffLen;


/*-----------------------------------------------------------------------------------*/
static void udpecho_thread(void *arg)
{
  err_t err;
  	INT8U revbuff[20];
  	INT16U len=0;
  
  LWIP_UNUSED_ARG(arg);

  conn = netconn_new(NETCONN_UDP);
  if (conn!= NULL)
  {
    err = netconn_bind(conn, IP_ADDR_ANY, 8808);
    if (err == ERR_OK)
    {
      while (1) 
      {
        err = netconn_recv(conn,&buf);
      
        if (buf!= NULL) 
        {
          addr = netbuf_fromaddr(buf);
          port = netbuf_fromport(buf);
          netbuf_delete(buf);
       
        }
      }
    }
    else
    {
      printf("can not bind netconn");
    }
  }
  else
  {
    printf("can create new UDP netconn");
  }
}
/*-----------------------------------------------------------------------------------*/
void udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE,UDPECHO_THREAD_PRIO );
}

void APP_UDP_Sendto(INT8U *pbuff,INT32U len)
{
	buf->p->payload = pbuff;
	buf->p->len = len;
	buf->p->tot_len= buf->p->len;
	buf->ptr->payload = pbuff;
	buf->ptr->len = len;
	buf->ptr->tot_len= buf->ptr->len;
	netconn_sendto(conn,buf,addr,port);
	BSP_Delay_ms(2);
}


#endif /* LWIP_NETCONN */
