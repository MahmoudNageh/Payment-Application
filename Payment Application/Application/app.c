#include <stdio.h>
#include "../Card/card.h"
#include "../Server/server.h"
#include "app.h"

#define appStart main

void appStart(void)
{
	// Card Error Flags
	EN_cardError_t cardHolderName_f;
	EN_cardError_t cardExpiryDate_f;
	EN_cardError_t cardPAN_f;

	// Card Data Struct
	ST_cardData_t cardHolderData;
	ST_cardData_t* cardData = &cardHolderData;

	// Card Module : Get Card Info Name, Expiry Date, PAN
	if (cardHolderName_f = getCardHolderName(cardData) == WRONG_NAME)
	{
		printf("Wrong Name !");
		return 0;
	}

	if (cardExpiryDate_f = getCardExpiryDate(cardData) == WRONG_EXP_DATE)
	{
		printf("Wrong Expiry Date !");
		return 0;
	}

	if (cardPAN_f = getCardPAN(cardData) == WRONG_PAN)
	{
		printf("Wrong PAN !");
		return 0;
	}
 
	//--------------------------------------------//
	// Terminal Error Flags
	EN_terminalError_t vaildPAN_f;
	EN_terminalError_t transactionDate_f;
	EN_terminalError_t cardEpired_f;
	EN_terminalError_t maxAmount_f;
	EN_terminalError_t transactionAmount_f;
	EN_terminalError_t belowMaxAmount_f;

	// Terminal Data Struct
	ST_terminalData_t terminalData;
	ST_terminalData_t* termData = &terminalData;

	// Terminal Module: Get Card Data, Transaction Date, Check if card is expired, Get transaction amount and check if it exceeds the max amount
	if (vaildPAN_f = isValidCardPAN(cardData) == INVALID_CARD)
	{
		printf("Invalid card");
		return 0;
	}

	if (transactionDate_f = getTransactionDate(termData) == WRONG_DATE)
	{
		printf("Wrong Date !");
		return 0;
	}

	if (cardEpired_f = isCardExpired(cardHolderData, terminalData) == EXPIRED_CARD)
	{
		printf("Your Card is Expired !, Please Refer To The Bank To Get Your New Card");
		return 0;
	}

	if (maxAmount_f = setMaxAmount(termData) == INVALID_MAX_AMOUNT)
	{
		printf("Invalid Max Amount");
		return 0;
	}

	if (transactionAmount_f = getTransactionAmount(termData) == INVALID_AMOUNT)
	{
		printf("Invalid Amount !, Please Enter A Valid Amount");
		return 0;
	}

	if (belowMaxAmount_f = isBelowMaxAmount(termData) == EXCEED_MAX_AMOUNT)
	{
		printf("Exceeded Max Amount !, You Can't Withdraw More Than The Max Amount Mentioned Above");
		return 0;
	}
	
	//--------------------------------------------//
	// Transaction Error Flag
	EN_transState_t recieveTransaction_f;

	// Transaction Data Struct
	ST_transaction_t transactonData;
	ST_transaction_t* transData = &transactonData;

	// Server Module: Get Card Data, and Terminal Data
	transData->cardHolderData = cardHolderData;
	transData->terminalData = terminalData;

	recieveTransaction_f = recieveTransactionData(transData);

	switch (recieveTransaction_f)
	{
	case APPROVED:
		printf("\nTransaction was successfull, Thank You !!");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		saveTransaction(transData);
		printf("\nTransaction Declined Due To Insuffecient Fund");
		break;

	case DECLINED_STOLEN_CARD:
		saveTransaction(transData);
		printf("\nTransaction Declined This Card Is Stolen, Please Return It To The Nearest Branch");
		break;

	case INTERNAL_SERVER_ERROR:
		saveTransaction(transData);
		printf("\nTransaction Declined Due To Server Error, Please Try Again Later .. ");
		break;

	default:
		break;
	}
}
