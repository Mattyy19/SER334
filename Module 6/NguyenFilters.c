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


////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

//TODO: finish me
void* boxBlur(void* data)
{
	return data;
}

void* cheese(void* data)
{
	return data;
}


void main(int argc, char* argv[]) {
	//TODO: finish me
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
			printf("%s was not found.\n", argv[1]);
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
		//TODO
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