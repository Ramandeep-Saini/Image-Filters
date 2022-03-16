#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // the first loop goes through the rows
    for (int i = 0; i < height; i++)
    {
        //this loop goes through the pixels
        for (int j = 0; j < width; j++)
        {
            //stores RGB values in corresponding variables
            float b = image[i][j].rgbtBlue;
            float g = image[i][j].rgbtGreen;
            float r = image[i][j].rgbtRed;

            //stores the average
            float average = round((r + b + g) / 3);

            //changes original rgb values in pixel to average
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            RGBTRIPLE t = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = t;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary array to store the original image, with original surrounding pixels
    RGBTRIPLE te [height][width];
    for (int  i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            te[i][j] = image[i][j];
        }
    }

    //going through rows
    for (int i = 0; i < height; i++)
    {
        //going through columns
        for (int j = 0; j < width; j++)
        {
            float sumr;
            float sumb;
            float sumg;
            int count;
            sumr = sumb = sumg = count = 0;

            //going vertically and horizontally for the pixels
            for (int q = -1; q < 2; q++)
            {
                for (int r = -1; r < 2; r++)
                {
                    // Check if pixel is outside rows
                    if (i + q < 0 || i + q >= height)
                    {
                        continue;
                    }
                    // Check if pixel is outside columns
                    if (j + r < 0 || j + r >= width)
                    {
                        continue;
                    }
                    // Otherwise add to sums
                    sumr += te[i + q][j + r].rgbtRed;
                    sumb += te[i + q][j + r].rgbtBlue;
                    sumg += te[i + q][j + r].rgbtGreen;
                    count++;
                }
            }
            // Get average and blur image
            image[i][j].rgbtRed = round(sumr / count);
            image[i][j].rgbtGreen = round(sumg / count);
            image[i][j].rgbtBlue = round(sumb / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary array to store the original image, with original surrounding pixels
    RGBTRIPLE te [height][width];
    for (int  i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            te[i][j] = image[i][j];
        }
    }

    //initialise sobel arrays for the kernel
    int Gx [3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy [3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Go through rows
    for (int i = 0; i < height; i++)
    {
        // Go through columns
        for (int j = 0; j < width; j++)
        {
            // Initialise variables
            float Gx_red;
            float Gx_blue;
            float Gx_green;
            float Gy_red;
            float Gy_blue;
            float Gy_green;
            Gx_red = Gx_blue = Gx_green = Gy_red = Gy_blue = Gy_green = 0;
            // Each pixel, loop vertically and horizontally
            for (int q = -1; q < 2; q++)
            {
                for (int r = -1; r < 2; r++)
                {
                    // Check if pixel outside rows
                    if (i + q < 0 || i + q >= height)
                    {
                        continue;
                    }
                    // Check if pixel outside columns
                    if (j + r < 0 || j + r >= width)
                    {
                        continue;
                    }
                    // Otherwise add to sums
                    Gx_red += te[i + q][j + r].rgbtRed * Gx[q + 1][r + 1];
                    Gx_green += te[i + q][j + r].rgbtGreen * Gx[q + 1][r + 1];
                    Gx_blue += te[i + q][j + r].rgbtBlue * Gx[q + 1][r + 1];
                    Gy_red += te[i + q][j + r].rgbtRed * Gy[q + 1][r + 1];
                    Gy_green += te[i + q][j + r].rgbtGreen * Gy[q + 1][r + 1];
                    Gy_blue += te[i + q][j + r].rgbtBlue * Gy[q + 1][r + 1];
                }
            }

            // Sobel operator calculations
            int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));
            // Stop at 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            // Assign new values to pixels
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
