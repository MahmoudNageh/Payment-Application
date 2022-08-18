#ifndef SERVER_H
#define SERVER_H

#include "../lib/std_types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"

typedef enum EN_transState_t
{
	APPROVED,					//0
	DECLINED_INSUFFECIENT_FUND, //1
	DECLINED_STOLEN_CARD,		//2
	INTERNAL_SERVER_ERROR		//3
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
	//OK,						//0
	SAVING_FAILED = 1,			//1
	TRANSACTION_NOT_FOUND,		//2
	ACCOUNT_NOT_FOUND,			//3
	LOW_BALANCE					//4
}EN_serverError_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);


extern ST_accountsDB_t Accounts[255];
extern ST_transaction_t Transactions[255];

#endif // !SERVER_H

