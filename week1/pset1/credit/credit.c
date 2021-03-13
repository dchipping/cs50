#include <stdio.h>
#include <cs50.h>
#include <math.h>

/*
Example No.
10 98 76 54 32 10
 2 42 13 60 20 34

American Express uses 15-digit numbers,
MasterCard uses 16-digit numbers,
Visa uses 13- and 16-digit numbers.

American Express numbers start with 34 or 37;
MasterCard numbers start with 51, 52, 53, 54, or 55
Visa numbers start with 4.
*/

bool checkSum(long number);
int digitSelector(int number, int position);
int getLength(long number);
int get2StartDigits(long number);

int main(void)
{
    long cardNumber = get_long("Enter a card number: ");

    // First check the validity of the checkSum using Luhn's Algorithm
    if (checkSum(cardNumber))
    {
        int startDigits = get2StartDigits(cardNumber);
        int length = getLength(cardNumber);

        // Checks if conditions of starting digits and length are meet
        if ((startDigits == 34 || startDigits == 37) && length == 15)
        {
            printf("AMEX\n");
        }
        else if ((51 <= startDigits && startDigits <= 55) && length == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((40 <= startDigits && startDigits <= 49) && (length == 13 || length == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

bool checkSum(long number)
{
    int evenSum = 0;
    int odds = 0;
    int length = getLength(number);

    // Iterate through each digit of the number
    for (int i = 1; i <= length; i++)
    {
        // Each iteration remove the first digit and evaluate
        int digit = number % 10;
        number /= 10;
        if (i % 2 == 0)
        {
            // Multiplies every other digit by 2
            int twice = digit * 2;
            do
            {
                // Seperates the doubled digit into seperate digits
                evenSum += twice % 10;
                twice /= 10;
            }
            while (twice != 0);
        }
        else
        {
            // Records every other digit starting from the first digit
            odds += digit;
        }
    }

    // Sum odd numbers to the evenSum and evaluates validity
    int sum = evenSum + odds;
    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


int getLength(long number)
{
    int counter = 0;
    do
    {
        // Divide by 10 untill all digits iterated
        number /= 10;
        counter++;
    }
    while (number > 0);
    return counter;
}

int get2StartDigits(long number)
{
    int counter = 0;
    int length = getLength(number);
    do
    {
        // Divide by 10 untill two digits remain
        number /= 10;
        counter++;
    }
    while (counter < length - 2);
    return number;
}
