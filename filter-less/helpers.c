#include "helpers.h"
#include <cs50.h>
#include <math.h>

int min(int i, int j);
bool check_pixel(int x, int y, int width, int height);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int new_color_value = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = new_color_value;
            image[i][j].rgbtGreen = new_color_value;
            image[i][j].rgbtRed = new_color_value;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepia_blue = round(0.131 * image[i][j].rgbtBlue + 0.534 * image[i][j].rgbtGreen + 0.272 * image[i][j].rgbtRed);
            int sepia_green = round(0.168 * image[i][j].rgbtBlue + 0.686 * image[i][j].rgbtGreen + 0.349 * image[i][j].rgbtRed);
            int sepia_red = round(0.189 * image[i][j].rgbtBlue + 0.769 * image[i][j].rgbtGreen + 0.393 * image[i][j].rgbtRed);

            image[i][j].rgbtBlue = min(sepia_blue, 255);
            image[i][j].rgbtGreen = min(sepia_green, 255);
            image[i][j].rgbtRed = min(sepia_red, 255);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    // Make a copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int blue = 0;
            int green = 0;
            int red = 0;

            // Loop over a 3 x 3 grid around each pixel
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // Check if each pixel is in the image
                    if (check_pixel(k, l, height, width))
                    {
                        // Increment the count and store the sum of values for each color
                        count++;
                        blue += tmp[k][l].rgbtBlue;
                        green += tmp[k][l].rgbtGreen;
                        red += tmp[k][l].rgbtRed;
                    }
                }
            }
            // Update the image with blurred values
            image[i][j].rgbtBlue = round((float)blue / count);
            image[i][j].rgbtGreen = round((float)green / count);
            image[i][j].rgbtRed = round((float)red / count);
        }
    }
    return;
}

// Return the lesser of two integers
int min(int i, int j)
{
    return (i < j ? i : j);
}

// Find out if a square grid of specified size around the pixel are in the image
bool check_pixel(int y, int x, int height, int width)
{
    return y >= 0 && x >= 0 && y < height && x < width;
}