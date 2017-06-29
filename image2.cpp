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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "image2.h"

unsigned char ColorTable[NUM_COLOR][3] = {{0,0,0},      /* TRANS */
                                  {255,0,0},            /* RED */
                                  {0,255,0},            /* GREEN */
                                  {0,0,255},            /* BLUE */
                                  {0,255,255},          /* CIAN */
                                  {255,0,255},          /* PURPLE */
                                  {255,255,0},          /* YELLOW */ 
                                  {0,0,0},              /* BLACK */
                                  {255,255,255}};       /* WHITE */

/* load image buffer from a PGM file */
int LoadCimagePGM(char *name_file, unsigned char **buffer, int *dimx, int *dimy)
{
  FILE *file = NULL;
  STRING str;
  int size, current, readed, numlevel;

  if (name_file != NULL) {
  	if ((file = fopen(name_file,"rb")) == NULL) {
      		(void)fprintf(stderr,"LoadCimagePGM: File %s does not exist.\n",name_file);
      		return(0);
  	}
  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	if ((str[0] != 'P') || (str[1] != '5')) {
        	(void)fclose(file);
        	(void)fprintf(stderr,"LoadCimagePGM: bad image type %s.\n",name_file);
        	return(0);
  	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	while(current=='#') { 
        	if (fgets(str, 249, file) == NULL) {
                	(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
        	if ((current = fgetc(file)) == EOF) {
                	(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
  	} 
  	if (ungetc(current,file) != (int)current) {
        	(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
        	(void)fclose(file);
        	return(0);
  	}
  	/* scan header */
  	if (fscanf(file,"%d %d", dimx, dimy) != 2) {
      		(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	if (fscanf(file,"%d", &numlevel) != 1) {
      		(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}
        if (numlevel>255) {
      		(void)fprintf(stderr,"LoadCimagePGM: format not supported in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadCimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	/* alloc buffer memory */
  	size = (*dimx) * (*dimy);
  	if (((*buffer) = (unsigned char *)malloc(sizeof(unsigned char)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadCimagePGM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}

  	/* read buffer */
  	readed = fread(*buffer, sizeof(unsigned char), size, file);
  	if (readed != size) {
      		(void)fprintf(stderr,"LoadCimagePGM: Bad size file %s (%d<%d)\n",
			name_file,readed,size);
      		(void)fclose(file);
      		return(0);
  	}
	(void)fclose(file);
  } else {
	(void)fprintf(stderr,"LoadCimagePGM: Stdin not supported\n");
  }

  return(1);
}

/* load image buffer from a PPM file */
int LoadCimagePPM(char *name_file, unsigned char **buffer, int *dimx, int *dimy)
{
  FILE *file = NULL;
  STRING str;
  int size, current, readed, numlevel;

  if (name_file != NULL) {
  	if ((file = fopen(name_file,"rb")) == NULL) {
      		(void)fprintf(stderr,"LoadCimagePPM: File %s does not exist.\n",name_file);
      		return(0);
  	}
  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	if ((str[0] != 'P') || (str[1] != '6')) {
        	(void)fclose(file);
        	(void)fprintf(stderr,"LoadCimagePPM: bad image type %s.\n",name_file);
        	return(0);
  	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	while(current=='#') { 
        	if (fgets(str, 249, file) == NULL) {
                	(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
        	if ((current = fgetc(file)) == EOF) {
                	(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
  	} 
  	if (ungetc(current,file) != (int)current) {
        	(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
        	(void)fclose(file);
        	return(0);
  	}
  	/* scan header */
  	if (fscanf(file,"%d %d\n", dimx, dimy) != 2) {
      		(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	if (fscanf(file,"%d", &numlevel) != 1) {
      		(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}
        if (numlevel!=255) {
      		(void)fprintf(stderr,"LoadCimagePPM: format not supported in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadCimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	/* alloc buffer memory */
  	size = (*dimx) * (*dimy) * 3;
  	if (((*buffer) = (unsigned char *)malloc(sizeof(unsigned char)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadCimagePPM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}

  	/* read buffer */
  	readed = fread(*buffer, sizeof(unsigned char), size, file);
  	if (readed == 0) {
      		(void)fprintf(stderr,"LoadCimagePPM: Bad size file %s (%d<%d)\n",
			name_file,readed,size);
      		(void)fclose(file);
      		return(0);
  	}
  	(void)fclose(file);
  } else {
	(void)fprintf(stderr,"LoadCimagePPM: Stdin not supported\n");
  }

  return(1);
}

/* save in a file an unisigned char image buffer */
int SaveCimagePGM(unsigned char *buffer, int dimx, int dimy, char *name_file)
{
  int size = dimx*dimy;
  FILE *fp=NULL;
 
  if (name_file != NULL) {
  	if ((fp = fopen(name_file,"wb")) == NULL) {
      		(void)fprintf(stdout,"SaveCimagePGM: Impossible to open file %s\n",name_file);
      		return(0);
  	}
  	if (!fprintf(fp,"P5\n#image created by JPT tool\n%i %i\n255\n",dimx, dimy)) {
	      	(void)fprintf(stderr,"SaveCimagePGM: Write error in file %s\n",name_file);
      		(void)fclose(fp);
      		return(0);
  	}
  	if (fwrite(buffer, sizeof(unsigned char), size, fp) != (unsigned)size) {
		(void)fprintf(stdout,"SaveCimagePGM: Write error in file %s\n",name_file);
		(void)fclose(fp);
		return(0);
  	}
  	(void)fclose(fp);
  } else {
	/* standard output */
  	(void)fprintf(stdout,"P5\n#image created by JPT tool\n%i %i\n255\n",dimx, dimy);
  	(void)fwrite(buffer, sizeof(unsigned char), size, stdout);
  }

  return(1);
}

/* save in a file an unisigned char image buffer */
int SaveCimagePPM(unsigned char *buffer, int dimx, int dimy, char *name_file)
{
  int size = dimx*dimy*3;
  FILE *fp=NULL;
 
  if (name_file != NULL) {
  	if ((fp = fopen(name_file,"wb")) == NULL) {
      		(void)fprintf(stdout,"SaveCimagePPM: Impossible to open file %s\n",name_file);
      		return(0);
  	}
  	if (!fprintf(fp,"P6\n#image created by JPT tool\n%i %i\n255\n",dimx, dimy)) {
	      	(void)fprintf(stderr,"SaveCimagePPM: Write error in file %s\n",name_file);
      		(void)fclose(fp);
      		return(0);
  	}
  	if (fwrite(buffer, sizeof(unsigned char), size, fp) != (unsigned)size) {
		(void)fprintf(stdout,"SaveCimagePPM: Write error in file %s\n",name_file);
		(void)fclose(fp);
		return(0);
  	}
  	(void)fclose(fp);
  } else {
	/* standard output */
  	(void)fprintf(stdout,"P6\n#image created by JPT tool\n%i %i\n255\n",dimx, dimy);
  	(void)fwrite(buffer, sizeof(unsigned char), size, stdout);
  }

  return(1);
}

/* load float image buffer from a PFM file */
int LoadFimagePFM(char *name_file, float **buffer, int *dimx, int *dimy, int *ncol)
{
  FILE *file = NULL;
  STRING str;
  int size, current, readed, i, j;

  if (name_file != NULL) {
  	if ((file = fopen(name_file,"rb")) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePFM: File %s does not exist.\n",name_file);
      		return(0);
  	}
  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	if ((str[0] == 'P') && (str[1] == 'f')) {
		*ncol=1;
	} else {
		if ((str[0] == 'P') && (str[1] == 'F')) {
			*ncol=3;
		} else {
        		(void)fclose(file);
        		(void)fprintf(stderr,"LoadFimagePFM: bad image type %s.\n",name_file);
        		return(0);
  		}
	}
   	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	while(current=='#') { 
        	if (fgets(str, 249, file) == NULL) {
                	(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
        	if ((current = fgetc(file)) == EOF) {
                	(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
  	} 
  	if (ungetc(current,file) != (int)current) {
        	(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
        	(void)fclose(file);
        	return(0);
  	}
  	/* scan header */
  	if (fscanf(file,"%d %d\n", dimx, dimy) != 2) {
      		(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePFM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}

  	/* alloc buffer memory */
  	size = (*dimx) * (*dimy) * (*ncol);
  	if (((*buffer) = (float *)malloc(sizeof(float)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadFimagePFM: Not enough memory.\n");
    		(void)fclose(file);
    		return(0);
  	}

  	/* read buffer */
  	readed = fread(*buffer, sizeof(float), size, file);
  	if (readed == 0) {
      		(void)fprintf(stderr,"LoadFimagePFM: Bad size file %s (%d<%d)\n",name_file,readed,size);
      		(void)fclose(file);
      		return(0);
  	}
 	/* swap lines */
	for (i=0; i<(*dimy)/2; i++) {
		for (j=0; j<(*dimx)*(*ncol); j++) {
			FSWAP( (*buffer)[i*(*dimx)*(*ncol)+j],(*buffer)[((*dimy)-1-i)*(*dimx)*(*ncol)+j] );
		}
	}

  	(void)fclose(file);
  } else {
	(void)fprintf(stderr,"LoadFimagePFM: Stdin not supported.\n");
  }

  return(1);
}

/* load float image buffer from a 2 bytes PGM file */
int LoadFimagePGM(char *name_file, float **buffer, int *dimx, int *dimy)
{
  FILE *file = NULL;
  STRING str;
  int size, current, readed, numlevel, i;
  unsigned short *tmp=NULL;

  if (name_file != NULL) {
  	if ((file = fopen(name_file,"rb")) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePGM: File %s does not exist.\n",name_file);
      		return(0);
  	}
  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	if ((str[0] != 'P') || (str[1] != '5')) {
        	(void)fclose(file);
        	(void)fprintf(stderr,"LoadFimagePGM: bad image type %s.\n",name_file);
        	return(0);
  	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	while(current=='#') { 
        	if (fgets(str, 249, file) == NULL) {
                	(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
        	if ((current = fgetc(file)) == EOF) {
                	(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
  	} 
  	if (ungetc(current,file) != (int)current) {
        	(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
        	(void)fclose(file);
        	return(0);
  	}
  	/* scan header */
  	if (fscanf(file,"%d %d\n", dimx, dimy) != 2) {
      		(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	if (fscanf(file,"%d\n", &numlevel) != 1) {
      		(void)fprintf(stderr,"LoadFimagePGM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}

        if (numlevel!=65535) {
      		(void)fprintf(stderr,"LoadFimagePGM: format not supported in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}

  	/* alloc buffer memory */
  	size = (*dimx) * (*dimy);
  	if (((*buffer) = (float *)malloc(sizeof(float)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadFimagePGM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}
  	if ((tmp = (unsigned short *)malloc(sizeof(unsigned short)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadFimagePGM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}

  	/* read buffer */
  	readed = fread(tmp, sizeof(unsigned short), size, file);
  	if (readed == 0) {
      		(void)fprintf(stderr,"LoadFimagePGM: Bad size file %s (%d<%d)\n", name_file,readed,size);
      		(void)fclose(file);
      		return(0);
  	}
	for (i=0; i<size; i++) (*buffer)[i] = (float)( (((tmp[i])<<8) & 0xFF00) | (((tmp[i])>>8) & 0xFF00) );
	free(tmp);

  	(void)fclose(file);
  } else {
	(void)fprintf(stderr,"LoadFimagePGM: Stdin not supported\n");
  }

  return(1);
}

/* load float image buffer from a 2 bytes PPM file */
int LoadFimagePPM(char *name_file, float **buffer, int *dimx, int *dimy)
{
  FILE *file = NULL;
  STRING str;
  int size, current, readed, numlevel, i;
  unsigned char *tmp=NULL;

  if (name_file != NULL) {
  	if ((file = fopen(name_file,"rb")) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePPM: File %s does not exist.\n",name_file);
      		return(0);
  	}
  	if (fgets(str, 249, file) == NULL) {
      		(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	if ((str[0] != 'P') || (str[1] != '6')) {
        	(void)fclose(file);
        	(void)fprintf(stderr,"LoadFimagePPM: bad image type %s.\n",name_file);
        	return(0);
  	}
  	if ((current = fgetc(file)) == EOF) {
      		(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}
  	while(current=='#') { 
        	if (fgets(str, 249, file) == NULL) {
                	(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
        	if ((current = fgetc(file)) == EOF) {
                	(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
                	(void)fclose(file);
                	return(0);
        	}
  	} 
  	if (ungetc(current,file) != (int)current) {
        	(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
        	(void)fclose(file);
        	return(0);
  	}
  	/* scan header */
  	if (fscanf(file,"%d %d\n", dimx, dimy) != 2) {
      		(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s\n",name_file);
      		(void)fclose(file);
      		return(0);
  	}

  	if (fscanf(file,"%d\n", &numlevel) != 1) {
      		(void)fprintf(stderr,"LoadFimagePPM: Read error in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}

        if (numlevel!=65535) {
      		(void)fprintf(stderr,"LoadFimagePPM: format not supported in file %s.\n",name_file);
      		(void)fclose(file);
      		return(0);
	}

  	/* alloc buffer memory */
  	size = (*dimx) * (*dimy) * 3;
  	if (((*buffer) = (float *)malloc(sizeof(float)*size)) == NULL) {
    		(void)fprintf(stderr,"LoadFimagePPM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}
  	if ((tmp = (unsigned char *)malloc(sizeof(unsigned char)*size*2)) == NULL) {
    		(void)fprintf(stderr,"LoadFimagePPM: Not enough memory\n");
    		(void)fclose(file);
    		return(0);
  	}

  	/* read buffer */
  	readed = fread(tmp, sizeof(unsigned char), size*2, file);
  	if (readed == 0) {
      		(void)fprintf(stderr,"LoadFimagePPM: Bad size file %s (%d<%d)\n", name_file,readed,size);
      		(void)fclose(file);
      		return(0);
  	}
	/* for ign images */
	//for (i=0; i<size; i++) (*buffer)[i] = (float)( (((tmp[i])<<8) & 0xFF00) | (((tmp[i])>>8) & 0xFF00) );
	for (i=0; i<size; i++) (*buffer)[i] = (float)( (int)tmp[2*i]*256+(int)tmp[2*i+1]);
	free(tmp);

  	(void)fclose(file);
  } else {
	(void)fprintf(stderr,"LoadFimagePPM: Stdin not supported\n");
  }

  return(1);
}

/* generic call for the load of an image buffer in float from a file */
int LoadFimageColor(char *name_file, float **buffer, int *dimx, int *dimy, int *ncol)
{
  char *end=NULL;

  if (name_file == NULL) return(0);

  end = name_file+strlen(name_file)-3;
  if ((end[0] == 'p') && (end[1] == 'f') && (end[2] == 'm'))
	return(LoadFimagePFM(name_file, buffer, dimx, dimy, ncol));

  if ((end[0] == 'p') && (end[1] == 'g') && (end[2] == 'm')) {
	*ncol=1;
	return(LoadFimagePGM(name_file, buffer, dimx, dimy));
  }

  if ((end[0] == 'p') && (end[1] == 'p') && (end[2] == 'm')) {
	*ncol=3; 
	return(LoadFimagePPM(name_file, buffer, dimx, dimy));
  }

  (void)fprintf(stderr,"LoadFimageColor: File %s with unknown format.\n",name_file);

  return(0);
}

#ifdef JPEG
#include "jpeglib.h"
#include "setjmp.h"

struct my_error_mgr {
  	struct jpeg_error_mgr pub;	/* "public" fields */
  	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

int LoadCimageJPG(char *filename, unsigned char **buffer, int *dimx, int *dimy, int *numcol)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  unsigned char *p_im = NULL, *p_buf = NULL;		/* Output row buffer */
  JSAMPARRAY jpg_buf;		/* Output row buffer */
  int row_stride, i;		/* physical row width in output buffer */

  if ((dimy == NULL) || (dimx == NULL) || (buffer == NULL) || (numcol == NULL)) {
	(void)fprintf(stderr, "LoadCimageJPG: Bad input parameters\n");	
	return(0);
  }
  if (filename == NULL) {
	(void)fprintf(stderr, "LoadCimageJPG: Stdin not supported\n");	
	return(0);
  }

  if ((infile = fopen(filename, "rb")) == NULL) {
    	(void)fprintf(stderr, "LoadCimageJPG: Can't open %s.\n", filename);
    	return(0);
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    (void)fprintf(stderr, "LoadCimageJPG: Can't open %s.\n", filename);
    return(0);
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  
  row_stride = cinfo.output_width * cinfo.output_components;
  jpg_buf = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  *dimy = cinfo.output_height;
  *dimx = cinfo.output_width;
  *numcol = cinfo.output_components;		/* # of color components per pixel */

  if ((*buffer = (unsigned char *)malloc(sizeof(unsigned char)*row_stride*cinfo.output_height)) == NULL) {        
    (void)fprintf(stderr,"LoadCimageJPG: Not enough memory.\n");
    return(0);
  }
  for (i=0; i<*dimx* *dimy* *numcol; i++) (*buffer)[i]=0;

  p_im = *buffer; 
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, jpg_buf, 1);
    p_buf = jpg_buf[0]; 
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    for (i=0; i<row_stride; i++) {
      *p_im = *p_buf;
      p_im++;
      p_buf++;
    }
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  return(1);
}


int SaveCimageJPG(unsigned char *buffer, int dimx, int dimy, char *filename, int numcol)
{
  int quality = 100; 		/* quality is 100% */  
  struct jpeg_compress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * outfile=NULL;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  if (filename ==NULL) {
	(void)fprintf(stderr,"SaveCimageJPG: Stdin not supported\n");
	return(0);
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_compress(&cinfo);
    (void)fprintf(stderr, "SaveCimageJPG: Can't write %s.\n", filename);
    return(0);
  }

  jpeg_create_compress(&cinfo);

  if ((outfile = fopen(filename, "wb")) == NULL) {
    	fprintf(stderr, "SaveCimageJPG: Can't open %s\n", filename);
    	return(0);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = dimx; 	/* image width and height, in pixels */
  cinfo.image_height = dimy;
  cinfo.input_components = numcol;		/* # of color components per pixel */
  if (numcol==1) {
  	cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
  } else {
  	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  }

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
  jpeg_start_compress(&cinfo, TRUE);

  row_stride = dimx*numcol;	/* JSAMPLEs per row in buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    	row_pointer[0] = buffer+cinfo.next_scanline*row_stride;
    	(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);

  return(1);
}

#endif


#ifdef JPEG
int ImageTypeJPG(char *name_file)
{
  char *end=NULL;

  if (name_file == NULL) return(0);
  end = name_file+strlen(name_file)-3;
  end[0] = 'j';
  end[1] = 'p';
  end[2] = 'g';

  return(1);
}
#endif

int ImageTypePGM(char *name_file)
{
  char *end=NULL;

  if (name_file == NULL) return(0);
  end = name_file+strlen(name_file)-3;
  end[0] = 'p';
  end[1] = 'g';
  end[2] = 'm';

  return(1);
}

int ImageTypePPM(char *name_file)
{
  char *end=NULL;

  if (name_file == NULL) return(0);
  end = name_file+strlen(name_file)-3;
  end[0] = 'p';
  end[1] = 'p';
  end[2] = 'm';

  return(1);
}

/* generic call for the load of an image buffer from a file */
int LoadCimage(char *name_file, unsigned char **buffer, int *dimx, int *dimy)
{
  char *end=NULL;
  int numcol;

  if (name_file == NULL) return(0);

  end = name_file+strlen(name_file)-3;
  if ((end[0] == 'p') && (end[1] == 'g') && (end[2] == 'm'))
	return(LoadCimagePGM(name_file, buffer, dimx, dimy));

#ifdef JPEG
  if ((end[0] == 'j') && (end[1] == 'p') && (end[2] == 'g')) {
	if (!LoadCimageJPG(name_file, buffer, dimx, dimy, &numcol)) return(0);
	if (numcol != 1) return(0);
	 else return(1);
  }
#endif

  (void)fprintf(stderr,"LoadCimage: File %s with unknown format.\n",name_file);
  return(0);
}

/* allocation memory for the unsigned char buffer of an image */
int AllocCimage(unsigned char **buffer, int dimx, int dimy, int numcol)
{
  int size = dimx*dimy*numcol;

  if (((*buffer) = (unsigned char *)malloc(size*sizeof(unsigned char))) == NULL) {
	  (void)fprintf(stderr,"AllocCimage: Not enough memory\n");
	  return(0);
  }

  return(1);
}

/* allocation memory for the float buffer of an image */
int AllocFimage(float **buffer, int dimx, int dimy, int numcol)
{
  int size = dimx*dimy*numcol;

  if ((*buffer = (float *)malloc(size*sizeof(float))) == NULL) {
	  (void)fprintf(stderr,"AllocFimage: Not enough memory\n");
	  return(0);
  }

  return(1);
}

/* allocation memory for the float buffer of an image */
int AllocSimage(short **buffer, int dimx, int dimy, int numcol)
{
  int size = dimx*dimy*numcol;

  if ((*buffer = (short *)malloc(size*sizeof(short))) == NULL) {
	  (void)fprintf(stderr,"AllocSimage: Not enough memory\n");
	  return(0);
  }

  return(1);
}

/* generic call for the load of an image buffer from a file */
int LoadCimageColor(char *name_file, unsigned char **buffer, int *dimx, int *dimy, int *ncol)
{
  char *end=NULL;

  if (name_file == NULL) return(0);

  end = name_file+strlen(name_file)-3;
  if ((end[0] == 'p') && (end[1] == 'g') && (end[2] == 'm')) {
	*ncol =  1;
	return(LoadCimagePGM(name_file, buffer, dimx, dimy));
  }
  if ((end[0] == 'p') && (end[1] == 'p') && (end[2] == 'm')) {
	*ncol =  3;
	return(LoadCimagePPM(name_file, buffer, dimx, dimy));
  }

#ifdef JPEG
  if ((end[0] == 'j') && (end[1] == 'p') && (end[2] == 'g')) {
	return(LoadCimageJPG(name_file, buffer, dimx, dimy, ncol));
  }  
  if ((end[0] == 'J') && (end[1] == 'P') && (end[2] == 'G')) {
	return(LoadCimageJPG(name_file, buffer, dimx, dimy, ncol));
  }
#endif

  //(void)fprintf(stderr,"LoadCimageColor: File %s with unknown format.\n",name_file);
  return(0);
}

/* generic call for the save in a file of an unsigned char image buffer */
int SaveCimage(unsigned char *buffer, int dimx, int dimy, char *name_file)
{
  char *end=NULL;

  /* pgm type for std output */
  if (name_file == NULL) return(SaveCimagePGM(buffer, dimx, dimy, name_file));

  end = name_file+strlen(name_file)-3;
  if ((end[0] == 'p') && (end[1] == 'g') && (end[2] == 'm'))
	return(SaveCimagePGM(buffer, dimx, dimy, name_file));

#ifdef JPEG
  if ((end[0] == 'j') && (end[1] == 'p') && (end[2] == 'g'))
	return(SaveCimageJPG(buffer, dimx, dimy, name_file, 1));
#endif

  (void)fprintf(stderr,"SaveCimage: File %s with unknown format.\n",name_file);

  return(0);
}

/* generic call for the save in a file of an unsigned char image buffer */
int SaveCimageColor(unsigned char *buffer, int dimx, int dimy, int ncol, char *name_file)
{
  char *end=NULL;

  switch (ncol) {
	case 1:   /* gray level  */
		return(SaveCimage(buffer, dimx, dimy, name_file));
		break;
	case 3:   /* color */
  		/* ppm type for std output */
  		if (name_file == NULL) return(SaveCimagePPM(buffer, dimx, dimy, name_file));

  		end = name_file+strlen(name_file)-3;
  		if ((end[0] == 'p') && (end[1] == 'p') && (end[2] == 'm')) 
			return(SaveCimagePPM(buffer, dimx, dimy, name_file));

  		if ((end[0] == 'p') && (end[1] == 'g') && (end[2] == 'm')) 
			return(SaveCimagePGM(buffer, dimx, dimy, name_file));

		#ifdef JPEG
  		if ((end[0] == 'j') && (end[1] == 'p') && (end[2] == 'g'))
			return(SaveCimageJPG(buffer, dimx, dimy, name_file, 3));
		#endif
  		(void)fprintf(stderr,"SaveCimageColor: File %s with unknown format.\n",name_file);
		break;
	default:
                (void)fprintf(stderr,"SaveCimageColor: images with %d chanels are not supported.\n", ncol);
                return(0);
  }

  return(0);
}

/* generic call for the save in a file of an float image buffer */
int SaveFimage(float *buffer, int dimx, int dimy, char *name_file)
{
   char *end=NULL;

   /* pfm type for std output */
   if (name_file == NULL) return(SaveFimagePFM(buffer, dimx, dimy, 1, name_file));

   end = name_file+strlen(name_file)-3;
   if ((end[0] == 'p') && (end[1] == 'f') && (end[2] == 'm'))
     return(SaveFimagePFM(buffer, dimx, dimy, 1, name_file));

   (void)fprintf(stderr,"SaveFimage: File %s with unknown format.\n",name_file);

   return(0);
}

/* generic call for the save in a file of an float image buffer */
int SaveFimageColor(float *buffer, int dimx, int dimy, int ncol, char *name_file)
{
   char *end=NULL;

   /* pfm type for std output */
   if (name_file == NULL) return(SaveFimagePFM(buffer, dimx, dimy, ncol, name_file));

   end = name_file+strlen(name_file)-3;
   if ((end[0] == 'p') && (end[1] == 'f') && (end[2] == 'm'))
     return(SaveFimagePFM(buffer, dimx, dimy, ncol, name_file));

   (void)fprintf(stderr,"SaveFimageColor: File %s with unknown format.\n",name_file);

   return(0);
}

/* save in a file a gray level image buffer with colored overlay */
int SaveCimageOverlay(unsigned char *buffer, unsigned char *overlay, 
	int dimx, int dimy, char *name_file)
{
  int size = dimx*dimy, i;
  unsigned char *tmpbuffer=NULL, color;

  if (!AllocCimage(&tmpbuffer, dimx, dimy, 3)) return(0);

  for (i=0; i<size; i++) {  
	  color = overlay[i]; 
	  if (color != TRANS) {
		if (color<NUM_COLOR) { /* false color overlay */
	  		tmpbuffer[i*3] = ColorTable[color][0];
	  		tmpbuffer[i*3+1] = ColorTable[color][1];
	  		tmpbuffer[i*3+2] = ColorTable[color][2];
		} else {
	  		tmpbuffer[i*3] = color;
	  		tmpbuffer[i*3+1] = color;
	  		tmpbuffer[i*3+2] = color;
		}
	  } else {
		if (buffer!=NULL) {
			tmpbuffer[i*3] = tmpbuffer[i*3+1] = tmpbuffer[i*3+2] = buffer[i];
		} else {
			tmpbuffer[i*3] = tmpbuffer[i*3+1] = tmpbuffer[i*3+2] = 0;
		}			
	  }		
  }  

  if (!SaveCimageColor(tmpbuffer, dimx, dimy, 3, name_file)) return(0);
  free(tmpbuffer);

  return(1);
}

/* save in a file an float image buffer */
int SaveFimagePFM(float *buffer, int dimx, int dimy, int ncol, char *name_file)
{
   int size = dimx*dimy*ncol, i, j;
   FILE *fp=NULL;
   float *tmp=NULL;

   /* swap lines */
   if (!AllocFimage(&tmp,dimx,dimy,ncol)) return(0);
   for (i=0; i<dimx*dimy*ncol; i++) tmp[i]=buffer[i];
   for (i=0; i<dimy/2; i++) {
     for (j=0; j<dimx*ncol; j++) {
         FSWAP( tmp[i*dimx*ncol+j],tmp[(dimy-1-i)*dimx*ncol+j]);
     }
   }

   if (name_file != NULL) {
       if ((fp = fopen(name_file,"wb")) == NULL) {
               (void)fprintf(stdout,"SaveFimagePFM: Impossible to open file %s\n",name_file);
               return(0);
       }
     if (ncol==1) {
           if (!fprintf(fp,"Pf\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveFimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
     if (ncol==3) {
           if (!fprintf(fp,"PF\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveFimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
       if (fwrite(tmp, sizeof(float), size, fp) != (unsigned)size) {
         (void)fprintf(stdout,"SaveFimagePFM: Write error in file %s, size=%d\n",name_file,size);
         (void)fclose(fp);
         return(0);
       }
       (void)fclose(fp);
   } else {
     /* standard output */
     if (ncol==1) {
           if (!fprintf(stdout,"Pf\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveFimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
     if (ncol==3) {
           if (!fprintf(stdout,"PF\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveFimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
       (void)fwrite(tmp, sizeof(float), size, stdout);
   }
   free(tmp);

   return(1);
}

/* save in a file a short image buffer */
int SaveSimagePFM(short *buffer, int dimx, int dimy, int ncol, char *name_file)
{
   int size = dimx*dimy*ncol, i, j;
   FILE *fp=NULL;
   float *tmp=NULL;

   /* swap lines */
   if (!AllocFimage(&tmp,dimx,dimy,ncol)) return(0);
   for (i=0; i<dimx*dimy*ncol; i++) tmp[i]=(float)(buffer[i]);
   for (i=0; i<dimy/2; i++) {
     for (j=0; j<dimx*ncol; j++) {
         FSWAP( tmp[i*dimx*ncol+j],tmp[(dimy-1-i)*dimx*ncol+j]);
     }
   }

   if (name_file != NULL) {
       if ((fp = fopen(name_file,"wb")) == NULL) {
               (void)fprintf(stdout,"SaveSimagePFM: Impossible to open file %s\n",name_file);
               return(0);
       }
     if (ncol==1) {
           if (!fprintf(fp,"Pf\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveSimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
     if (ncol==3) {
           if (!fprintf(fp,"PF\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveSimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
       if (fwrite(tmp, sizeof(float), size, fp) != (unsigned)size) {
         (void)fprintf(stdout,"SaveSimagePFM: Write error in file %s, size=%d\n",name_file,size);
         (void)fclose(fp);
         return(0);
       }
       (void)fclose(fp);
   } else {
     /* standard output */
     if (ncol==1) {
           if (!fprintf(stdout,"Pf\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveSimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
     if (ncol==3) {
           if (!fprintf(stdout,"PF\n%i %i\n-1.0\n",dimx, dimy)) {
                   (void)fprintf(stderr,"SaveSimagePFM: Write error in file %s\n",name_file);
                   (void)fclose(fp);
                   return(0);
           }
     }
       (void)fwrite(tmp, sizeof(float), size, stdout);
   }
   free(tmp);

   return(1);
}


/*
 *  This Quickselect routine is based on the algorithm described in
 *  "Numerical recipes in C", Second Edition,
 *  Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
 *  This code by Nicolas Devillard - 1998. Public domain.
 *  see http://ndevilla.free.fr/median/
 */
unsigned char QuickCMedian(unsigned char *arr, int n) 
{
    int low, high;
    int median;
    int middle, ll, hh;

    low = 0 ; high = n-1 ; median = (low + high) / 2;
    for (;;) {
        if (high <= low) /* One element only */
            return arr[median] ;

        if (high == low + 1) {  /* Two elements only */
            if (arr[low] > arr[high])
                CSWAP(arr[low], arr[high]) ;
            return arr[median] ;
        }

    /* Find median of low, middle and high items; swap into position low */
    middle = (low + high) / 2;
    if (arr[middle] > arr[high])    CSWAP(arr[middle], arr[high]) ;
    if (arr[low] > arr[high])       CSWAP(arr[low], arr[high]) ;
    if (arr[middle] > arr[low])     CSWAP(arr[middle], arr[low]) ;

    /* Swap low item (now in position middle) into position (low+1) */
    CSWAP(arr[middle], arr[low+1]) ;

    /* Nibble from each end towards middle, swapping items when stuck */
    ll = low + 1;
    hh = high;
    for (;;) {
        do ll++; while (arr[low] > arr[ll]) ;
        do hh--; while (arr[hh]  > arr[low]) ;

        if (hh < ll)
        break;

        CSWAP(arr[ll], arr[hh]) ;
    }

    /* Swap middle item (in position low) back into correct position */
    CSWAP(arr[low], arr[hh]) ;

    /* Re-set active partition */
    if (hh <= median)
        low = ll;
        if (hh >= median)
        high = hh - 1;
    }
}


int MedianSmoothCimage(int dimx, int dimy, int ncol, unsigned char *buffer, int size, unsigned char *result) 
{
 int i, j, k, l, m, hsize=size/2, index, jndex;
 unsigned char *tmp=NULL;

 if ((buffer == NULL) || (result == NULL)) {
          fprintf(stderr,"MedianSmoothCimage: Bad pointers\n");
          return(0);
 }
 if (!AllocCimage(&tmp, size, size, 1)) return(0);

 for (m=0; m<ncol; m++) {
 	for (j=0; j<dimy; j++) {
		for (i=0; i<dimx; i++) {
			for (k=0; k<size; k++) {
				index=i+k-hsize;
				if (index<0) index=-index;
				if (index>=dimx) index=2*dimx-2-index;
				for (l=0; l<size; l++) {
					jndex=j+l-hsize;
					if (jndex<0) jndex=-jndex;
					if (jndex>=dimy) jndex=2*dimy-2-jndex;
					tmp[k*size+l]=buffer[(jndex*dimx+index)*ncol+m];
				}
			}
			result[(j*dimx+i)*ncol+m] = QuickCMedian(tmp,size*size);
		}
 	}
 }

 free(tmp);
	
 return(1);
}

unsigned char CMin(unsigned char *vals, int size)
{
 int i;
 unsigned char min;
 
 min=vals[0];
 for (i=1; i<size; i++) {
	if (vals[i]<min) min=vals[i];
 }

 return(min);
}

 
