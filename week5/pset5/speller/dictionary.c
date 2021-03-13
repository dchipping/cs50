// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 20000;

// Hash table
node *table[N];

// For size function
int words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int end;
    char lowercase[LENGTH + 1];
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            lowercase[i] = word[i] + 32;
        }
        else
        {
            lowercase[i] = word[i];
        }
        end = i;
    }
    lowercase[end + 1] = '\0';

    int key = hash(lowercase);
    node *n = table[key];

    if (n != NULL)
    {
        do
        {
            if (!strcmp(n->word, lowercase))
            {
                return true;
            }
            n = n->next;
        }
        while (n != NULL);
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 - http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        printf("Error opening dictionary");
        return false;
    }

    fseek(inptr, 0, SEEK_END);
    int bytes = ftell(inptr);
    fseek(inptr, 0, SEEK_SET);

    unsigned char buffer[bytes];
    fread(buffer, bytes, sizeof(unsigned char), inptr);

    for (int n = 0; n < N; n++)
    {
        table[n] = NULL;
    }

    for (int i = 0, s = 0; i < bytes; i++)
    {
        if (buffer[i] == '\n')
        {
            // Create new word
            int len = i - s;
            char new_word[len];
            for (int j = 0; j < len; j++)
            {
                new_word[j] = buffer[s + j];
            }
            new_word[len] = '\0';

            // Create node
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            memset(n, 0, sizeof(node));
            
            for (int k = 0; k < len + 1; k++)
            {
                n->word[k] = new_word[k];
            }
            n->next = NULL;

            // Hash new word and bucket
            int key = hash(new_word);
            if (table[key] == NULL)
            {
                table[key] = n;
            }
            else
            {
                node *tmp = table[key];
                while (tmp->next != NULL)
                {
                    tmp = tmp->next;
                }
                tmp->next = n;
            }

            // Update new starting letter
            s = i + 1;

            // Word count increased
            words++;
        }
    }
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Recursively free up a node
void free_up(node *n)
{
    if (n == NULL)
    {
        return;
    }

    free_up(n->next);

    free(n);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        if (tmp != NULL)
        {
            free_up(tmp);
        }
    }
    return true;
}
