/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    double base = finalscale / initialscale;
    double denom = (double) framecount - 1.0;
    double exp = 1.0 / denom;
    double r = pow(base, exp);
    double scale, power;
    u_int64_t size = 2 * resolution + 1;
    for (int i = 1; i <= framecount; i++) {
	power = pow(r, i - 1);
    	scale = initialscale * power;
	output[i - 1] = (u_int64_t *) malloc(size * size * sizeof(u_int64_t));
    	Mandelbrot(threshold, max_iterations, center, scale, resolution, output[i - 1]);
    }
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	double threshold;
	ComplexNumber* center;
	u_int64_t max_iterations, resolution;

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	double initialscale = atof(argv[5]);
	double finalscale = atof(argv[6]);
	int framecount = atof(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);
	char* output_folder = argv[9];
	char* colorfile = argv[10];
	u_int64_t size = (int) 2 * resolution + 1;

	if (threshold <= 0 || finalscale <= 0 || max_iterations <= 0 || initialscale <= 0) {
		printf("The threshold, initialscale, finalscale, and max_iterations must be > 0\n");
		printUsage(argv);
		return 1;
	} else if (framecount > 10000 || framecount <= 0) {
		printf("The framecount must be > 0 and <= 10,000\n");
		printUsage(argv);
		return 1;
	} else if (resolution < 0) {
		printf("The resolution can not < 0\n");
		printUsage(argv);
		return 1;
	} else if (framecount == 1 && initialscale != finalscale) {
		printf("When framecount = 1, intialscale must = finalscale\n");
		printUsage(argv);
		return 1;
	}
	
	u_int64_t** output;
	output = malloc(framecount * sizeof(u_int64_t*));
	if (output == NULL) {
		printf("Unable to allocate %lu bytes for output\n", framecount * sizeof(u_int64_t));
		return 1;
	}

	int colorcount = 0;
	uint8_t** colormap;
	colormap = FileToColorMap(colorfile, &colorcount);
	if (colormap == NULL) {
		printf("colorfile is in wrong fromat or there is not enough memory for colormap\n");
		return 1;
	}

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	FILE* file_frame;
	char string_name[255];
	u_int64_t num_its;
	uint8_t* zero_color;
	zero_color = (uint8_t*) malloc(sizeof(uint8_t) * 3);
	zero_color[0] = 0;
	zero_color[1] = 0;
	zero_color[2] = 0;
	for (int i = 0; i < framecount; i++) {
		if (i < 10) {
			sprintf(string_name, "%s/frame0000%d.ppm", output_folder, i);
		} else if (i >= 10 && i < 100) {
			sprintf(string_name, "%s/frame000%d.ppm", output_folder, i);		
		} else if (i >= 100 && i < 1000) {
			sprintf(string_name, "%s/frame00%d.ppm", output_folder, i);
		} else if (i >= 1000 && i < 10000) {
			sprintf(string_name, "%s/frame0%d.ppm", output_folder, i);
		}
		file_frame = fopen(string_name, "w+");
		fprintf(file_frame, "P6 %lu %lu 255\n", size, size);
		for (int j = 0; j < (size * size); j++) {
			num_its = output[i][j];
			if (num_its > colorcount) {
				num_its = num_its % colorcount;
			}
			if (num_its == 0) {
				fwrite(zero_color, 1, 3, file_frame);
			} else {
				fwrite(colormap[num_its - 1], 1, 3, file_frame);
			}
		}
		free(output[i]);
		fclose(file_frame);
	}
	
	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	for (int t = 0; t < colorcount; t++) {
		free(colormap[t]);
	}
	free(colormap);
	free(output);
	free(zero_color);
	freeComplexNumber(center);
	return 0;
}

