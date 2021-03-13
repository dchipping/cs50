#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void export_jpeg(int start, int finish, int counter, int blocks, unsigned char card[blocks][512]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // Find the size of the file
    fseek(inptr, 0, SEEK_END);
    int size = ftell(inptr);
    fseek(inptr, 0, SEEK_SET);
    // printf("Size of %i Bytes\n", size);

    // Determine how many blocks of 512
    int blocks = size / 512;
    // printf("%i Many Blocks of 512B\n", blocks);

    // Allocate memory the size of card
    unsigned char (*card)[512]  = calloc(blocks, 512 * sizeof(char));
    if (card == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(inptr);
        return 1;
    }

    // Break input file into blocks of 512B
    for (int i = 0; i < blocks; i++)
    {
        // Read every 512B of data
        fread(card[i], sizeof(char), 512, inptr);
        // printf("Block %i: 0x%x, 0x%x, 0x%x\n", i, (unsigned char)card[i][0], (unsigned char)card[i][1], (unsigned char)card[i][2]);

    }

    // Look at every blocks first 3 bytes
    int counter = 0;
    for (int j = 0; j < blocks; j++)
    {
        if (card[j][0] == 0xff && card[j][1] == 0xd8 && card[j][2] == 0xff)
        {
            for (int k = j + 1; k < blocks; k++)
            {
                if (card[k][0] == 0xff && card[k][1] == 0xd8 && card[k][2] == 0xff)
                {
                    int start = j;
                    int finish = k;
                    export_jpeg(start, finish, counter, blocks, card);
                    counter++;
                    break;
                }
                else if (k == blocks - 1)
                {
                    int start = j;
                    int finish = k + 1;
                    export_jpeg(start, finish, counter, blocks, card);
                    break;
                }
            }
        }
    }

    fclose(inptr);
    free(card);
}

void export_jpeg(int start, int finish, int counter, int blocks, unsigned char card[blocks][512])
{
    char *num = calloc(4, sizeof(char));
    num[0] = '0' + (char) counter / 100 % 10;
    num[1] = '0' + (char) counter / 10 % 10;
    num[2] = '0' + (char) counter % 10;
    num[3] = '\0';

    char *suffix = ".jpg";
    int suf_len = strlen(suffix);

    int total_len = 2 + suf_len;
    char *file_name = calloc(total_len, sizeof(char));

    strcat(file_name, num);
    strcat(file_name, suffix);

    FILE *outptr = fopen(file_name, "w");
    if (outptr == NULL)
    {
        printf("Could not create %s.\n", file_name);
        return;
    }

    for (int i = start; i < finish; i++)
    {
        fwrite(card[i], sizeof(char), 512, outptr);
    }

    // printf("%s\n", file_name);
    // printf("Start: %i\n", start);
    // printf("Finish: %i\n", finish);

    fclose(outptr);
    free(num);
    free(file_name);
}