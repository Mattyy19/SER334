/**
* Does something
*
* Completion time:
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
    struct BMP_Header* bmpHeader = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* dibHeader = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
    struct Image* image = (struct Image*)malloc(sizeof(struct Image));
    FILE* file;
    struct Pixel** pArr;
    char input[] = " ";
    char fileName[1024];
    int grayFilterDone = 0;
    int hasOutputName = 0;
    int readFile = 0;

    printf("To exit the program, please type 'exit'.\n");

    while(input != NULL)
    {
        printf("Please input what BMP file you'd like to alter and the commands: ");
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
                char *temp = strtok(input, " ");
                while(temp!= NULL)
                {
                    if(strcmp(temp, "ImageProcessor") == 0)
                    {
                        break;
                    }
                    temp = strtok(NULL, " ");
                }

                temp = strtok(NULL, " ");
                file = fopen(temp, "rb");

                //If input is invalid
                if(file == NULL)
                {
                    printf("%s was not found.\n", temp);
                    return 1;
                }

                strcpy(fileName, temp);

                //Reads BMP, DIP header, and pixel array
                readBMPHeader(file, bmpHeader);
                readDIBHeader(file, dibHeader);

                pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dibHeader->imageHeight);
                for (int p = 0; p < 152; p++) {
                    pArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * dibHeader->imageWidth);
                }

                readPixelsBMP(file, pArr, dibHeader->imageWidth, dibHeader->imageHeight);
                image = image_create(pArr, dibHeader->imageWidth, dibHeader->imageHeight);

                readFile = 1;
            }

            //Makes output file name
            if(strcmp(token, "-o") == 0)
            {
                char *temp = token;
                temp = strtok(NULL, " ");
                FILE* fileOut = fopen(temp, "wb");

                //Writes to BMP file
                writeBMPHeader(fileOut, bmpHeader);
                writeDIBHeader(fileOut, dibHeader);
                writePixelsBMP(fileOut, pArr, dibHeader->imageWidth, dibHeader->imageHeight);

                hasOutputName = 1;
            }

            //Grayscale
            if(strcmp(token, "-w") == 0)
            {
                image_apply_bw(image);
                grayFilterDone = 1;
            }

            //Shift red
            if(strcmp(token, "-r") == 0)
            {
                char *temp = token;
                temp = strtok(NULL, " ");
                int rShift = 0;
                sscanf(temp, "%d", &rShift);

                if(grayFilterDone == 0)
                {
                    char *temp2 = token;
                    while(temp2 != NULL)
                    {
                        if(strcmp(token, "-w\n") == 0)
                        {
                            image_apply_bw(image);
                            grayFilterDone = 1;
                        }
                        temp2 = strtok(NULL, " ");
                    }
                }

                image_apply_colorshift(image, rShift, 0, 0);
            }

            //Shift green
            if(strcmp(token, "-g") == 0)
            {
                if(grayFilterDone == 0)
                {
                    char *temp2 = token;
                    while(temp2 != NULL)
                    {
                        if(strcmp(token, "-w\n") == 0)
                        {
                            image_apply_bw(image);
                            grayFilterDone = 1;
                        }
                        temp2 = strtok(NULL, " ");
                    }
                }

                char *temp = token;
                temp = strtok(NULL, " ");
                int gShift = 0;
                sscanf(temp, "%d", &gShift);

                image_apply_colorshift(image, 0, gShift, 0);
            }

            //Shift blue
            if(strcmp(token, "-b") == 0)
            {
                if(grayFilterDone == 0)
                {
                    char *temp2 = token;
                    while(temp2 != NULL)
                    {
                        if(strcmp(token, "-w\n") == 0)
                        {
                            image_apply_bw(image);
                            grayFilterDone = 1;
                        }
                        temp2 = strtok(NULL, " ");
                    }
                }

                char *temp = token;
                temp = strtok(NULL, " ");
                //temp[strlen(temp) - 1] = '\0';
                int bShift = 0;
                sscanf(temp, "%d", &bShift);

                image_apply_colorshift(image, 0, 0, bShift);
            }

            if(strcmp(token, "-s") == 0)
            {
                char *temp = token;
                temp = strtok(NULL, " ");
                float factor = 1.0;
                sscanf(temp, "%f", &factor);

                image_apply_resize(image, factor);
            }

            token = strtok(NULL, " ");
        }
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
        writePixelsBMP(fileOut, pArr, dibHeader->imageWidth, dibHeader->imageHeight);
    }

    return 0;
}