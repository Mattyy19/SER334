/**
* Specification for an image ADT.
*
* Completion time: 2 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "Image.h"

Image* image_create(struct Pixel** pArr, int width, int height)
{
 Image* newImage = malloc(sizeof(Image));
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
 unsigned char i;
 unsigned char j;
 for(i = 0; i < image_get_height(img); i++)
 {
  for(j = 0; j < image_get_width(img); j++)
  {
   struct Pixel* temp = img->pArr[(i * img->width) + j];

   unsigned char grey = (unsigned char)((0.299 * temp->red) + (0.587 * temp->green) + (0.114 * temp->blue));
   temp->red = grey;
   temp->green = grey;
   temp->blue = grey;
  }
 }
}

void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift)
{
 unsigned char i;
 unsigned char j;
 for(i = 0; i < image_get_height(img); i++)
 {
  for(j = 0; j < image_get_width(img); j++)
  {
   struct Pixel* temp = img->pArr[(i * img->width) + j];

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
 image_apply_bw(img);
 img->width = image_get_width(img) * factor;
 img->height = image_get_height(img) * factor;
}
