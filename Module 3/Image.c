/**
* Specification for an image ADT.
*
* Completion time: 5 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "Image.h"

Image* image_create(struct Pixel** pArr, int width, int height)
{
 Image* newImage = (struct Image*)malloc(sizeof(Image));
 newImage->pArr = pArr;
 newImage->width = width;
 newImage->height = height;
 return newImage;
}

void image_destroy(Image** img)
{
 //If img is null
 if(img == NULL)
 {
  return;
 }

 free(*img);
 *img = NULL;
}

struct Pixel** image_get_pixels(Image* img)
{
 struct Pixel** temp = img->pArr;
 return temp;
}

int image_get_width(Image* img)
{
 return img->width;
}

int image_get_height(Image* img)
{
 return img->height;
}

void image_apply_bw(Image* img)
{
 int i, j;
 unsigned char gray;
 for(i = image_get_height(img) - 1; i >= 0; i--)
 {
  for(j = 0; j < image_get_width(img); j++)
  {
   struct Pixel* temp = &image_get_pixels(img)[i][j];

   gray = (unsigned char)((0.299 * temp->red) + (0.587 * temp->green) + (0.114 * temp->blue));
   temp->red = gray;
   temp->green = gray;
   temp->blue = gray;
  }
 }
}

void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift)
{
 int i, j;
 for(i = image_get_height(img) - 1; i >= 0; i--)
 {
  for(j = 0; j < image_get_width(img); j++)
  {
   struct Pixel* temp = &image_get_pixels(img)[i][j];

   if(temp->red + rShift >= 255)
   {
    temp->red = 255;
   }
   else if(temp->red + rShift <= 0)
   {
    temp->red = 0;
   }
   else
   {
    temp->red = temp->red + rShift;
   }

   if(temp->green + gShift >= 255)
   {
    temp->green = 255;
   }
   else if(temp->green + gShift <= 0)
   {
    temp->green = 0;
   }
   else
   {
    temp->green = temp->green + gShift;
   }

   if(temp->blue + bShift >= 255)
   {
    temp->blue = 255;
   }
   else if(temp->blue + bShift <= 0)
   {
    temp->blue = 0;
   }
   else
   {
    temp->blue = temp->blue + bShift;
   }
  }
 }
}

void image_apply_resize(Image* img, float factor)
{
 int newWidth = (int)(img->width * factor);
 int newHeight = (int)(img->height * factor);

 //New pixel array
 struct Pixel** newpArr = (struct Pixel**)malloc(newHeight * sizeof(struct Pixel*));
 for (int i = 0; i < newHeight; i++) {
  newpArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
 }

 int i, j;
 //Putting in new array
 for (i = 0; i < newHeight; i++) {
  for (j = 0; j < newWidth; j++) {
   //Original pixel locations
   int origI = (int)(i / factor);
   int origJ = (int)(j / factor);

   // printf("Old location: R = %d, G = %d, B= %d\n", img->pArr[origI][origJ].red, img->pArr[origI][origJ].green,
   //  img->pArr[origI][origJ].blue); //for testing

   //Copy pixell in new array
   newpArr[i][j] = img->pArr[origI][origJ];
   // printf("New location: R = %d, G = %d, B= %d\n", newpArr[i][j].red, newpArr[i][j].green,
   //  newpArr[i][j].blue); //for testing
  }
 }

 //New pixel array and new width and height
 img->pArr = newpArr;
 img->width = newWidth;
 img->height = newHeight;
}