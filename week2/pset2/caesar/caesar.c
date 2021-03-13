#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

bool isValidInput(string key);
void keyMapper(int key, string plaintext);
int inputStringToInt(string key);

int main(int argc, string argv[])
{
    if (argc != 2 || !isValidInput(argv[1]))
    {
        printf("Invalid Input\n");
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    int key = inputStringToInt(argv[1]);
    keyMapper(key, plaintext);
}

bool isValidInput(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isdigit(key[i]))
        {
            return false;
        }
    }
    return true;
}

int inputStringToInt(string key)
{
    int num = 0;
    int len = strlen(key);
    for (int i = 0; i < len; i++)
    {
        int j = key[i] - '0';
        num += j * (int) pow(10, len - (i + 1));
    }
    return num % 26;
}

void keyMapper(int key, string plaintext)
{
    char ciphertext[strlen(plaintext) + 1];
    for (int i = 0; i < strlen(plaintext); i++)
    {
        int c = plaintext[i];
        if (65 <= c && c <= 90)
        {
            c += key;
            if (c > 90)
            {
                c -= 26;
            }
        }
        else if (97 <= c && c <= 122)
        {
            c += key;
            if (c > 122)
            {
                c -= 26;
            }
        }
        ciphertext[i] = (char) c;
    }
    ciphertext[strlen(plaintext)] = '\0';
    printf("ciphertext: %s\n", ciphertext);
}

