/*
 * quectelCommands.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */


#include "quectelCommands.h"
#include "FLASH_PAGE_F1.h"



uint8_t mainBuffer[128];
//sprintf((char*) MQTT_CHECK_DATA, "{\"operationType\":\"check\",\"content\":{\"terminalID\":\"164522982240839\",\"firmwareVersion\":%lX}}",version);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
int doPost(uint8_t* postData) {
	uint8_t RXBuffer[128];
	uint8_t statusString[5];
	uint8_t dataSizeTimeOut[64];
	uint8_t size = strlen(((char*) postData)) - 2;
	sprintf((char*) dataSizeTimeOut, "at+qhttppost=%d,80,80\r\n", size);
	//HD44780_Clear();
	//HD44780_SetCursor(0, 0);
	//HD44780_PrintStr("  PLEASE WAIT!");
	//HAL_UART_Transmit(&huart2, dataSizeTimeOut, 64, 1000);
	HAL_UART_Transmit(&huart2, postData, size + 2, 100);

	sprintf((char*)mainBuffer, ATQHTTPCFG0);
	HAL_UART_Transmit(&huart1, mainBuffer, 27, 100); // send AT+QHTTPCFG="contextid",1
	HAL_UART_Receive(&huart1, RXBuffer, 31, 100);   // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 31, 100);  // forward answer

	sprintf((char*)mainBuffer, ATQHTTPCFG1);
	HAL_UART_Transmit(&huart1, mainBuffer, 26, 100); // send AT+QHTTPCFG="sslctxid",1
	HAL_UART_Receive(&huart1, RXBuffer, 31, 100);   // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 31, 100);  // forward answer

	sprintf((char*)mainBuffer, ATQSSLCFG);
	HAL_UART_Transmit(&huart1, mainBuffer, 29, 100); //send AT+QSSLCFG="sslversion",1,1
	HAL_UART_Receive(&huart1, RXBuffer, 34, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 34, 100);  // forward answer

	sprintf((char*)mainBuffer, ATQHTTPCFG);
	HAL_UART_Transmit(&huart1, mainBuffer, 29, 100); //send AT+QHTTPCFG="contenttype",1
	HAL_UART_Receive(&huart1, RXBuffer, 34, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 34, 100); // forward answer

	sprintf((char*)mainBuffer, ATQHTTPURL);
	HAL_UART_Transmit(&huart1, mainBuffer, 19, 100); //send at+qhttpurl=57,80
	HAL_UART_Receive(&huart1, RXBuffer, 28, 100);  // receive answer
	HAL_UART_Transmit(&huart2, RXBuffer, 28, 100); // forward answer

	sprintf((char*)mainBuffer, URL);
	HAL_UART_Transmit(&huart1, mainBuffer, 59, 100); // https://us-central1-lift-os.cloudfunctions.net/payForRide
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
	 static uint8_t Error[40];
	sprintf((char*)mainBuffer, ATQHTTPREAD);
	HAL_UART_Transmit(&huart1, mainBuffer, 17, 300); // forward answer
	HAL_UART_Receive(&huart1, mainBuffer, 50, 300);  // receive answer
	//HAL_UART_Transmit(&huart2, RXBuffer, 40, 300); // forward answer
	int L = 0;
	for (uint8_t i = 29; i < 60; i++) {
		if (mainBuffer[i] == ';')
			break;
		Error[L++] = mainBuffer[i];
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

void checkQueqtelWithNoEcho (uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
    sprintf((char*)mainBuffer, AT);

	while (status == 0) {
			HAL_UART_Transmit(&huart1, mainBuffer, 4, 200);
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
	uint8_t GSM_signal_error_counter = 0; // for version check

	sprintf((char*)mainBuffer, ATCOPS);
	while(status == 0){
			HAL_UART_Transmit(&huart1, mainBuffer, 10, 200);
			HAL_UART_Receive(&huart1, RXBuffer, 1000, 1000);
			status = checkCommand(RXBuffer, (uint8_t*)"MAGT");
			if(!status){
				GSM_signal_error_counter++;
			}
			if(GSM_signal_error_counter >= 30){
				sprintf((char*)mainBuffer, ATQPOWD);
				HAL_UART_Transmit(&huart1, mainBuffer, 12, 200);
				HAL_NVIC_SystemReset();
			}

			if(HAL_GetTick() - timeOut >= 60000){
				HAL_NVIC_SystemReset();
			}
		}



}
void MQTTRecMod(uint8_t *RXBuffer){
	uint8_t status_err = 0;
	uint8_t status_ok = 0;
	uint8_t GPRS_signal_error_counter = 0;
	int timeOut = HAL_GetTick();
	sprintf((char*)mainBuffer, ATQMTCFG);
	while(status_err == 0 || status_ok == 0){
		HAL_UART_Transmit(&huart1, mainBuffer, 29, 100);
		HAL_UART_Receive(&huart1, RXBuffer, 10, 100);
		status_ok = checkCommand(RXBuffer, (uint8_t*)"OK");
		status_err =  checkCommand(RXBuffer, (uint8_t*)"ERROR");
		if(GPRS_signal_error_counter >= 10){
			sprintf((char*)mainBuffer, ATQPOWD);
			HAL_UART_Transmit(&huart1, mainBuffer, 12, 200);
			HAL_NVIC_SystemReset();
		}
		if(status_err || status_ok){
			break;
		}
		else{
			GPRS_signal_error_counter++;
		}

		if(HAL_GetTick() - timeOut >= 30000){
			HAL_NVIC_SystemReset();
		}
	}
}

void MQTTOpenPort(uint8_t *RXBuffer){
	uint8_t status_err_1 = 0;
	uint8_t status_err_2 = 0;
	uint8_t GPRS_signal_error_counter = 0;
	int timeOut = HAL_GetTick();
	sprintf((char*)mainBuffer, ATQMTOPEN);
	while(status_err_1 == 0 || status_err_2 == 0){

		HAL_UART_Transmit(&huart1, mainBuffer, 34, 100);
		HAL_UART_Receive(&huart1, RXBuffer, 21, 4000);
		status_err_1 =  checkCommand(RXBuffer, (uint8_t*)"+QMTOPEN: 0,0");
		status_err_2 =  checkCommand(RXBuffer, (uint8_t*)"+QMTOPEN: 0,2");
		if(GPRS_signal_error_counter >= 3){
			sprintf((char*)mainBuffer, ATQPOWD);
			HAL_UART_Transmit(&huart1, mainBuffer, 12, 200);
			HAL_NVIC_SystemReset();
		}
		if(status_err_2 || status_err_1){
			break;
		}
		else{
			GPRS_signal_error_counter++;
		}
		if(HAL_GetTick() - timeOut >= 30000){
			HAL_NVIC_SystemReset();
		}

	}
}

void MQTTConnect(uint8_t *RXBuffer){
	uint8_t status = 0;
	int timeOut = HAL_GetTick();
	sprintf((char*)mainBuffer, ATQMTCONN);
	while(status == 0){

		HAL_UART_Transmit(&huart1, mainBuffer, 49, 100);
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
	sprintf((char*)mainBuffer, ATQMTSUB);

		HAL_UART_Transmit(&huart1, mainBuffer, 35, 100);
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
	uint8_t MQTT_PUB[60];
	sprintf((char*) MQTT_PUB, "AT+QMTPUBEX=0,0,0,0,\"liftos08765546789\",%d\r\n", (length));


		HAL_UART_Transmit_IT(&huart1, MQTT_PUB, sizeof MQTT_PUB / sizeof MQTT_PUB[0]);

		//HAL_UART_Receive(&huart1, RXBuffer, 10, 100);

		//status = checkCommand(RXBuffer, (uint8_t*)">");
		HAL_Delay(30);

}

uint8_t checkMQTTCon(uint8_t *RXBuffer){
	uint8_t status = 0;
	sprintf((char*)mainBuffer, ATQMTOPEN1);

	while(status == 0){

		HAL_UART_Transmit(&huart1, mainBuffer, 13, 100);

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








