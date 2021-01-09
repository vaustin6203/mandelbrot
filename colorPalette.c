/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}
//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	if ((width < 1) | (heightpercolor < 1)) {
		return 1;
	}
	FILE* file;
	int num_colors = 0;
	uint8_t** color_array;
	color_array = FileToColorMap(colorfile, &num_colors);
	if (color_array == NULL) {
		return 1;
	}
	file = fopen(outputfile, "w");
	fprintf(file, "P3 %d %d 255\n", width, heightpercolor * num_colors);
	for (int i = 0; i < num_colors; i++) {
		for (int t = 0; t < heightpercolor; t++) {
			for (int j = 0; j < width - 1; j++) {
			fprintf(file, "%d %d %d ", color_array[i][0], color_array[i][1], color_array[i][2]);
			}
		    fprintf(file, "%d %d %d\n", color_array[i][0], color_array[i][1], color_array[i][2]);
		}
		free(color_array[i]);
	}
	fclose(file);
	free(color_array);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	if ((width < 1) | (heightpercolor < 1)) {
		return 1;
	}
	FILE* file;
	int colors_num;
	uint8_t** color_array;
	color_array = FileToColorMap(colorfile, &colors_num);
	if (color_array == NULL) {
		return 1;
	}
	file = fopen(outputfile, "w");
	fprintf(file, "P6 %d %d 255\n", width, heightpercolor * colors_num);
	for (int i = 0; i < colors_num; i++) {
		for (int t = 0; t < (heightpercolor * width); t++) {
			fwrite(color_array[i], 1, 3, file);
		}
		free(color_array[i]);
	}
	fclose(file);
	free(color_array);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




