/**
* Takes user input to alter BMP files based on their commands
*
* Completion time: 1 hour
*
* @author Matthew Nguyen
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "BMPHandler.h"
#include "PixelProcessor.h"
//TODO: finish me

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096

//TODO: finish me
#define THREAD_COUNT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

//TODO: finish me
struct Pixel** pArr;

struct thread_info
{
	int width;
	int height;
	struct Pixel** pixels;
};

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//TODO: Add blurs for edges of width
//Applies box blur
void* boxBlur(void* data)
{
	struct thread_info* info = (struct thread_info*)data;
	int i, j;
	for(i = info->height - 1; i >= 0; i--)
	{
		for(j = 0; j < info->width; j++)
		{
			//Initial pixel
			struct Pixel temp = info->pixels[i][j];
			int totalRed = temp.red;
			int totalGreen = temp.green;
			int totalBlue = temp.blue;

			//If initial pixel not around edges
			if(i != (info->height - 1) && i != 0 && j != 0 && j != (info->width - 1))
			{
				struct Pixel temp1 = info->pixels[i-1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 9;
				totalGreen = totalGreen / 9;
				totalBlue = totalBlue / 9;
			}
			//If initial pixel is at the top
			else if(i == (info->height - 1) && j != 0 && j != (info->width - 1))
			{
				struct Pixel temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 6;
				totalGreen = totalGreen / 6;
				totalBlue = totalBlue / 6;
			}
			//If the intial pixel is at the bottom
			else if(i == 0 && j != 0 && j != (info->width - 1))
			{
				struct Pixel temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 6;
				totalGreen = totalGreen / 6;
				totalBlue = totalBlue / 6;
			}
			//Top left corner
			else if(i == (info->height - 1) && j == 0)
			{
				struct Pixel temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 4;
				totalGreen = totalGreen / 4;
				totalBlue = totalBlue / 4;
			}
			//Top right corner
			else if(i == (info->height - 1) && j == (info->width-1))
			{
				struct Pixel temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 4;
				totalGreen = totalGreen / 4;
				totalBlue = totalBlue / 4;
			}
			//Bottom left
			else if(i == 0 && j == 0)
			{
				struct Pixel temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 4;
				totalGreen = totalGreen / 4;
				totalBlue = totalBlue / 4;
			}
			//Bottom right
			else if(i == 0 && j == (info->width-1))
			{
				struct Pixel temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i+1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 4;
				totalGreen = totalGreen / 4;
				totalBlue = totalBlue / 4;
			}

			info->pixels[i][j].red = totalRed;
			info->pixels[i][j].green = totalGreen;
			info->pixels[i][j].blue= totalBlue;
		}
	}
	return info;
}

void* cheese(void* data)
{
	pthread_exit(0);
}

//Just tests filters
void tester()
{
	struct BMP_Header* bmpHeader1 = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
	struct DIB_Header* dibHeader1 = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
	FILE* file1;

	//For inputs
	char fileName[1024];

	//Finds input BMP file
		file1 = fopen("test1wonderbread.bmp", "rb");

	strcpy(fileName, "test3.bmp");

	//Reads BMP, DIP header, and pixel array
	readBMPHeader(file1, bmpHeader1);
	readDIBHeader(file1, dibHeader1);

	pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dibHeader1->imageHeight);
	int p;
	for (p = 0; p < dibHeader1->imageHeight; p++) {
		pArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * dibHeader1->imageWidth);
	}

	readPixelsBMP(file1, pArr, dibHeader1->imageWidth, dibHeader1->imageHeight);

	fclose(file1);

	struct thread_info* test1 = (struct thread_info*)malloc(sizeof(struct thread_info));
	test1->height = dibHeader1->imageHeight;
	test1->width = dibHeader1->imageWidth;
	test1->pixels = pArr;

	boxBlur(test1);

	free(test1);

	FILE* fileOut = fopen("result.bmp", "wb");

	//Writes to BMP file
	writeBMPHeader(fileOut, bmpHeader1);
	writeDIBHeader(fileOut, dibHeader1);
	writePixelsBMP(fileOut, pArr, dibHeader1->imageWidth, dibHeader1->imageHeight);
	fclose(fileOut);

	free(bmpHeader1);
	free(dibHeader1);
}

void main(int argc, char* argv[]) {
	//TODO: finish me
	tester();
	pthread_t tids[THREAD_COUNT];
	//For BMP file
	struct BMP_Header* bmpHeader = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
	struct DIB_Header* dibHeader = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
	FILE* file;

	//For inputs
	char fileName[1024];
	char outputName[1024];
	char filterType[1024];
	int hasOutputName = 0;

	//Opens BMP file
	if(strcmp(argv[0], "./NguyenFilters -i") == 0)
	{
		//Finds input BMP file
		file = fopen(argv[2], "rb");

		//If input is invalid
		if(file == NULL)
		{
			printf("%s was not found.\n", argv[2]);
		}

		strcpy(fileName, argv[2]);

		//Reads BMP, DIP header, and pixel array
		readBMPHeader(file, bmpHeader);
		readDIBHeader(file, dibHeader);

		pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dibHeader->imageHeight);
		int p;
		for (p = 0; p < dibHeader->imageHeight; p++) {
			pArr[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * dibHeader->imageWidth);
		}

		readPixelsBMP(file, pArr, dibHeader->imageWidth, dibHeader->imageHeight);

		fclose(file);
	}

	//Loops through argv to find command arguments
	int i;
	for(i = 3; i < argc; i++)
	{
		//Makes output file name
		if(strcmp(argv[i], "-o") == 0)
		{
			strcpy(outputName, argv[i + 1]);
			hasOutputName = 1;
		}

		//Has filter
		if(strcmp(argv[i], "-f") == 0)
		{
			strcpy(filterType, argv[i + 1]);
		}
	}

	//Blur filter selected
	if(strcmp(filterType, "b") == 0)
	{
		struct thread_info* test = (struct thread_info*)malloc(sizeof(struct thread_info));
		test->height = dibHeader->imageHeight;
		test->width = dibHeader->imageWidth;
		test->pixels = pArr;

		boxBlur(test);

		free(test);
	}

	//Cheese filter selected
	if(strcmp(filterType, "c") == 0)
	{
		//TODO
	}

	//If output was used as an option
	if(hasOutputName == 1)
	{
		FILE* fileOut = fopen(outputName, "wb");

		//Writes to BMP file
		writeBMPHeader(fileOut, bmpHeader);
		writeDIBHeader(fileOut, dibHeader);
		writePixelsBMP(fileOut, pArr, dibHeader->imageWidth, dibHeader->imageHeight);
		fclose(fileOut);
	}

	free(bmpHeader);
	free(dibHeader);
	free(pArr);
}