/*
 * quectelCommands.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */


#include "stm32f1xx_hal.h"
#include "quectelCommands.h"
#include "main.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

uint8_t dataSizeTimeOut[64];
uint8_t statusString[64];
uint8_t TXBuffer[] = "at\r\n";
uint8_t TXBuffer1[] = "AT+QHTTPCFG=\"contextid\",1\r\n";
uint8_t TXBuffer2[] = "AT+QHTTPCFG=\"sslctxid\",1\r\n";
uint8_t TXBuffer3[] = "AT+QSSLCFG=\"sslversion\",1,1\r\n";
uint8_t TXBuffer4[] = "AT+QHTTPCFG=\"contenttype\",1\r\n";
uint8_t TXBuffer5[] = "at+qhttpurl=57,80\r\n";
uint8_t TXBuffer6[] = "https://us-central1-lift-os.cloudfunctions.net/payForRide\r\n";
uint8_t TXBuffer7[] = "AT+QHTTPREAD=80\r\n";
uint8_t TXBuffer8[] = "AT+CGACT=1,1\r\n";
uint8_t TXBuffer9[] = "AT+CGPADDR=1\r\n";
uint8_t TXBuffer10[] = "AT+COPS?\r\n";
uint8_t config_recive_mod[] = "AT+QMTCFG=\"recv/mode\",0,0,1\r\n";
uint8_t MQTT_OPEN_PORT[] = "AT+QMTOPEN=0,\"165.22.25.19\",1883\r\n";
uint8_t MQTT_CONNECT[] = "AT+QMTCONN=0,0\r\n";
uint8_t MQTT_SUB_TO_TOPIC[] = "AT+QMTSUB=0,1,\"liftos08765546781\",2\r\n";
uint8_t MQTT_PUB[] = "AT+QMTPUBEX=0,0,0,0,\"liftos08765546789\",124\r\n";
uint8_t MQTT_CON_CHECK[] = "AT+QMTOPEN?\r\n";
uint8_t RXBuffer[128];

uint8_t payload[128];
uint8_t Error[64];


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
int doPost(uint8_t* postData) {

	uint8_t size = strlen(((char*) postData)) - 2;
	sprintf((char*) dataSizeTimeOut, "at+qhttppost=%d,80,80\r\n", size);
	//HD44780_Clear();
	//HD44780_SetCursor(0, 0);
	//HD44780_PrintStr("  PLEASE WAIT!");
	//HAL_UART_Transmit(&huart2, dataSizeTimeOut, 64, 1000);
	HAL_UART_Transmit(&huart2, postData, size + 2, 100);

	HAL_UART_Transmit(&huart1, TXBuffer1, 27, 100); // send AT+QHTTPCFG="contextid",1
	HAL_UART_Receive(&huart1, RXBuffer, 31, 100);   // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 31, 100);  // forward answer

	HAL_UART_Transmit(&huart1, TXBuffer2, 26, 100); // send AT+QHTTPCFG="sslctxid",1
	HAL_UART_Receive(&huart1, RXBuffer, 31, 100);   // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 31, 100);  // forward answer

	HAL_UART_Transmit(&huart1, TXBuffer3, 29, 100); //send AT+QSSLCFG="sslversion",1,1
	HAL_UART_Receive(&huart1, RXBuffer, 34, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 34, 100);  // forward answer

	HAL_UART_Transmit(&huart1, TXBuffer4, 29, 100); //send AT+QHTTPCFG="contenttype",1
	HAL_UART_Receive(&huart1, RXBuffer, 34, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 34, 100); // forward answer

	HAL_UART_Transmit(&huart1, TXBuffer5, 19, 100); //send at+qhttpurl=57,80
	HAL_UART_Receive(&huart1, RXBuffer, 28, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 28, 100); // forward answer

	HAL_UART_Transmit(&huart1, TXBuffer6, 59, 100); // https://us-central1-lift-os.cloudfunctions.net/payForRide
	HAL_UART_Receive(&huart1, RXBuffer, 7, 2000);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 7, 2000); // forward answer

	HAL_UART_Transmit(&huart1, dataSizeTimeOut, 23, 500); // at+qhttppost=%d,80,80
	HAL_UART_Receive(&huart1, RXBuffer, 33, 20000);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 33, 10000); // forward answer
	HAL_UART_Transmit(&huart1, postData, size + 2, 1000);
	HAL_UART_Receive(&huart1, RXBuffer, 30, 20000);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 30, 10000); // forward answer

	statusString[0] = RXBuffer[23];
	statusString[1] = RXBuffer[24];
	statusString[2] = RXBuffer[25];


	int Status = atoi((char*) statusString);
	return Status;

}


uint8_t* readIncomeData() {
	HAL_UART_Transmit(&huart1, TXBuffer7, 17, 300); // forward answer
	HAL_UART_Receive(&huart1, RXBuffer, 50, 300);  // receive answer
	//HAL_UART_Transmit(&huart2, RXBuffer, 40, 300); // forward answer
	int L = 0;
	for (uint8_t i = 29; i < 60; i++) {
		if (RXBuffer[i] == ';')
			break;
		Error[L++] = RXBuffer[i];
	}
	return Error;
}



uint8_t checkCommand(uint8_t* text, uint8_t* word) {
	uint8_t textLen = strlen((char*)text);
	uint8_t wordLen = strlen((char*)word);
	uint8_t i, j;

    for (i = 0; i <= textLen - wordLen; i++) {
        for (j = 0; j < wordLen; j++) {
            if (text[i+j] != word[j]) {
                break;
            }
        }
        if (j == wordLen) {
            return 1;  // word found
        }
    }

    return 0;  // word not found
}
void checkQueqtel (){
	int timeOut = HAL_GetTick();
	while (RXBuffer[5] != 'K' && RXBuffer[7] != 'K' && RXBuffer[6] != 'K') {
			HAL_UART_Transmit(&huart1, TXBuffer,
					sizeof TXBuffer / sizeof TXBuffer[0], 200);
			HAL_UART_Receive(&huart1, RXBuffer, 9, 500);
			HAL_UART_Transmit(&huart2, RXBuffer, 9, 100);
			printWaitDevaice();
			HAL_Delay(100);
			if(HAL_GetTick() - timeOut >= 25000){
				HAL_NVIC_SystemReset();
			}
		}
}

void ckeckQuectelSignal(){
	int timeOut = HAL_GetTick();
	while (RXBuffer[23] != 'M' && RXBuffer[22] != 'M' && RXBuffer[24] != 'M') {
				HAL_UART_Transmit(&huart1, TXBuffer10, sizeof TXBuffer10 / sizeof TXBuffer10[0], 200);
				HAL_UART_Receive(&huart1, RXBuffer, 45, 50);
				HAL_UART_Transmit(&huart2, RXBuffer, 45, 50);
				printWaitDevaice();
				HAL_Delay(100);
				if(HAL_GetTick() - timeOut >= 10000){
					HAL_NVIC_SystemReset();
				}
			}
}

void checkQueqtelWithNoEcho (uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	while (status == 0) {
			HAL_UART_Transmit(&huart1, TXBuffer, sizeof TXBuffer / sizeof TXBuffer[0], 200);
			HAL_UART_Receive(&huart1, RXBuffer, 6, 500);
			status = checkCommand(RXBuffer, (uint8_t*)"OK");
			if(status){
				break;
			}
			if(HAL_GetTick() - timeOut >= 10000){
				HAL_NVIC_SystemReset();
			}
		}
}

void ckeckQuectelSignalWithNoEcho(uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	while (status == 0) {
		HAL_UART_Transmit(&huart1, TXBuffer10, sizeof TXBuffer10 / sizeof TXBuffer10[0], 200);
		HAL_UART_Receive(&huart1, RXBuffer, 45, 100);
		status = checkCommand(RXBuffer, (uint8_t*)"MAGT");

		if(status){
			break;
		}
		if(HAL_GetTick() - timeOut >= 10000){
			HAL_NVIC_SystemReset();
		}
	}
}
void MQTTRecMod(uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	while(status == 0){
		HAL_UART_Transmit(&huart1, config_recive_mod, sizeof config_recive_mod / sizeof config_recive_mod[0], 100);
		HAL_UART_Receive(&huart1, RXBuffer, 10, 100);
		status = checkCommand(RXBuffer, (uint8_t*)"OK");
		if(status == 0){
			status = checkCommand(RXBuffer, (uint8_t*)"ERROR");
		}
		if(status){
			break;
		}
		if(HAL_GetTick() - timeOut >= 10000){
			HAL_NVIC_SystemReset();
		}
	}
}

void MQTTOpenPort(uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	while(status == 0){
		HAL_UART_Transmit(&huart1, MQTT_OPEN_PORT, sizeof MQTT_OPEN_PORT / sizeof MQTT_OPEN_PORT[0], 100);
		HAL_UART_Receive(&huart1, RXBuffer, 21, 4000);

		status = checkCommand(RXBuffer, (uint8_t*)"+QMTOPEN: 0,0");
		if(status == 0){
			status = checkCommand(RXBuffer, (uint8_t*)"+QMTOPEN: 0,2");
		}


		if(status){
			break;
		}

		if(HAL_GetTick() - timeOut >= 10000){
			HAL_NVIC_SystemReset();
		}

	}
}

void MQTTConnect(uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	while(status == 0){
		HAL_UART_Transmit(&huart1, MQTT_CONNECT, sizeof MQTT_CONNECT / sizeof MQTT_CONNECT[0], 100);
		HAL_UART_Receive(&huart1, RXBuffer, 9, 4000);

		status = checkCommand(RXBuffer, (uint8_t*)"+QMT");

		if(status == 0){
			status = checkCommand(RXBuffer, (uint8_t*)"ERROR");

		}

		if(status){
			break;
		}
		if(HAL_GetTick() - timeOut >= 10000){
			HAL_NVIC_SystemReset();
		}
	}
}

void MQTTSubToTopic(uint8_t *RXBuffer){
//	uint8_t status = 0;
//	while(status == 0){
		HAL_UART_Transmit(&huart1, MQTT_SUB_TO_TOPIC, sizeof MQTT_SUB_TO_TOPIC / sizeof MQTT_SUB_TO_TOPIC[0], 100);
//		HAL_UART_Receive(&huart1, RXBuffer, 25, 2000);
		HAL_Delay(500);
//		status = checkCommand(RXBuffer, (uint8_t*)"+QMTSUB: 0,1,0,2");
//
//		if(status == 0){
//			status = checkCommand(RXBuffer, (uint8_t*)"OK");
//
//		}
//		if(status){
//			break;
//		}
//	}
}

void MQTTPubToTopic(int length){
	//uint8_t status = 0;
	sprintf((char*) MQTT_PUB, "AT+QMTPUBEX=0,0,0,0,\"liftos08765546789\",%d\r\n", (length));


		HAL_UART_Transmit(&huart1, MQTT_PUB, sizeof MQTT_PUB / sizeof MQTT_PUB[0], 100);

		//HAL_UART_Receive(&huart1, RXBuffer, 10, 100);

		//status = checkCommand(RXBuffer, (uint8_t*)">");
		HAL_Delay(30);

}

uint8_t checkMQTTCon(uint8_t *RXBuffer){
	uint8_t status = 0;

	while(status == 0){

		HAL_UART_Transmit(&huart1, MQTT_CON_CHECK, sizeof MQTT_CON_CHECK / sizeof MQTT_CON_CHECK[0], 100);

		HAL_UART_Receive(&huart1, RXBuffer, 40, 500);

		status = checkCommand(RXBuffer, (uint8_t*)"165.22.25.19");

		if(status){
			break;
		}
		else{
			return 1;
		}

	}

	return 0;
}








