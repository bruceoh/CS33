/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "204851972",            /* UID */

    "Gwanho Oh",          /* Full name */
    "brucegwanhooh@ucla.edu",  /* Email */

    "",                   /* Leave Blank */
    ""                    /* Leave Blank */
};

//  You should modify only this file -- feel free to modify however you like!

/*
 * setup - This function runs one time, and will not be timed.
 *         You can do whatever initialization you need here, but
 *         it is not required -- don't use if you don't want to.
 */
void setup() {
  
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}


/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
     for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
      accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

typedef struct {
    int dim;
    int bottom;
    int row;
    pixel *src;
    pixel *dst;
} mutithread;

void Functhread(void * arg) {
        mutithread x =*(mutithread *) arg;
        int i;
        int j;
    for (i = x.bottom; i < (x.row); i++)
            for (j = 0; j < (x.dim); j++)
               x.dst[RIDX(i, j, x.dim)] = avg(x.dim, i, j, x.src);
}
void multismooth(int dim, pixel *src, pixel *dst) {

        mutithread smooth0 = {dim,0,(dim/4),src,dst};
        mutithread smooth1 = {dim,(dim/4),(dim/2),src,dst};
        mutithread smooth2 = {dim,(dim/2),((3*dim)/4),src,dst};
        mutithread smooth3 = {dim,((3*dim)/4),dim,src,dst};

        pthread_t thread_0, thread_1,thread_2,thread_3;

        pthread_create(&thread_0,NULL,(void *) Functhread,(void *) &smooth0);
        pthread_create(&thread_1,NULL,(void *) Functhread,(void *) &smooth1);
        pthread_create(&thread_2,NULL,(void *) Functhread,(void *) &smooth2);
        pthread_create(&thread_3,NULL,(void *) Functhread,(void *) &smooth3);

        pthread_join(thread_0,NULL);
        pthread_join(thread_1,NULL);
        pthread_join(thread_2,NULL);
        pthread_join(thread_3,NULL);
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (j = 0; j < dim; j++) {
       for (i = 0; i < dim; i++) {  
           dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
       }
    }

}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */

char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    multismooth(dim, src, dst);
}



/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    //add_smooth_function(&smooth1, smooth_descr1);
    //add_smooth_function(&smooth2, smooth_descr2);
    //add_smooth_function(&smooth3, smooth_descr3);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    
    /* ... Register additional test functions here */
}






// IGNORE EVERYTHING AFTER THIS POINT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
// You are not implementing rotate


/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
  for (j = 0; j < dim; j++)
      dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}



