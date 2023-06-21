/*
 * switchSounds.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */


#include "stm32f1xx_hal.h"
#include "switchSounds.h"
#include "main.h"
#include <string.h>
#include "quectelCommands.h"
#include <stdlib.h>

#define versionAdress 0x0800BFF0
#define resetAddress 0x08000000
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define currentTerminal 164522975789130

void AppruveSound() {
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(80);
}
void RelaySwitch() {
	HAL_GPIO_TogglePin(GPIOB, RELAY_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(GPIOB, RELAY_Pin);

}
void ErrorSound() {
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);

}
void CardReadSound() {
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
	HAL_Delay(50);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);

}

void takeData(uint8_t* data, int length, uint8_t* dataToDisplay){
	int i,j,l;
	for(i =5; i< length; i++){
	  if(data[i] == '<')
	  {
		  break;
	  }
	}
	for(j = i; j< length; j++){
	  if(data[j] == ',')
		 {
			  break;
		 }
	}

	for(l = j; l< length; l++){
	  if(data[l] == '!')
		 {
			  break;
		 }
	}

	strncpy((char *)dataToDisplay, (char *)data + j + 1, l - j - 1);


}

void insert(uint8_t* main){

    int lenght = strlen((char*)main);
    char* operationTypePtr = strstr((char*)main, "\"payment");
    char* value = strchr((char*)operationTypePtr, 'p');
    int index = value - (char*)main;


    for(int i = lenght+4; i>index; i--){
        main[i] = main[i - 4];
    }
    char* valueStart = strchr((char*)operationTypePtr, '\"') + 1;
    strncpy((char*)valueStart, "makepayment", 11);

}

int takeStatus(uint8_t* data, int length){
	int i;
	uint8_t STATUSSTR[3];
	for(i =5; i< length; i++){
	  if(data[i] == '<')
	  {
		  break;
	  }
	}
	STATUSSTR[0] = data[i+1];
	STATUSSTR[1] = data[i+2];
	STATUSSTR[2] = data[i+3];
	if(STATUSSTR[0] == 'E' && STATUSSTR[1] == 'R' && STATUSSTR[2] == 'A'){
		jumpToAddress(resetAddress);
	}
	int Status = atoi((char*)STATUSSTR);
	return Status;

}
