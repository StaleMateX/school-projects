/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Jacob Xu",     /* Full name */
  "u1448572@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}


/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  int i, j, k, l;
  int avg;
  const int u = 32;

  for(i = 0; i < dim; i += u){
    for(j = 0; j < dim; j += u){
      for(k = i; k < i + u; k++){
	for(l = j; l < j + u; l++){
          avg = ((int)src[RIDX(k, l, dim)].red +
                (int)src[RIDX(k, l, dim)].green +
                (int)src[RIDX(k, l, dim)].blue) / 3;
          dest[RIDX(dim - l - 1, dim - k - 1, dim)].red = avg;
          dest[RIDX(dim - l - 1, dim - k - 1, dim)].green = avg;
          dest[RIDX(dim - l - 1, dim - k - 1, dim)].blue = avg;
	}
      }
    }
  }
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

/* 
 * Optimized function to calculate the average color values (red, green, blue) of a 3x3 pixel grid.
 * This function assumes that bounds checking is not necessary
 * so it's intended to be used on non-edge pixels for better performance.
 */
static pixel weighted_combo_motion(int dim, int i, int j, pixel *src) 
{
    int red = 0, green = 0, blue = 0;
    const int num_neighbors = 9;// Fixed number of neighbors in the 3x3 grid

    red   = src[RIDX(i + 0, j + 0, dim)].red + src[RIDX(i + 0, j + 1, dim)].red + src[RIDX(i + 0, j + 2, dim)].red +
            src[RIDX(i + 1, j + 0, dim)].red + src[RIDX(i + 1, j + 1, dim)].red + src[RIDX(i + 1, j + 2, dim)].red +
            src[RIDX(i + 2, j + 0, dim)].red + src[RIDX(i + 2, j + 1, dim)].red + src[RIDX(i + 2, j + 2, dim)].red;

    green = src[RIDX(i + 0, j + 0, dim)].green + src[RIDX(i + 0, j + 1, dim)].green + src[RIDX(i + 0, j + 2, dim)].green +
            src[RIDX(i + 1, j + 0, dim)].green + src[RIDX(i + 1, j + 1, dim)].green + src[RIDX(i + 1, j + 2, dim)].green +
            src[RIDX(i + 2, j + 0, dim)].green + src[RIDX(i + 2, j + 1, dim)].green + src[RIDX(i + 2, j + 2, dim)].green;

    blue  = src[RIDX(i + 0, j + 0, dim)].blue + src[RIDX(i + 0, j + 1, dim)].blue + src[RIDX(i + 0, j + 2, dim)].blue +
            src[RIDX(i + 1, j + 0, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue + src[RIDX(i + 1, j + 2, dim)].blue +
            src[RIDX(i + 2, j + 0, dim)].blue + src[RIDX(i + 2, j + 1, dim)].blue + src[RIDX(i + 2, j + 2, dim)].blue;

    /* Calculate the average color values */
    pixel result;
    result.red = (unsigned short) (red / num_neighbors);
    result.green = (unsigned short) (green / num_neighbors);
    result.blue = (unsigned short) (blue / num_neighbors);

    return result;
   }

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
    int i;
    int j;
    
    /* Process the main (non-edge) area of the image using the faster blur function */
    for (i = 0; i < (dim - 2); i++){
        for (j = 0; j < (dim - 2); j++){
	  dst[RIDX(i, j, dim)] = weighted_combo_motion(dim, i, j, src);
        }
    }

    /* Process the bottom edge (i from dim-2 to dim-1) using the slower, bound-checking blur */
    for (i = dim - 2; i < dim; i++){
        for (j = 0; j < dim; j++){
            dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
        }
    }

    /* Process the right edge (j from dim-2 to dim-1) using the bound-checking blur */
    for (i = 0; i < dim; i++){
        for (j = dim - 2; j < dim; j++){
            dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
        }
    }
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
