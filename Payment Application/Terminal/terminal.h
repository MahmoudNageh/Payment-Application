#ifndef TERMINAL_H
#define TERMINAL_H

#include "../lib/std_types.h"
#include "../Card/card.h"

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	//OK,					//0
	WRONG_DATE = 1,			//1
	EXPIRED_CARD,			//2
	INVALID_CARD,			//3
	INVALID_AMOUNT,			//4
	EXCEED_MAX_AMOUNT,		//5
	INVALID_MAX_AMOUNT		//6
}EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t * cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t * termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t * termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t * termData);


#endif // !TERMINAL_H
