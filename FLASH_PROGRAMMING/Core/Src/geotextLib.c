/*
 * geotextLib.c
 *
 *  Created on: May 6, 2023
 *      Author: Dell
 */
#include "GEOtextLib.h"
#include "liquidcrystal_i2c.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
void printItvirteba(uint8_t row, uint8_t column){
	uint8_t IGEO[8] = { I_GEO_MACRO };
	uint8_t TGEO[8] = { T_GEO_MACRO };
	uint8_t VGEO[8] = { V_GEO_MACRO };
	uint8_t RGEO[8] = { R_GEO_MACRO };
	uint8_t tGEO[8] = { t_GEO_MACRO };
	uint8_t EGEO[8] = { E_GEO_MACRO };
	uint8_t BGEO[8] = { B_GEO_MACRO };
	uint8_t AGEO[8] = { A_GEO_MACRO };

	HD44780_Clear();
	HD44780_CreateSpecialChar(1, IGEO); // i
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, TGEO); //t
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, VGEO);//v
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(1); // i
	HD44780_CreateSpecialChar(5, RGEO); //r
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);
	HD44780_CreateSpecialChar(6, tGEO); // t
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);
	HD44780_CreateSpecialChar(7, EGEO); //e
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(7);
	HD44780_CreateSpecialChar(8, BGEO); //b
	HD44780_SetCursor(column+7, row);
	HD44780_PrintSpecialChar(8);
	HD44780_CreateSpecialChar(4, AGEO); //a
	HD44780_SetCursor(column+8, row);
	HD44780_PrintSpecialChar(4);
	HD44780_PrintStr("!");
}

void printMzadVar(uint8_t row, uint8_t column){
	uint8_t MGEO[8] = { M_GEO_MACRO };
	uint8_t ZGEO[8] = { Z_GEO_MACRO };
	uint8_t AGEO[8] = { A_GEO_MACRO };
	uint8_t DGEO[8] = { D_GEO_MACRO };
	uint8_t VGEO[8] = { V_GEO_MACRO };
	uint8_t RGEO[8] = { R_GEO_MACRO };


	HD44780_Clear();
	HD44780_CreateSpecialChar(1, MGEO);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, ZGEO);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, AGEO);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(4, DGEO);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);
	HD44780_CreateSpecialChar(5, VGEO);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(5);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(6, RGEO);
	HD44780_SetCursor(column+7, row);
	HD44780_PrintSpecialChar(6);
	HD44780_SetCursor(column+8, row);
	HD44780_PrintStr("!");
	HD44780_SetCursor(6, 1);
	HD44780_PrintStr("-_-");
}
void printUcxoBaratia(uint8_t row, uint8_t column){
		uint8_t sGeo[8] = { s_GEO_MACRO };
		uint8_t xGeo[8] = { X_GEO_MACRO };
		uint8_t vGeo[8] = { V_GEO_MACRO };
		uint8_t aGeo[8] = { A_GEO_MACRO };
		uint8_t bGeo[8] = { B_GEO_MACRO };
		uint8_t rGeo[8] = { R_GEO_MACRO };
		uint8_t tGeo[8] = { T_GEO_MACRO };
		uint8_t iGeo[8] = { I_GEO_MACRO };
		HD44780_Clear();
		HD44780_CreateSpecialChar(1, sGeo);
		HD44780_SetCursor(column, row);
		HD44780_PrintSpecialChar(1);

		HD44780_CreateSpecialChar(2, xGeo);
		HD44780_SetCursor(column+1, row);
		HD44780_PrintSpecialChar(2);

		HD44780_CreateSpecialChar(3, vGeo);
		HD44780_SetCursor(column+2, row);
		HD44780_PrintSpecialChar(3);

		HD44780_CreateSpecialChar(4, aGeo);
		HD44780_SetCursor(column+3, row);
		HD44780_PrintSpecialChar(4);

		HD44780_CreateSpecialChar(5, bGeo);
		HD44780_SetCursor(column+5, row);
		HD44780_PrintSpecialChar(5);

		HD44780_SetCursor(column+6, row);
		HD44780_PrintSpecialChar(4);

		HD44780_CreateSpecialChar(6, rGeo);
		HD44780_SetCursor(column+7, row);
		HD44780_PrintSpecialChar(6);

		HD44780_SetCursor(column+8, row);
		HD44780_PrintSpecialChar(4);

		HD44780_CreateSpecialChar(7, tGeo);
		HD44780_SetCursor(column+9, row);
		HD44780_PrintSpecialChar(7);

		HD44780_CreateSpecialChar(8, iGeo);
		HD44780_SetCursor(column+10, row);
		HD44780_PrintSpecialChar(8);

		HD44780_SetCursor(column+11, row);
		HD44780_PrintSpecialChar(4);
		HD44780_PrintStr("!");

}

void printBlansiAraa(uint8_t row, uint8_t column){
	uint8_t bGeo[8] = { B_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t lGeo[8] = { L_GEO_MACRO };
	uint8_t nGeo[8] = { N_GEO_MACRO };
	uint8_t sGeo[8] = { s_GEO_MACRO };
	uint8_t iGeo[8] = { I_GEO_MACRO };
	uint8_t rGeo[8] = { R_GEO_MACRO };
	HD44780_Clear();
	HD44780_CreateSpecialChar(1, bGeo); //b
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);

	HD44780_CreateSpecialChar(2, aGeo); //a
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);

	HD44780_CreateSpecialChar(3, lGeo); // l
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);


	HD44780_SetCursor(column+3, row); //a
	HD44780_PrintSpecialChar(2);

	HD44780_CreateSpecialChar(4, nGeo); //n
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(4);

	HD44780_CreateSpecialChar(5, sGeo); //s
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(5);

	HD44780_CreateSpecialChar(6, iGeo); //i
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(6);

	HD44780_SetCursor(column+8, row); //a
	HD44780_PrintSpecialChar(2);

	HD44780_CreateSpecialChar(7, rGeo);
	HD44780_SetCursor(column+9, row);
	HD44780_PrintSpecialChar(7);


	HD44780_SetCursor(column+10, row);//a
	HD44780_PrintSpecialChar(2);

	HD44780_PrintSpecialChar(2);
	HD44780_PrintStr("!");
}

void printMiadetBarati(uint8_t row, uint8_t column){
	uint8_t mGeo[8] = { M_GEO_MACRO };
	uint8_t iGeo[8] = { I_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t dGeo[8] = { D_GEO_MACRO };
	uint8_t eGeo[8] = { E_GEO_MACRO };
	uint8_t tGeo[8] = { t_GEO_MACRO };
	uint8_t bGeo[8] = { B_GEO_MACRO };
	uint8_t rGeo[8] = { R_GEO_MACRO };

	HD44780_Clear();
	HD44780_CreateSpecialChar(1, mGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, iGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, aGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(4, dGeo);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);
	HD44780_CreateSpecialChar(5, eGeo);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);
	HD44780_CreateSpecialChar(6, tGeo);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);
	HD44780_CreateSpecialChar(7, bGeo);
	HD44780_SetCursor(column+7, row);
	HD44780_PrintSpecialChar(7);
	HD44780_SetCursor(column+8, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(8, rGeo);
	HD44780_SetCursor(column+9, row);
	HD44780_PrintSpecialChar(8);
	HD44780_SetCursor(column+10, row);
	HD44780_PrintSpecialChar(3);
	HD44780_SetCursor(column+11, row);
	HD44780_PrintSpecialChar(6);
	HD44780_SetCursor(column+12, row);
	HD44780_PrintSpecialChar(2);
	HD44780_SetCursor(column+2, row+1);
	HD44780_PrintStr("0.18 GEL");


}
void printShecdoma(uint8_t row, uint8_t column){
	uint8_t SGeo[8] = { S_GEO_MACRO };
	uint8_t eGeo[8] = { E_GEO_MACRO };
	uint8_t cGeo[8] = { C_GEO_MACRO };
	uint8_t dGeo[8] = { D_GEO_MACRO };
	uint8_t oGeo[8] = { O_GEO_MACRO };
	uint8_t mGeo[8] = { M_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };

	HD44780_Clear();
	HD44780_CreateSpecialChar(1, SGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, eGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, cGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(4, dGeo);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);
	HD44780_CreateSpecialChar(5, oGeo);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);
	HD44780_CreateSpecialChar(6, mGeo);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);
	HD44780_CreateSpecialChar(7, aGeo);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(7);
	HD44780_SetCursor(column+7, row);
	HD44780_PrintStr("!");
}

void printDaicadet(uint8_t row, uint8_t column){
	uint8_t dGeo[8] = { D_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t iGeo[8] = { I_GEO_MACRO };
	uint8_t cGeo[8] = { C_GEO_MACRO };
	uint8_t eGeo[8] = { E_GEO_MACRO };
	uint8_t tGeo[8] = { T_GEO_MACRO };

	HD44780_Clear();
	HD44780_CreateSpecialChar(1, dGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, aGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, iGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(4, cGeo);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(2);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(5, eGeo);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(5);
	HD44780_CreateSpecialChar(6, tGeo);
	HD44780_SetCursor(column+7, row);
	HD44780_PrintSpecialChar(6);
	HD44780_PrintStr("!");
}
void printVemzadebi(uint8_t row, uint8_t column){
	uint8_t vGeo[8] = { V_GEO_MACRO };
	uint8_t eGeo[8] = { E_GEO_MACRO };
	uint8_t mGeo[8] = { M_GEO_MACRO };
	uint8_t zGeo[8] = { Z_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t dGeo[8] = { D_GEO_MACRO };
	uint8_t bGeo[8] = { B_GEO_MACRO };
	uint8_t iGeo[8] = { I_GEO_MACRO };

	HD44780_Clear();
	HD44780_CreateSpecialChar(1, vGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);
	HD44780_CreateSpecialChar(2, eGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(3, mGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);
	HD44780_CreateSpecialChar(4, zGeo);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);
	HD44780_CreateSpecialChar(5, aGeo);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);
	HD44780_CreateSpecialChar(6, dGeo);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(2);
	HD44780_CreateSpecialChar(7, bGeo);
	HD44780_SetCursor(column+7, row);
	HD44780_PrintSpecialChar(7);
	HD44780_CreateSpecialChar(8, iGeo);
	HD44780_SetCursor(column+8, row);
	HD44780_PrintSpecialChar(8);
	HD44780_PrintStr("!");
}

void printBalansi(uint8_t row, uint8_t column){
	uint8_t bGeo[8] = { B_GEO_MACRO };
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t lGeo[8] = { L_GEO_MACRO };
	uint8_t nGeo[8] = { N_GEO_MACRO };
	uint8_t sGeo[8] = { s_GEO_MACRO };
	uint8_t iGeo[8] = { I_GEO_MACRO };


	HD44780_Clear();
	HD44780_CreateSpecialChar(1, bGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);

	HD44780_CreateSpecialChar(2, aGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);

	HD44780_CreateSpecialChar(3, lGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);

	HD44780_PrintSpecialChar(2);
	HD44780_SetCursor(column+3, row);

	HD44780_CreateSpecialChar(5, nGeo);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);

	HD44780_CreateSpecialChar(6, sGeo);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);

	HD44780_CreateSpecialChar(7, iGeo);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(7);

	HD44780_PrintStr(": ");
}
void printAxldeba(uint8_t row, uint8_t column){
	uint8_t aGeo[8] = { A_GEO_MACRO };
	uint8_t xGeo[8] = { X_GEO_MACRO };
	uint8_t lGeo[8] = { L_GEO_MACRO };
	uint8_t dGeo[8] = { D_GEO_MACRO };
	uint8_t eGeo[8] = { E_GEO_MACRO };
	uint8_t bGeo[8] = { B_GEO_MACRO };


	HD44780_Clear();
	HD44780_CreateSpecialChar(1, aGeo);
	HD44780_SetCursor(column, row);
	HD44780_PrintSpecialChar(1);

	HD44780_CreateSpecialChar(2, xGeo);
	HD44780_SetCursor(column+1, row);
	HD44780_PrintSpecialChar(2);

	HD44780_CreateSpecialChar(3, lGeo);
	HD44780_SetCursor(column+2, row);
	HD44780_PrintSpecialChar(3);

	HD44780_CreateSpecialChar(4, dGeo);
	HD44780_SetCursor(column+3, row);
	HD44780_PrintSpecialChar(4);

	HD44780_CreateSpecialChar(5, eGeo);
	HD44780_SetCursor(column+4, row);
	HD44780_PrintSpecialChar(5);

	HD44780_CreateSpecialChar(6, bGeo);
	HD44780_SetCursor(column+5, row);
	HD44780_PrintSpecialChar(6);

	HD44780_CreateSpecialChar(7, aGeo);
	HD44780_SetCursor(column+6, row);
	HD44780_PrintSpecialChar(7);

	HD44780_PrintStr("!");
}
