#include <stdio.h>
#include "server.h"
#include <string.h>
#include "../Card/card.h"

ST_accountsDB_t Accounts[255] = { { 1200.00, "5399353562786336"},{ 11.00, "5399353562786344"},{ 100.00, "5399353562786443"} };
ST_transaction_t Transactions[255] = { 0 };

uint32_t TransactionNumber = 0;

uint16_t acc_num = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t vaildAccount_f;
	EN_serverError_t amountAvailable_f;
	EN_serverError_t savingTransaction_f;

	if (vaildAccount_f = isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
	{
		return DECLINED_STOLEN_CARD;
	}

	if (amountAvailable_f = isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}

	if (savingTransaction_f = saveTransaction(transData) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}

	printf("\nYour Balance before The Transaction : %.2f", Accounts[acc_num].balance);

	Accounts[acc_num].balance = (Accounts[acc_num].balance - transData->terminalData.transAmount);

	printf("\nTou Balance After The Transaction : %.2f", Accounts[acc_num].balance);

	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	for (acc_num = (0 + acc_num); acc_num < 255; acc_num++)
	{
		if (strcmp (cardData->primaryAccountNumber,Accounts[acc_num].primaryAccountNumber) == 0)
		{
			return OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount < Accounts[acc_num].balance)
	{
		return OK;
	}
	return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transData->transactionSequenceNumber = 0 + TransactionNumber;
	if (transData->transactionSequenceNumber < 255)
	{
		Transactions[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
		Transactions[transData->transactionSequenceNumber].terminalData = transData->terminalData;
		Transactions[transData->transactionSequenceNumber].transState = transData->transState;
		Transactions[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber + 1;

		TransactionNumber++;
		return OK;
	}
	return SAVING_FAILED;
}
