#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;

            int grey = round((red + green + blue) / 3);

            image[i][j].rgbtRed = grey;
            image[i][j].rgbtGreen = grey;
            image[i][j].rgbtBlue = grey;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int limit = width / 2;
    if (width % 2 != 0)
    {
        limit = (width + 1) / 2;
    }
    width--;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < limit; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][width - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j].rgbtBlue;

            image[i][width - j].rgbtRed = red;
            image[i][width - j].rgbtGreen = green;
            image[i][width - j].rgbtBlue = blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (new_image == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float redSum = 0;
            float greenSum = 0;
            float blueSum = 0;
            float visitedCells = 0;
            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    int y = i + a;
                    int x = j + b;
                    if (y > -1 && y < height && x > -1 && x < width)
                    {
                        redSum += image[y][x].rgbtRed;
                        greenSum += image[y][x].rgbtGreen;
                        blueSum += image[y][x].rgbtBlue;
                        visitedCells++;
                    }
                }
            }
            new_image[i][j].rgbtRed = round(redSum / visitedCells);
            new_image[i][j].rgbtGreen = round(greenSum / visitedCells);
            new_image[i][j].rgbtBlue = round(blueSum / visitedCells);
        }
    }

    for (int m = 0; m < height; m++)
    {
        for (int n = 0; n < width; n++)
        {
            image[m][n] = new_image[m][n];
        }
    }
    
    free(new_image);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (new_image == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }

    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
        
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float redSum[2] = {0, 0};
            float greenSum[2] = {0, 0};
            float blueSum[2] = {0, 0};

            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    int y = i + a;
                    int x = j + b;
                    if (y > -1 && y < height && x > -1 && x < width)
                    {
                        int red = image[y][x].rgbtRed;
                        int green = image[y][x].rgbtGreen;
                        int blue = image[y][x].rgbtBlue;

                        redSum[0] += red * Gx[a + 1][b + 1];
                        redSum[1] += red * Gy[a + 1][b + 1];

                        greenSum[0] += green * Gx[a + 1][b + 1];
                        greenSum[1] += green * Gy[a + 1][b + 1];

                        blueSum[0] += blue * Gx[a + 1][b + 1];
                        blueSum[1] += blue * Gy[a + 1][b + 1];
                    }
                }
            }

            int redSobel = round(sqrt(pow(redSum[0], 2) + pow(redSum[1], 2)));
            int greenSobel = round(sqrt(pow(greenSum[0], 2) + pow(greenSum[1], 2)));
            int blueSobel = round(sqrt(pow(blueSum[0], 2) + pow(blueSum[1], 2)));

            if (redSobel > 255)
            {
                redSobel = 255;
            }
            if (greenSobel > 255)
            {
                greenSobel = 255;
            }
            if (blueSobel > 255)
            {
                blueSobel = 255;
            }

            new_image[i][j].rgbtRed = redSobel;
            new_image[i][j].rgbtGreen = greenSobel;
            new_image[i][j].rgbtBlue = blueSobel;
        }
    }

    for (int m = 0; m < height; m++)
    {
        for (int n = 0; n < width; n++)
        {
            image[m][n] = new_image[m][n];
        }
    }
    
    free(new_image);
    return;
}
