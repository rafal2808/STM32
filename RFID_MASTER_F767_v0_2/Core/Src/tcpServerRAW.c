/*
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************

  File:		  	   tcpServerRAW.c
  Modified By:     ControllersTech.com
  Updated:    	   26-Jul-2021

  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ***************************************************************************************************************
*/


/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable  application.
 *
 **/

 /* This file was modified by ST */

#include "tcpserverRAW.h"
#include "main.h"
#include "lwip/tcp.h"



/*  protocol states */
enum tcp_server_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

/* structure for maintaining connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_server_struct
{
  u8_t state;             /* current connection state */
  u8_t retries;
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};

char buf2[6];
char buf[100];

uint8_t PAL_00[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_01[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_02[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_04[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_05[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_06[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_07[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_08[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_09[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_10[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_11[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_12[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_13[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_14[14] = {"PAL_Linia_X_XX"};
uint8_t PAL_15[14] = {"PAL_Linia_X_XX"};

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);

static void tcp_server_handle (struct tcp_pcb *tpcb, struct tcp_server_struct *es);


/* Impementation for the TCP Server
   1. Create TCP Block.
   2. Bind the Block to server address, and port.
   3. Listen for the  incoming requests by the client
   4. Accept the Request, and now the server is ready for the data transfer
 */

void tcp_server_init(void)
{
	/* 1. create new tcp pcb */
	struct tcp_pcb *tpcb;

	tpcb = tcp_new();

	err_t err;

	/* 2. bind _pcb to port 7 ( protocol) */
	ip_addr_t myIPADDR;
	IP_ADDR4(&myIPADDR, 172, 31, 165, 100);
	err = tcp_bind(tpcb, &myIPADDR, 1100);

	if (err == ERR_OK)
	{
		/* 3. start tcp listening for _pcb */
		tpcb = tcp_listen(tpcb);

		/* 4. initialize LwIP tcp_accept callback function */
		tcp_accept(tpcb, tcp_server_accept);
	}
	else
	{
		/* deallocate the pcb */
		memp_free(MEMP_TCP_PCB, tpcb);
	}
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used
  * @retval err_t: error status
  */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* set priority for the newly accepted tcp connection newpcb */
  tcp_setprio(newpcb, TCP_PRIO_MIN);

  /* allocate structure es to maintain tcp connection information */
  es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
  if (es != NULL)
  {
    es->state = ES_ACCEPTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;

    /* pass newly allocated es structure as argument to newpcb */
    tcp_arg(newpcb, es);

    /* initialize lwip tcp_recv callback function for newpcb  */
    tcp_recv(newpcb, tcp_server_recv);

    /* initialize lwip tcp_err callback function for newpcb  */
    tcp_err(newpcb, tcp_server_error);

    /* initialize lwip tcp_poll callback function for newpcb */
    tcp_poll(newpcb, tcp_server_poll, 0);

    ret_err = ERR_OK;
  }
  else
  {
    /*  close tcp connection */
    tcp_server_connection_close(newpcb, es);
    /* return memory error */
    ret_err = ERR_MEM;
  }
  return ret_err;
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_server_struct *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct tcp_server_struct *)arg;

  /* if we receive an empty tcp frame from client => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p == NULL)
    {
       /* we're done sending, close connection */
       tcp_server_connection_close(tpcb, es);
    }
    else
    {
      /* we're not done yet */
      /* acknowledge received packet */
      tcp_sent(tpcb, tcp_server_sent);

      /* send remaining data*/
      tcp_server_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from client but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_ACCEPTED)
  {
    /* first data chunk in p->payload */
    es->state = ES_RECEIVED;

    /* store reference to incoming pbuf (chain) */
    es->p = p;

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(tpcb, tcp_server_sent);

    /* handle the received data */
    tcp_server_handle(tpcb, es);

    ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {
    /* more data received from client and previous data has been already sent*/
    if(es->p == NULL)
    {
      es->p = p;

      /* handle the received data */
      tcp_server_handle(tpcb, es);
    }
    else
    {
      struct pbuf *ptr;

      /* chain pbufs to the end of what we recv'ed previously  */
      ptr = es->p;
      pbuf_chain(ptr,p);
    }
    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {
    /* unknown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs.
  * @param  arg: pointer on argument parameter
  * @param  err: not used
  * @retval None
  */
static void tcp_server_error(void *arg, err_t err)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(err);

  es = (struct tcp_server_struct *)arg;
  if (es != NULL)
  {
    /*  free es structure */
    mem_free(es);
  }
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  es = (struct tcp_server_struct *)arg;
  if (es != NULL)
  {
    if (es->p != NULL)
    {
      tcp_sent(tpcb, tcp_server_sent);
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_server_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /*  close tcp connection */
        tcp_server_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  None
  * @retval None
  */
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(len);

  es = (struct tcp_server_struct *)arg;
  es->retries = 0;

  if(es->p != NULL)
  {
    /* still got pbufs to send */
    tcp_sent(tpcb, tcp_server_sent);
    tcp_server_send(tpcb, es);

  }
  else
  {
    /* if no more data to send and client closed connection*/
    if(es->state == ES_CLOSING)
      tcp_server_connection_close(tpcb, es);
  }
  return ERR_OK;
}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on _state structure
  * @retval None
  */
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) &&
         (es->p != NULL) &&
         (es->p->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

    if (wr_err == ERR_OK)
    {
      u16_t plen;
      u8_t freed;

      plen = ptr->len;

      /* continue with next pbuf in chain (if any) */
      es->p = ptr->next;

      if(es->p != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p);
      }

     /* chop first pbuf from chain */
      do
      {
        /* try hard to free pbuf */
        freed = pbuf_free(ptr);
      }
      while(freed == 0);
     /* we can read more data now */
     tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }

  }
}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on _state structure
  * @retval None
  */
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  /* delete es structure */
  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
}

/* Handle the incoming TCP Data */

static void tcp_server_handle (struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

	struct tcp_server_struct *esTx;
	esTx = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
	/* get the Remote IP */
	ip4_addr_t inIP = tpcb->remote_ip;
	uint16_t inPort = tpcb->remote_port;

	/* Extract the IP */
	char *remIP = ipaddr_ntoa(&inIP);

	esTx->state = es->state;
	esTx->pcb = es->pcb;
	esTx->p = es->p;

	//char buf[100];
	memset (buf, '\0', 100);

	strncpy(buf, (char *)es->p->payload, es->p->tot_len);
	//UART_Log(buf);

	buf2[0] = buf[0];
	buf2[1] = buf[1];
	buf2[2] = buf[2];
	buf2[3] = buf[3];
	buf2[4] = buf[4];
	buf2[5] = buf[5];
	//UART_Log(buf2);

	if(strncmp(buf2,"TM0000",6)==0){

		PAL_00[10] = ST_RFID_00[20];
		PAL_00[12] = ST_RFID_00[22];
		PAL_00[13] = ST_RFID_00[23];

		esTx->p->payload = (void *)PAL_00;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_00);
		esTx->p->len = strlen (PAL_00);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0001",6)==0){
		//uint8_t PAL_01[14] = {"PAL_Linia_X_XX"};
		PAL_01[10] = ST_RFID_01[20];
		PAL_01[12] = ST_RFID_01[22];
		PAL_01[13] = ST_RFID_01[23];

		esTx->p->payload = (void *)PAL_01;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_01);
		esTx->p->len = strlen (PAL_01);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0002",6)==0){
		//uint8_t PAL_02[14] = {"PAL_Linia_X_XX"};
		PAL_02[10] = ST_RFID_02[20];
		PAL_02[12] = ST_RFID_02[22];
		PAL_02[13] = ST_RFID_02[23];

		esTx->p->payload = (void *)PAL_02;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_02);
		esTx->p->len = strlen (PAL_02);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0003",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_03[10] = ST_RFID_03[20];
		PAL_03[12] = ST_RFID_03[22];
		PAL_03[13] = ST_RFID_03[23];

		esTx->p->payload = (void *)PAL_03;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_03);
		esTx->p->len = strlen (PAL_03);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0004",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_04[10] = ST_RFID_04[20];
		PAL_04[12] = ST_RFID_04[22];
		PAL_04[13] = ST_RFID_04[23];

		esTx->p->payload = (void *)PAL_04;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_04);
		esTx->p->len = strlen (PAL_04);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0005",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_05[10] = ST_RFID_05[20];
		PAL_05[12] = ST_RFID_05[22];
		PAL_05[13] = ST_RFID_05[23];

		esTx->p->payload = (void *)PAL_05;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_05);
		esTx->p->len = strlen (PAL_05);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0006",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_06[10] = ST_RFID_06[20];
		PAL_06[12] = ST_RFID_06[22];
		PAL_06[13] = ST_RFID_06[23];

		esTx->p->payload = (void *)PAL_06;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_06);
		esTx->p->len = strlen (PAL_06);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0007",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_07[10] = ST_RFID_07[20];
		PAL_07[12] = ST_RFID_07[22];
		PAL_07[13] = ST_RFID_07[23];

		esTx->p->payload = (void *)PAL_07;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_07);
		esTx->p->len = strlen (PAL_07);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0008",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_08[10] = ST_RFID_08[20];
		PAL_08[12] = ST_RFID_08[22];
		PAL_08[13] = ST_RFID_08[23];

		esTx->p->payload = (void *)PAL_08;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_08);
		esTx->p->len = strlen (PAL_08);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0009",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_09[10] = ST_RFID_09[20];
		PAL_09[12] = ST_RFID_09[22];
		PAL_09[13] = ST_RFID_09[23];

		esTx->p->payload = (void *)PAL_09;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_09);
		esTx->p->len = strlen (PAL_09);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0010",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_10[10] = ST_RFID_10[20];
		PAL_10[12] = ST_RFID_10[22];
		PAL_10[13] = ST_RFID_10[23];

		esTx->p->payload = (void *)PAL_10;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_10);
		esTx->p->len = strlen (PAL_10);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0011",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_11[10] = ST_RFID_11[20];
		PAL_11[12] = ST_RFID_11[22];
		PAL_11[13] = ST_RFID_11[23];

		esTx->p->payload = (void *)PAL_11;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_11);
		esTx->p->len = strlen (PAL_11);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0012",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_12[10] = ST_RFID_12[20];
		PAL_12[12] = ST_RFID_12[22];
		PAL_12[13] = ST_RFID_12[23];

		esTx->p->payload = (void *)PAL_12;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_12);
		esTx->p->len = strlen (PAL_12);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0013",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_13[10] = ST_RFID_13[20];
		PAL_13[12] = ST_RFID_13[22];
		PAL_13[13] = ST_RFID_13[23];

		esTx->p->payload = (void *)PAL_13;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_13);
		esTx->p->len = strlen (PAL_13);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0014",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_14[10] = ST_RFID_14[20];
		PAL_14[12] = ST_RFID_14[22];
		PAL_14[13] = ST_RFID_14[23];

		esTx->p->payload = (void *)PAL_14;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_14);
		esTx->p->len = strlen (PAL_14);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}
	else if(strncmp(buf2,"TM0015",6)==0){
		//uint8_t PAL_03[14] = {"PAL_Linia_X_XX"};
		PAL_15[10] = ST_RFID_15[20];
		PAL_15[12] = ST_RFID_15[22];
		PAL_15[13] = ST_RFID_15[23];

		esTx->p->payload = (void *)PAL_15;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (PAL_15);
		esTx->p->len = strlen (PAL_15);

		tcp_server_send(tpcb, esTx);
		pbuf_free(es->p);
		}




	else
	{
		uint8_t ERR_str[3] = {"ERR"};
		UART_Log("ERR TCP Command");
		//strcat (buf, "+ Hello from TCP SERVER1111111\n");


		esTx->p->payload = (void *)ERR_str;
		esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (ERR_str);
		esTx->p->len = strlen (ERR_str);

		tcp_server_send(tpcb, esTx);


		pbuf_free(es->p);
}
}
