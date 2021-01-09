/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "sys/types.h"

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    u_int64_t it = 0; 
    struct ComplexNumber *z = newComplexNumber(0.0, 0.0);
    while (it < maxiters) {
    	struct ComplexNumber *z_sqr = ComplexProduct(z, z);
        freeComplexNumber(z);
    	struct ComplexNumber *sum = ComplexSum(z_sqr, point);
    	double abs_sum = ComplexAbs(sum);
    	if (abs_sum >= threshold) {
    		freeComplexNumber(sum);
    		freeComplexNumber(z_sqr);
		    it += 1;
		    return it;
    	}
	z = newComplexNumber(Re(sum), Im(sum));
    	freeComplexNumber(sum);
    	freeComplexNumber(z_sqr);
        it++;
    }
    freeComplexNumber(z);
    return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    u_int64_t image_width = (2 * resolution + 1);
    int i = 0;
    struct ComplexNumber *scalar = newComplexNumber(-1 * scale, scale);
    struct ComplexNumber *c_val = ComplexSum(center, scalar);
    double pixels = scale / (double) resolution;
    u_int64_t array_length = image_width * (image_width - 1);
    if (resolution == 0) {
        output[0] = MandelbrotIterations(max_iterations, center, threshold);
    }
    while (i <= array_length) {
        output[i] = MandelbrotIterations(max_iterations, c_val, threshold);
        for (int j = 1; j < image_width; j++) {
	    double inc_val = j * pixels;
	    struct ComplexNumber *s_inc = newComplexNumber(Re(scalar) + inc_val, Im(scalar));
	    freeComplexNumber(c_val);
	    c_val = ComplexSum(center, s_inc);
	    freeComplexNumber(s_inc);
            output[i + j] = MandelbrotIterations(max_iterations, c_val, threshold);
        }
	freeComplexNumber(c_val);
	freeComplexNumber(scalar);
        i += image_width;
	double inc = (double) pixels * i;
	inc = (double) inc / image_width;
	scalar = newComplexNumber(-1 * scale, scale - inc);
	c_val = ComplexSum(center, scalar);
    }
    freeComplexNumber(c_val);
    freeComplexNumber(scalar);
}
