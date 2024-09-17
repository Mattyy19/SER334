/**
* Takes user input to alter BMP files based on their commands
*
* Completion time: 6 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BMPHandler.h"
#include "Image.h"

int main(int argc, char *argv[])
{
    //For BMP file
    struct BMP_Header* bmpHeader = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* dibHeader = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
    struct Image* image = (struct Image*)malloc(sizeof(struct Image));
    FILE* file;
    struct Pixel** pArr;

    //For inputs
    char fileName[1024];
    char outputName[1024];
    int hasGrayFilter = 0;
    int hasOutputName = 0;
    int hasScaleShift = 0;
    float factor = 1.0;
    int rShift = 0;
    int gShift = 0;
    int bShift = 0;
    int readFile = 0;

    if(strcmp(argv[0], "./ImageProcessor") == 0)
    {
        //Finds input BMP file
        file = fopen(argv[1], "rb");

        //If input is invalid
        if(file == NULL)
        {
            printf("%s was not found.\n", argv[1]);
            return 1;
        }

        strcpy(fileName, argv[1]);

        //Reads BMP, DIP header, and pixel array
        readBMPHeader(file, bmpHeader);
        readDIBHeader(file, dibHeader);

        pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dibHeader->imageHeight);
        int p;
        for (p = 0; p < dibHeader->imageHeight; p++) {
            pArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * dibHeader->imageWidth);
        }

        readPixelsBMP(file, pArr, dibHeader->imageWidth, dibHeader->imageHeight);
        image = image_create(pArr, dibHeader->imageWidth, dibHeader->imageHeight);

        readFile = 1;
        fclose(file);
    }

    //Loops through argv to find command arguments
    int i;
    for(i = 2; i < argc; i++)
    {
        //Makes output file name
        if(strcmp(argv[i], "-o") == 0)
        {
            strcpy(outputName, argv[i + 1]);
            hasOutputName = 1;
        }

        //Grayscale
        if(strcmp(argv[i], "-w") == 0)
        {
            hasGrayFilter = 1;
        }

        ///Shift red
        if(strcmp(argv[i], "-r") == 0)
        {
            sscanf(argv[i + 1], "%d", &rShift);
            if(rShift == 0)
            {
                printf("-r has an invalid shift\n");
            }
        }

        //Shift green
        if(strcmp(argv[i], "-g") == 0)
        {
            sscanf(argv[i + 1], "%d", &gShift);
            if(gShift == 0)
            {
                printf("-g has an invalid shift\n");
            }
        }

        //Shift blue
        if(strcmp(argv[i], "-b") == 0)
        {
            sscanf(argv[i + 1], "%d", &bShift);
            if(bShift == 0)
            {
                printf("-b has an invalid shift\n");
            }
        }

        if(strcmp(argv[i], "-s") == 0)
        {
            sscanf(argv[i + 1], "%f", &factor);
            hasScaleShift = 1;
        }
    }


    //Applies gray filter if user specified
    if(hasGrayFilter == 1)
    {
        image_apply_bw(image);
    }

    //Applis color shift if user specified
    if(rShift != 0 || gShift != 0 || bShift != 0)
    {
        image_apply_colorshift(image, rShift, gShift, bShift);
    }

    //Applies scale shift if user specified
    if(hasScaleShift == 1)
    {
        image_apply_resize(image, factor);

        //Change DIB and BMP header for new size
        dibHeader->imageWidth = image_get_width(image);
        dibHeader->imageHeight = image_get_height(image);
        //Row padding for array
        int padding = (4 - (dibHeader->imageWidth * 3) % 4) % 4;
        int width = dibHeader->imageWidth + padding;
        int size = width * dibHeader->imageHeight;
        dibHeader->imageSize = size;
        bmpHeader->size = dibHeader->imageSize + 54;
    }

    //If output was used as an option
    if(hasOutputName == 1)
    {
        FILE* fileOut = fopen(outputName, "wb");

        //Writes to BMP file
        writeBMPHeader(fileOut, bmpHeader);
        writeDIBHeader(fileOut, dibHeader);
        writePixelsBMP(fileOut, image_get_pixels(image), dibHeader->imageWidth, dibHeader->imageHeight);
        fclose(fileOut);
    }

    //If output wasn't used as an option
    if(hasOutputName == 0 && readFile == 1)
    {
        char copy[] = "_copy.bmp";
        fileName[strlen(fileName) - 4] = '\0';
        strcat(fileName, copy);

        FILE* fileOut = fopen(fileName, "wb");

        writeBMPHeader(fileOut, bmpHeader);
        writeDIBHeader(fileOut, dibHeader);
        writePixelsBMP(fileOut, image_get_pixels(image), dibHeader->imageWidth, dibHeader->imageHeight);
        fclose(fileOut);
    }

    free(bmpHeader);
    free(dibHeader);
    free(pArr);
    image_destroy(&image);

    return 0;
}