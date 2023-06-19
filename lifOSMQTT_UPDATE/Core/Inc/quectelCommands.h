/*
 * quectelCommands.h
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */

#ifndef SRC_QUECTELCOMMANDS_H_
#define SRC_QUECTELCOMMANDS_H_
#include "stm32f1xx_hal.h"

int doPost(uint8_t* postData);
void checkQueqtel ();
void ckeckQuectelSignal();
uint8_t* readIncomeData();
void checkQueqtelWithNoEcho(uint8_t *RXBuffer);
void ckeckQuectelSignalWithNoEcho(uint8_t *RXBuffer);
void MQTTRecMod(uint8_t *RXBuffer);
void MQTTOpenPort(uint8_t *RXBuffer);
void MQTTConnect(uint8_t *RXBuffer);
void MQTTSubToTopic(uint8_t *RXBuffer);
void MQTTPubToTopic(int pubData);
uint8_t checkMQTTCon(uint8_t *RXBuffer);
uint8_t checkCommand(uint8_t* text, uint8_t* word);
#endif /* SRC_QUECTELCOMMANDS_H_ */
