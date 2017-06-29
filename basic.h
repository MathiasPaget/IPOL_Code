/*
 *	
 *      
 *	basic defines
 *
 *      Jean-Philippe Tarel
 *      e-mail: Jean-Philippe.Tarel@inrets.fr
 *
 *	18/01/2000 LCPC/INRETS copyright
 *       
 */

#ifndef basic_h
#define basic_h

typedef struct Stack {
   int Num, Max;
   int *Begin;
} Stack, *pStack;

/* basic macros, types and variables */
#define ABS(x) (((x) > 0) ? (x) : -(x))
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define SGN(x) (((x) > 0) ? 1 : -1)

typedef char STRING[512];

#define F_PI 3.141593653589f
#define D_PI 3.1415926535897932384626433832795

#define FALSE 0
#define TRUE 1

#define CSWAP(a,b) { register unsigned char t=(a);(a)=(b);(b)=t;}
#define ISWAP(a,b) { register int t=(a);(a)=(b);(b)=t;}
#define FSWAP(a,b) { register float t=(a);(a)=(b);(b)=t;}
#define DSWAP(a,b) { register double t=(a);(a)=(b);(b)=t;}


#endif

