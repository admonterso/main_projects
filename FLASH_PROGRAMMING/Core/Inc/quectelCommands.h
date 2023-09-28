/*
 * quectelCommands.h
 *
 *  Created on: Apr 30, 2023
 *      Author: Dell
 */

#ifndef SRC_QUECTELCOMMANDS_H_
#define SRC_QUECTELCOMMANDS_H_

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <terminalConfig.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define at             "AT"
#define AT             (at "\r\n")
#define ATCOPS         (at "+COPS?\r\n")
#define ATQMTOPEN      (at "+QMTOPEN=0,\"167.71.57.18\",1883\r\n")
#define ATQMTSUB	   (at "+QMTSUB=0,1,\"" STR(currentTerminalSub) "\",2\r\n")
#define ATQMTCFG       (at "+QMTCFG=\"recv/mode\",0,0,1\r\n")
#define ATQMTCONN      (at "+QMTCONN=0,\"client2\",\"terminal\",\"41Q@BG2dIzIM\"\r\n")
#define ATQMTOPEN1     (at "+QMTOPEN?\r\n")
#define ATQHTTPCFG0    (at "+QHTTPCFG=\"contextid\",1\r\n")
#define ATQHTTPCFG1    (at "+QHTTPCFG=\"sslctxid\",1\r\n")
#define ATQSSLCFG      (at "+QSSLCFG=\"sslversion\",1,1\r\n")
#define ATQHTTPCFG     (at "+QHTTPCFG=\"contenttype\",1\r\n")
#define ATQHTTPURL     (at "+qhttpurl=57,80\r\n")
#define URL            "https://us-central1-lift-os.cloudfunctions.net/payForRide\r\n"
#define ATQHTTPREAD    (at "+QHTTPREAD=80\r\n")
#define ATCGACT        (at "+CGACT=1,1\r\n")
#define ATCGPADDR      (at "+CGPADDR=1\r\n")
#define ATQPOWD		   (at "+QPOWD=0\r\n")



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
