/**
* Implementation of several functions to manipulate BMP files.
*
* Completion time: 2 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "BMPHandler.h"
#include "Image.h"

void readBMPHeader(FILE* file, struct BMP_Header* header)
{
 //Set file poointer to beginning of file
 fseek(file, 0, SEEK_SET);

 //Reads file and stores into  BMP header
 fread(header->signature, sizeof(char), 2, file);
 fread(&header->size, sizeof(int), 1, file);
 fread(&header->reserved1, sizeof(short), 1, file);
 fread(&header->reserved2, sizeof(short), 1, file);
 fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header)
{
 //Set file poointer to beginning of file
 fseek(file, 0, SEEK_SET);

 //Writes to file from header
 fwrite(header->signature, sizeof(char), 2, file);
 fwrite(&header->size, sizeof(int), 1, file);
 fwrite(&header->reserved1, sizeof(short), 1, file);
 fwrite(&header->reserved2, sizeof(short), 1, file);
 fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

void readDIBHeader(FILE* file, struct DIB_Header* header)
{
 //Set file pointer to DIB header
 fseek(file, 14, SEEK_SET);

 //Reads from  file and stores in DIB header
 fread(&header->headerSize, sizeof(int), 1, file);
 fread(&header->imageWidth, sizeof(int), 1, file);
 fread(&header->imageHeight, sizeof(int), 1, file);
 fread(&header->planes, sizeof(short), 1, file);
 fread(&header->bitsPP, sizeof(short), 1, file);
 fread(&header->compression, sizeof(int), 1, file);
 fread(&header->imageSize, sizeof(int), 1, file);
 fread(&header->xPixelsPM, sizeof(int), 1, file);
 fread(&header->yPixelsPM, sizeof(int), 1, file);
 fread(&header->colorInCT, sizeof(int), 1, file);
 fread(&header->impColorCount, sizeof(int), 1, file);
}

void writeDIBHeader(FILE* file, struct DIB_Header* header)
{
 //Set file pointer to DIB header
 fseek(file, 14, SEEK_SET);

 //Writes to file  from header
 fwrite(&header->headerSize, sizeof(int), 1, file);
 fwrite(&header->imageWidth, sizeof(int), 1, file);
 fwrite(&header->imageHeight, sizeof(int), 1, file);
 fwrite(&header->planes, sizeof(short), 1, file);
 fwrite(&header->bitsPP, sizeof(short), 1, file);
 fwrite(&header->compression, sizeof(int), 1, file);
 fwrite(&header->imageSize, sizeof(int), 1, file);
 fwrite(&header->xPixelsPM, sizeof(int), 1, file);
 fwrite(&header->yPixelsPM, sizeof(int), 1, file);
 fwrite(&header->colorInCT, sizeof(int), 1, file);
 fwrite(&header->impColorCount, sizeof(int), 1, file);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height)
{
 //BMP Signature
 header->signature[0] = 'B';
 header->signature[1] = 'M';

 //Image size
 int row = ((width * 3) + 3) & ~3;
 int size = row * height;
 header->size = size + 54;

 //Reserved 1 and 2
 header->reserved1 = 0;
 header->reserved2 = 0;

 //Offfset
 header->offset_pixel_array = 54;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height)
{
 //Header size
 header->headerSize = 40;

 //Width and height
 header->imageWidth = width;
 header->imageHeight = height;

 //PLanes
 header->planes = 1;

 //Bits Per PIxel
 header->bitsPP = 24;

 //Compression
 header->compression = 0;

 //Image size
 int row = ((width * 3) + 3) & ~3;
 int size = row * height;
 header->imageSize = size;

 //Resolution
 header->xPixelsPM = 3780;
 header->yPixelsPM = 3780;

 //Color number
 header->colorInCT = 0;

 //Important color number
 header->impColorCount = 0;
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height)
{
 //Row size
 int size = ((width * 3) + 3) & ~3;

 unsigned char* row = malloc(sizeof(size));

 //Loop to read through array
 int i;
 for(i = height - 1; i >= 0; i--)
 {
  //Read from row and store in pixel array
  fread(row, sizeof(unsigned char), size, file);
  int j;
  for(j = 0; j < width; j++)
  {
   pArr[i][j].blue = row[j * 3];
   pArr[i][j].green = row[(j * 3) + 1];
   pArr[i][j].red = row[(j * 3) + 2];
  }
 }

 free(row);
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height)
{
 //Row size
 int size = ((width * 3) + 3) & ~3;

 unsigned char* row = malloc(sizeof(size));

 //Loop to read through array
 int i;
 for(i = height - 1; i >= 0; i--)
 {
  int j;
  for(j = 0; j < width; j++)
  {
   row[j * 3] = pArr[i][j].blue;
   row[(j * 3) + 1] = pArr[i][j].green;
   row[(j * 3) + 2] = pArr[i][j].red;
  }

  //If row not padded
  int k;
  for(k = width * 3; k < size; k++)
  {
   row[k] = 0;
  }

  //Write from pixel array
  fwrite(row, sizeof(unsigned char), size, file);
 }

 free(row);
}