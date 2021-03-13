#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int numberOfLetters(string text);
int numberOfWords(string text);
int numberOfSentences(string text);
void ColemanLiauIndex(string text);

int main(void)
{
    string text = get_string("Text: ");
    ColemanLiauIndex(text);
}

int numberOfWords(string text)
{
    int counter = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            counter++;
        }
    }
    return ++counter;
}

int numberOfSentences(string text)
{
    int counter = 0;
    for (int j = 0, n = strlen(text); j < n; j++)
    {
        if (text[j] == '.' || text[j] == '!' || text[j] == '?')
        {
            counter++;
        }
    }
    return counter;
}

int numberOfLetters(string text)
{
    int counter = 0;
    for (int k = 0, n = strlen(text); k < n; k++)
    {
        int code = text[k];
        if ((65 <= code && code  <= 90)
            || (97 <= code && code <= 122))
        {
            counter++;
        }
    }
    return counter;
}

void ColemanLiauIndex(string text)
{
    int letters = numberOfLetters(text);
    int words = numberOfWords(text);
    int sentences = numberOfSentences(text);

    int index = round((0.0588 * 100 * letters / words) - (0.296 * 100 * sentences / words) - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}