/*
 * FUNCTIONS.h
 *
 *  Created on: Apr 27, 2020
 *      Author: Controllerstech
 */

#ifndef INC_ESP_H_
#define INC_ESP_H_


void ESP_Init (char *SSID, char *PASSWD);

void ESP_GetData (char *api_key, char *Total, char *Deaths, char *Recovered);


#endif /* INC_ESP_H_ */
