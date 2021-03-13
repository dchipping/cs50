#include <stdio.h>
#include <cs50.h>

void lineMaker(int spaceNumber, int hashNumber);
int askPositiveHeight(void);

int main(void)
{
    // Loop untill equal to height of stairs
    int stairsHeight = askPositiveHeight();
    for (int k = 1; k <= stairsHeight; k++)
    {
        lineMaker(stairsHeight - k, k);
    }
}

// Makes both spaces and hash to create line graphic
void lineMaker(int spaceNumber, int hashNumber)
{
    // Counting down from spaceNumber in each line
    for (int i = spaceNumber; i > 0; i--)
    {
        printf(" ");
    }
    // Counting up till hashNumber in each line
    for (int j = 0; j < hashNumber; j++)
    {
        printf("#");
    }
    printf("\n");
}

int askPositiveHeight(void)
{
    // Ensures only integers between 1 to 8
    int n = 0;
    do
    {
        n = get_int("Height: \n");
    }
    while (n < 1 || n > 8);
    return n;
}