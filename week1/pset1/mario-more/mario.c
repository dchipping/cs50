# include <stdio.h>
# include <cs50.h>

void lineMaker(int spaceNumber, int hashNumber);
void spaceMaker(int spaces);
void hashMaker(int hashes);
int askPositiveHeight(void);

// Asks user how heigh to build and runs loop till height reached
int main(void)
{
    int stairsHeight = askPositiveHeight();
    for (int k = 1; k <= stairsHeight; k++)
    {
        lineMaker(stairsHeight - k, k);
    }
}

// Makes desired pattern of one line (e.g. ..##.##..)
void lineMaker(int indentNumber, int hashNumber)
{
    spaceMaker(indentNumber);
    hashMaker(hashNumber);
    printf("  ");
    hashMaker(hashNumber);
    spaceMaker(indentNumber);
    printf("\n");
}

// Concatinates 'spaces' number of spaces together
void spaceMaker(int spaces)
{
    // Counting down from spaceNumber in each line
    for (int i = 0; i < spaces; i++)
    {
        printf(" ");
    }
}

// Concatinates 'hashes' number of hashes together
void hashMaker(int hashes)
{
    for (int j = 0; j < hashes; j++)
    {
        printf("#");
    }
}

// Ensures only integers between 1 to 8
int askPositiveHeight(void)
{
    int n = 0;
    do
    {
        n = get_int("Height: \n");
    }
    while (n < 1 || n > 8);
    return n;
}