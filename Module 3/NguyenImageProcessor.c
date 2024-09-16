/**
* Takes user input to alter BMP files based on their commands
*
* Completion time: 5 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BMPHandler.h"
#include "Image.h"

int main()
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
    char input[] = " ";


    printf("To exit the program, please type 'exit'.\n");

    while(input != NULL)
    {
        printf("Please input what BMP file you'd like to alter and the commands(ImageProcessor file name ...): ");
        fgets(input, 1024, stdin);
        input[strlen(input) - 1] = '\0';

        if(strcmp(input, "exit") == 0)
        {
            break;
        }

        char *token = strtok(input, " ");
        while(token != NULL)
        {
            //Finds input BMP file
            if(strcmp(token, "ImageProcessor") == 0)
            {
                token = strtok(NULL, " ");
                file = fopen(token, "rb");

                //If input is invalid
                if(file == NULL)
                {
                    printf("%s was not found.\n", token);
                    return 1;
                }

                strcpy(fileName, token);

                //Reads BMP, DIP header, and pixel array
                readBMPHeader(file, bmpHeader);
                readDIBHeader(file, dibHeader);

                pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dibHeader->imageHeight);
                for (int p = 0; p < dibHeader->imageHeight; p++) {
                    pArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * dibHeader->imageWidth);
                }

                readPixelsBMP(file, pArr, dibHeader->imageWidth, dibHeader->imageHeight);
                image = image_create(pArr, dibHeader->imageWidth, dibHeader->imageHeight);

                readFile = 1;
                fclose(file);
            }

            //Makes output file name
            if(strcmp(token, "-o") == 0)
            {
                token = strtok(NULL, " ");
                strcpy(outputName, token);
                hasOutputName = 1;
            }

            //Grayscale
            if(strcmp(token, "-w") == 0)
            {
                hasGrayFilter = 1;
            }

            //Shift red
            if(strcmp(token, "-r") == 0)
            {
                token = strtok(NULL, " ");
                sscanf(token, "%d", &rShift);
            }

            //Shift green
            if(strcmp(token, "-g") == 0)
            {
                token = strtok(NULL, " ");
                sscanf(token, "%d", &gShift);
            }

            //Shift blue
            if(strcmp(token, "-b") == 0)
            {
                token = strtok(NULL, " ");
                sscanf(token, "%d", &bShift);
            }

            if(strcmp(token, "-s") == 0)
            {
                token = strtok(NULL, " ");
                sscanf(token, "%f", &factor);
                hasScaleShift = 1;
            }

            token = strtok(NULL, " ");
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
    }

    return 0;
}