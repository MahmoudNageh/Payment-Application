#include <stdio.h>
#include "terminal.h"
#include <time.h>
#include <string.h>

#define CardYearTens  3
#define CardYearOnes  4
#define DateYearTens  8
#define DaterYearOnes 9

#define CardMonTens  0
#define CardMonOnes  1
#define DateMonTens  3
#define DaterMonOnes 4

#define maxAmount 1000;

// DD/MM/YYYY format defines
#define _D      0
#define D_      1
#define DD_YY   2
#define _M      3
#define M_      4
#define MM_YYYY 5
#define _YYY    6
#define Y_YY    7
#define YY_Y    8
#define YYY_    9

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    uint8_t currDay[3];
    uint8_t currMon[3];
    uint8_t currYear[5];

    // Get System Date

    time_t date_time = time(NULL);                  // time_t is a long long typedef in time.h
    struct tm currDate = *localtime(&date_time);    // cuurDate is a struct that contains all the system Date and time info, 

    //sprintf sends formated output to a string
    sprintf(currDay, "%d", currDate.tm_mday);           // Get Day from system and store it in currDay
    sprintf(currMon, "%d", currDate.tm_mon + 1);        // Get Month from system and store it in currMon 
    sprintf(currYear, "%d", currDate.tm_year + 1900);   //Get Year from system and store it in currYear
   
    // Should be stored in this format - > DD/MM/YYYY

    // Storing DD
    if ((currDate.tm_mday) < 10)
    {
        (termData->transactionDate[_D]) = '0';
        (termData->transactionDate[D_]) = currDay[0];
    }
    else {
        (termData->transactionDate[_D]) = currDay[0];
        (termData->transactionDate[D_]) = currDay[1];
    }

    (termData->transactionDate[DD_YY]) = '/';

    // Storing MM
   if ((currDate.tm_mon + 1) < 10)
    {
        (termData->transactionDate[_M]) = '0';
        (termData->transactionDate[M_]) = currMon[0];

    }
   else {
       (termData->transactionDate[_M]) = currMon[0];
       (termData->transactionDate[M_]) = currMon[1];
   }

   // Storing YYYY
   (termData->transactionDate[MM_YYYY]) = '/';
   (termData->transactionDate[_YYY]) = currYear[0];
   (termData->transactionDate[Y_YY]) = currYear[1];
   (termData->transactionDate[YY_Y]) = currYear[2];
   (termData->transactionDate[YYY_]) = currYear[3];
   (termData->transactionDate[10]) = '\0';

   if (termData->transactionDate == NULL)
   {
       return WRONG_DATE;
   }
   printf("Date:%s\n", termData->transactionDate);

   return OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    // cardExpirationDate MM/YY      transactionDate DD/MM/YYYY

    if ((cardData.cardExpirationDate[CardYearTens]) < (termData.transactionDate[DateYearTens])) // Year tens digit check        MM/_Y <> DD/MM/_Y
        return EXPIRED_CARD;

    if ((cardData.cardExpirationDate[CardYearTens]) == (termData.transactionDate[DateYearTens]))
    {
        if ((cardData.cardExpirationDate[CardYearOnes]) < (termData.transactionDate[DaterYearOnes])) // Year ones digit check   MM/Y_ <> DD/MM/Y_
            return EXPIRED_CARD;

        if ((cardData.cardExpirationDate[CardYearOnes]) == (termData.transactionDate[DaterYearOnes]))
        {
            if ((cardData.cardExpirationDate[CardMonTens]) < (termData.transactionDate[DateMonTens])) // Month tens check       _M/YY <> DD/_M/YY
                return EXPIRED_CARD;

            if ((cardData.cardExpirationDate[CardMonTens]) == (termData.transactionDate[DateMonTens]))
            {
                if ((cardData.cardExpirationDate[CardMonOnes]) < (termData.transactionDate[DaterMonOnes])) // Month ones check   M_/YY <> DD/M_/YY
                    return EXPIRED_CARD;
            }
        }
    }
    printf("Your Card is NOT Expired\n");
    return OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    /* Check PAN validity with luhn algorithm:
     * Drop the rightmost digit (checksum)
     * Starting from the right most digit double the vaule of every other digit
     * sum the digits and that sum%10 should be 0
     */

    uint16_t sum = 0;
    uint16_t PANlen;
    uint16_t counter = 1;
    uint16_t PANdigits[20] = { 0 };

    PANlen = strlen(cardData->primaryAccountNumber);    // Get PAN length

    for (uint16_t i = PANlen-1 ; i > 0; i--)            
    {
        if (counter % 2 == 0)                          // Double every other digit
        {
            uint16_t temp = (cardData->primaryAccountNumber[i] - '0') * 2; 

            if ((temp / 10) > 0)                        // If the double value is a 2 digit number add them together
            {
                temp = (temp % 10) + 1;
                PANdigits[i] = temp;
            }
            else
            {
                PANdigits[i] = temp;
            }
        }
        else 
        {
            PANdigits[i] = (cardData->primaryAccountNumber[i] - '0');
        }
        counter++;
    }
    
    // The same as the above loop but for the first number ( index = 0 )
    if (counter % 2 == 0)
    {
        uint16_t temp = (cardData->primaryAccountNumber[0] - '0') * 2;;

        if ((temp / 10) > 0)
        {
            temp = (temp % 10) + 1;
            PANdigits[0] = temp;
        }
        else
        {
            PANdigits[0] = temp;
        }
    }
    else
    {
        PANdigits[0] = (cardData->primaryAccountNumber[0] - '0');
    }

    // Sum all the digits
    for (uint16_t i = 0; i < PANlen; i++)
    {
        sum += PANdigits[i];
    }
    

    // Check for Validity
    if (sum % 10 == 0)
    {
        printf("Valid card\n");
        return OK;
    }
    else
        return INVALID_CARD;
    
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    // Get Transaction Amount and it should be > than 0

    printf("Please enter the Transaction Amount: ");

    scanf("%f", &termData->transAmount);

    if ((termData->transAmount) <= 0)
        return INVALID_AMOUNT;
    
    printf("Your Transaction Amount is: %.2f\n", termData->transAmount);

    return OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    // Checks if the Transactions Amount is below the Max Amount

    if ((termData->transAmount) > (termData->maxTransAmount))
        return EXCEED_MAX_AMOUNT;

    return OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    // Sets Max Amount 

    termData->maxTransAmount = maxAmount;

    printf("Max Transaction Amount is : %.2f\n", termData->maxTransAmount);

    if ((termData->maxTransAmount) <= 0)
        return INVALID_MAX_AMOUNT;

    return OK;
}