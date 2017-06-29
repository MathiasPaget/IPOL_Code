/*
 *	
 *      
 *	useful write/read functions for image files
 *
 *      Jean-Philippe Tarel
 *      e-mail: Jean-Philippe.Tarel@ifsttar.fr
 *
 *      23/11/2011 IFSTTAR copyright  
 */

#ifndef image_h
#define image_h

#include "basic.h"

#define NUM_COLOR 9
#define TRANS (unsigned char)0
#define RED (unsigned char)1
#define GREEN (unsigned char)2
#define WHITE (unsigned char)8

#define FSWAP(a,b) { register float t=(a);(a)=(b);(b)=t;}

extern unsigned char ColorTable[NUM_COLOR][3];

// Allocation

int AllocCimage(unsigned char **buffer, int dimx, int dimy, int numcol);
int AllocFimage(float **buffer, int dimx, int dimy, int numcol);
int AllocSimage(float **buffer, int dimx, int dimy, int numcol);

// type

int ImageTypePGM(char *name_file);
int ImageTypePPM(char *name_file);
int ImageTypeJPG(char *name_file);

// Load

int LoadCimage(char *name_file, unsigned char **buffer, int *dimx, int *dimy);
int LoadCimageColor(char *name_file, unsigned char **buffer, int *dimx, int *dimy, int *ncol);
int LoadFimageColor(char *name_file, float **buffer, int *dimx, int *dimy, int *ncol);

// Save
int SaveFimage(float *buffer, int dimx, int dimy, char *name_file);
int SaveFimageColor(float *buffer, int dimx, int dimy, int ncol, char *name_file);

int SaveCimage(unsigned char *buffer, int dimx, int dimy, char *name_file);
int SaveCimageColor(unsigned char *buffer, int dimx, int dimy, int ncol, char *name_file);

int SaveCimageOverlay(unsigned char *buffer, unsigned char *overlay, int dimx, int dimy, char *name_file);
int SaveFimagePFM(float *buffer, int dimx, int dimy, int ncol, char *name_file);
int SaveSimagePFM(short *buffer, int dimx, int dimy, int ncol, char *name_file);

unsigned char CMin(unsigned char *vals, int size);
int MedianSmoothCimage(int dimx, int dimy, int ncol, unsigned char *buffer, int size, unsigned char *result);

unsigned char QuickCMedian(unsigned char *arr, int n);

#endif 

