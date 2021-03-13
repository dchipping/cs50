#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

bool validInput(string key);
bool isAlphabet(string key);
bool isUppercase(char c);
bool isLowercase(char c);
bool everyLetterOnce(string key);
string toupperAll(string key);
void keyMapper(string key, string plainText);

int main(int argc, string argv[])
{
    if (argc != 2 || !validInput(argv[1]))
    {
        printf("Invalid Key\n");
        return 1;
    }
    string key = toupperAll(argv[1]);
    string plainText = get_string("plaintext: ");
    keyMapper(key, plainText);
    return 0;
}

bool validInput(string key)
{
    if (strlen(key) == 26 && isAlphabet(key))
    {
        key = toupperAll(key);
        return everyLetterOnce(key);
    }
    return false;
}

bool isAlphabet(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (isUppercase(key[i]) || isLowercase(key[i]))
        {
            return true;
        }
    }
    return false;
}

bool isUppercase(char c)
{
    if (65 <= c && c <= 90)
    {
        return true;
    }
    return false;
}

bool isLowercase(char c)
{
    if (97 <= c && c <= 122)
    {
        return true;
    }
    return false;
}

bool everyLetterOnce(string key)
{
    bool letterId[26];
    key = toupperAll(key);
    for (int i = 0; i < strlen(key); i++)
    {
        letterId[key[i] - 'A'] = true;
    }
    for (int j = 0; j < 26; j++)
    {
        if (!letterId[j]) 
        {
            return false;
        }
    }
    return true;
}

string toupperAll(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        key[i] = toupper(key[i]);
    }
    return key;
}

void keyMapper(string key, string plainText)
{
    char mapping[strlen(plainText) + 1];
    for (int i = 0; i < strlen(plainText); i++)
    {
        char c = plainText[i];
        if (isUppercase(c)) 
        {
            c = key[c - 'A'];
        }
        else if (isLowercase(c)) 
        {
            c = tolower(key[c - 'a']);
        }
        mapping[i] = c;
    }
    mapping[strlen(plainText)] = '\0';
    printf("ciphertext: %s\n", mapping);
}

