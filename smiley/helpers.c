#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare new color
    RGBTRIPLE color;
    color.rgbtRed = 0xFF;
    color.rgbtGreen = 0xE0;
    color.rgbtBlue = 0x33;

    // Replace all black pixels with color
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            if (pixel.rgbtRed == 0x00 && pixel.rgbtGreen == 0x00 && pixel.rgbtBlue == 0x00)
            {
                image[i][j] = color;
            }
        }
    }
}
