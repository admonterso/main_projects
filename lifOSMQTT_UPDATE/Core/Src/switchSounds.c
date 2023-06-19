/*
 * switchSounds.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */


#include "stm32f1xx_hal.h"
#include "switchSounds.h"
#include "main.h"

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
