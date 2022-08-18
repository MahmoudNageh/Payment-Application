#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "card.h"

#define MaxCardHolderName 24
#define MinCardHolderName 20
#define ExpiryDateStrLen  5
#define MaxCardPAN 19
#define MinCardPAN 16

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    printf("Please enter the Card Holder's Name: ");

    scanf_s("%[^\n]s",&cardData->cardHolderName,MaxCardHolderName + 1); // scanf_s prevents overflow 

    // Check on the Card Holder's Name format
    if (strlen(cardData->cardHolderName) < MinCardHolderName || (cardData->cardHolderName) == NULL)
        return WRONG_NAME;
    

    printf("Hi, %s\n", cardData->cardHolderName);

    return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
   printf("Please enter the card Expiry Date in the format of MM/YY: ");

   // Using <ctype.h> to check on the Expiry Date format
   scanf_s("%s", &cardData->cardExpirationDate, ExpiryDateStrLen + 1);

    if (strlen(cardData->cardExpirationDate) != ExpiryDateStrLen || (cardData->cardExpirationDate) == NULL )
        return WRONG_EXP_DATE;

    if (!isdigit(cardData->cardExpirationDate[0]) || !isdigit(cardData->cardExpirationDate[1])) 
        return WRONG_EXP_DATE;

    if ((cardData->cardExpirationDate[2]) != '/')                                               
        return WRONG_EXP_DATE;

    if (!isdigit(cardData->cardExpirationDate[3]) || !isdigit(cardData->cardExpirationDate[4])) 
        return WRONG_EXP_DATE;

    printf("Your Card Expiry Date is:%s\n", cardData->cardExpirationDate);

    return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    printf("Please enter the Card PAN: ");

    scanf_s("%s", &cardData->primaryAccountNumber, MaxCardPAN + 1);

    // Check on the Card PAN format
    if (strlen(cardData->primaryAccountNumber) < MinCardPAN || (cardData->cardHolderName) == NULL)
    {
        return WRONG_PAN;
    }
    printf("Your Card PAN is:%s\n", cardData->primaryAccountNumber);
    return OK;
}
