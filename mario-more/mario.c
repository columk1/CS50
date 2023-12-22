#include <cs50.h>
#include <stdio.h>

void printHash(x, y)
    {
        for(int i = x + y; i >= x; i--)
            {
                printf("#");
            }
    }

void drawPyramid(int height)
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = i + 1; j < height; j++)
            {
                printf(" ");
            }
            printHash(height, i);
            printf("  ");
            printHash(height, i);
            printf("\n");
        }
    }

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    drawPyramid(height);
}