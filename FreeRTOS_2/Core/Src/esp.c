/*
 * FUNCTIONS.c
 *
 *  Created on: Apr 27, 2020
 *      Author: Controllerstech
 */


#include <esp.h>
#include "UartRingbuffer.h"
#include "stdio.h"
#include "string.h"


void bufclr (char *buf)
{
	int len = strlen (buf);
	for (int i=0; i<len; i++) buf[i] = '\0';
}


/*****************************************************************************************************************************************/

void ESP_Init (char *SSID, char *PASSWD)
{
	char data[80];

	Ringbuf_init();

	Uart_sendstring("AT+RST\r\n");
	HAL_Delay(5000);

	/********* AT **********/
	Uart_sendstring("AT\r\n");
	while(!(Wait_for("AT\r\r\n\r\nOK\r\n")));


	/********* AT+CWMODE=1 **********/
	Uart_sendstring("AT+CWMODE=1\r\n");
	while (!(Wait_for("AT+CWMODE=1\r\r\n\r\nOK\r\n")));


	/********* AT+CWJAP="SSID","PASSWD" **********/
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data);
	while (!(Wait_for("WIFI GOT IP\r\n\r\nOK\r\n")));

	/********* AT+CIPMUX=0 **********/
	Uart_sendstring("AT+CIPMUX=0\r\n");
	while (!(Wait_for("AT+CIPMUX=0\r\r\n\r\nOK\r\n")));

}

void ESP_GetData (char *api_key, char *Total, char *Deaths, char *Recovered)
{
	char local_buf[100] = {0};
	char local_buf2[30] = {0};
	

	Uart_sendstring("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");
	
	HAL_Delay(500);
	sprintf (local_buf, "GET/apps/thinghttp/send_request?api_key=%s\r\n", api_key);
	int len = strlen (local_buf);
	
	sprintf (local_buf2, "AT+CIPSEND=%d\r\n", len);
	HAL_Delay(500);
	Uart_sendstring(local_buf2);
	//while (!(Wait_for(">")));
	
	bufclr(local_buf2);
	HAL_Delay(500);
	Uart_sendstring (local_buf);
	//while (!(Wait_for("SEND OK\r\n")));
	

	//while (!(Wait_for ("\">")));
	//while (!(Copy_upto ("</span>", local_buf2)));
	len = strlen (local_buf2);
	snprintf (Total, len-6,local_buf2);


	bufclr(local_buf2);
	//while (!(Wait_for("<span>")));
	//while (!(Copy_upto ("</span>", local_buf2)));
	len = strlen (local_buf2);
	snprintf (Deaths, len-6,local_buf2);

	bufclr(local_buf2);
	while (!(Wait_for("<span>")));
	while (!(Copy_upto ("</span>", local_buf2)));
	len = strlen (local_buf2);
	snprintf (Recovered, len-6,local_buf2);
}
