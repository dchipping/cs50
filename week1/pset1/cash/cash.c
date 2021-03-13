#include <stdio.h>
#include <cs50.h>
#include <math.h>

int getCents(float cash);
int numberOfCents(int cents);
float getValidCash(void);

int main(void)
{
    float validCash = getValidCash();
    int cents = getCents(validCash);
    //printf("Cents Input: %i\n", cents);
    printf("%i\n", numberOfCents(cents));
}

// Converts the change into integer
int getCents(float cash)
{
    int cents = round(cash * 100);
    return cents;
}

// Simple greedy algorithm to sort through change
int numberOfCents(int cents)
{
    int counter = 0;
    while (cents > 0)
    {
        if (cents - 25 >= 0)
        {
            cents -= 25;
            counter++;
        }
        else if (cents - 10 >= 0)
        {
            cents -= 10;
            counter++;
        }
        else if (cents - 5 >= 0)
        {
            cents -= 5;
            counter++;
        }
        else
        {
            cents -= 1;
            counter++;
        }
    }
    return counter;
}


// Ensures inputs are 0.01 or more
float getValidCash(void)
{
    float cash = 0;
    do
    {
        cash = get_float("How much cash do you have? \n");
    }
    while (getCents(cash) < 0);
    return cash;
}