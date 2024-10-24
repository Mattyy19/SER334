/**
* Takes user input to alter BMP files based on their commands
*
* Completion time: 4.5 hours
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

struct mark
{
	int mark;
};

struct thread_info
{
	int width;
	int height;
	struct Pixel** pixels;
	struct mark** marks;
};

struct mark** marked;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//Applies box blur
void* boxBlur(void* data)
{
	struct thread_info* info = (struct thread_info*)data;
	struct thread_info* newInfo = (struct thread_info*)malloc(sizeof(struct thread_info));
	//Creates copy of original pixel array
	newInfo->pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * info->height);
	int p;
	for (p = 0; p < info->height; p++) {
		newInfo->pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * info->width);
	}

	//Copies values from orignal pixel array into new blurred pixel array
	int k, l;
	for(k = info->height - 1; k >= 0; k--)
	{
		for(l = 0; l < info->width; l++)
		{
			newInfo->pixels[k][l].red = info->pixels[k][l].red;
			newInfo->pixels[k][l].red = info->pixels[k][l].green;
			newInfo->pixels[k][l].red = info->pixels[k][l].blue;
		}
	}

	//Loops through to calculate appropriate blur
	int i, j;
	for(i = info->height - 1; i >= 0; i--)
	{
		for(j = 0; j < info->width; j++)
		{
			//Initial pixel
			struct Pixel temp = info->pixels[i][j];
			//printf("Orig pixel: r = %d, g = %d, b = %d\n", temp.red, temp.green, temp.blue);
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
				//printf("Prev top left: r = %d, g = %d, b = %d\n", temp1.red, temp1.green, temp1.blue);
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
			//Left side
			else if(i != (info->height - 1) && i != 0 && j == 0)
			{
				struct Pixel temp1 = info->pixels[i][j+1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j+1];
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

				temp1 = info->pixels[i-1][j];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				totalRed = totalRed / 6;
				totalGreen = totalGreen / 6;
				totalBlue = totalBlue / 6;
			}
			//Right side
			else if(i != (info->height - 1) && i != 0 && j == (info->width - 1))
			{
				struct Pixel temp1 = info->pixels[i][j-1];
				totalRed += temp1.red;
				totalGreen += temp1.green;
				totalBlue += temp1.blue;

				temp1 = info->pixels[i-1][j-1];
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

				temp1 = info->pixels[i-1][j];
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

			newInfo->pixels[i][j].red = totalRed;
			newInfo->pixels[i][j].green = totalGreen;
			newInfo->pixels[i][j].blue= totalBlue;
			//printf("New pixel: r = %d, g = %d, b = %d\n", newInfo->pixels[i][j].red, newInfo->pixels[i][j].green, newInfo->pixels[i][j].blue);
			//printf("Orig pixel: r = %d, g = %d, b = %d\n\n", info->pixels[i][j].red, info->pixels[i][j].green, info->pixels[i][j].blue);
		}
	}

	//Stores new blurred pixel array into original pixel array
	int m, n;
	for(m = info->height - 1; m >= 0; m--)
	{
		for(n = 0; n < info->width; n++)
		{
			info->pixels[m][n].red = newInfo->pixels[m][n].red;
			info->pixels[m][n].green = newInfo->pixels[m][n].green;
			info->pixels[m][n].blue = newInfo->pixels[m][n].blue;
		}
	}

	//Frees new blurred pixel array
	for (p = 0; p < info->height; p++) {
		free(newInfo->pixels[p]);
	}
	free(newInfo->pixels);
	free(newInfo);

	return info;
}

void* cheese(void* data)
{
	struct thread_info* info = (struct thread_info*)data;
	//Loops through to draw circles
	int i, j;
	for(i = info->height - 1; i >= 0; i--)
	{
		for(j = 0; j < info->width; j++)
		{
			printf("%d", info->marks[i][j].mark);
			if(info->marks[i][j].mark == 1)
			{
				info->pixels[i][j].red = 0;
				info->pixels[i][j].green = 0;
				info->pixels[i][j].blue = 0;
			}
		}
		printf("\n");
	}

	return data;
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

	marked = (struct mark**)malloc(sizeof(struct mark) * dibHeader1->imageHeight);
	for (p = 0; p < dibHeader1->imageHeight; p++) {
		marked[p] = (struct mark*)malloc(sizeof(struct mark) * dibHeader1->imageWidth);
	}

	int numHoles;
	int avgRadius;
	int smallSide;
	if(dibHeader1->imageWidth > dibHeader1->imageHeight)
	{
		smallSide = dibHeader1->imageHeight;
		numHoles = (int)(dibHeader1->imageHeight * .08);
		avgRadius = (int)(dibHeader1->imageHeight * .08);
	}
	else if(dibHeader1->imageWidth < dibHeader1->imageHeight)
	{
		smallSide = dibHeader1->imageWidth;
		numHoles = (int)(dibHeader1->imageWidth * .08);
		avgRadius = (int)(dibHeader1->imageWidth * .08);
	}
	else
	{
		smallSide = dibHeader1->imageHeight;
		numHoles = (int)(dibHeader1->imageHeight * (8.0/100.0));
		avgRadius = (int)(dibHeader1->imageHeight * (8.0/100.0));

		if(numHoles == 0)
		{
			numHoles = 1;
		}
		if(avgRadius == 0)
		{
			avgRadius = 1;
		}
	}

	srand(rand());
	int smallHoles = rand() % (((numHoles)/4 ) - 0 + 1);
	int largeHoles = rand() % (((numHoles)/4 ) - 0 + 1);

	int i, j;
	for(i = 0; i < smallHoles; i++)
	{
		int radius = avgRadius - (rand() % ((avgRadius - 1) - 1 + 1) + 1);

		int xCenter = rand() % (dibHeader1->imageWidth - 0 + 1);
		int yCenter = rand() % (dibHeader1->imageHeight - 0 + 1);

		int x, y;
		for(x = dibHeader1->imageHeight - 1; x >= 0; x--)
		{
			for(y = 0; y < dibHeader1->imageWidth; y++)
			{
				int dx = x - xCenter;
				int dy = y - yCenter;

				if((dx * dx + dy * dy) <= radius * radius + radius)
				{
					marked[x][y].mark = 1;
				}
			}
		}
	}

	for(j = 0; j < largeHoles; j++)
	{
		int radius = avgRadius + (rand() % ((smallSide/4)) - (avgRadius + 1) + 1) + (avgRadius + 1);

		int xCenter = rand() % (dibHeader1->imageWidth - 0 + 1);
		int yCenter = rand() % (dibHeader1->imageHeight - 0 + 1);

		int x, y;
		for(x = dibHeader1->imageHeight - 1; x >= 0; x--)
		{
			for(y = 0; y < dibHeader1->imageWidth; y++)
			{
				int dx = x - xCenter;
				int dy = y - yCenter;

				if((dx * dx + dy * dy) <= radius * radius + radius)
				{
					marked[x][y].mark = 1;
				}
			}
		}
	}

	int k;
	for(k = 0; k < (numHoles - (largeHoles + smallHoles)); k++)
	{
		int xCenter = rand() % (dibHeader1->imageWidth - 0 + 1);
		int yCenter = rand() % (dibHeader1->imageHeight - 0 + 1);

		int x, y;
		for(x = dibHeader1->imageHeight - 1; x >= 0; x--)
		{
			for(y = 0; y < dibHeader1->imageWidth; y++)
			{
				int dx = x - xCenter;
				int dy = y - yCenter;

				if((dx * dx + dy * dy) <= avgRadius * avgRadius + avgRadius)
				{
					marked[x][y].mark = 1;
				}
			}
		}
	}

	test1->marks = marked;

	cheese(test1);

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
		struct thread_info* test = (struct thread_info*)malloc(sizeof(struct thread_info));
		test->height = dibHeader->imageHeight;
		test->width = dibHeader->imageWidth;
		test->pixels = pArr;

		cheese(test);

		free(test);
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