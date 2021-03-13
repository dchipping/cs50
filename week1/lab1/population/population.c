#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start = 0;
    while (start < 9)
    {
        start = get_int("Start size: ");
    }
    // TODO: Prompt for end size
    int end = 0;
    while (end < start)
    {
        end = get_int("End size: ");
    }
    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int pop = start;
    while (pop < end)
    {
        int born = pop / 3;
        int died = pop / 4;
        pop += (born - died);
        years++;
    }
    // TODO: Print number of years
    printf("Years: %i\n", years);
}