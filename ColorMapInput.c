
/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"
#include <inttypes.h>

/** Frees all malloced data if colorfile is malformed. **/
void FreeData(uint8_t** color_map, int num_its) {
	for (int s = 0; s <= num_its; s++) {
		free(color_map[s]);
	}
	free(color_map);
}
/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE* file;
	uint8_t** colors;
	uint8_t* rgb;
	int index = 0;
	file = fopen(colorfile, "r");
	if (file == NULL) {
		fclose(file);
		return colors;
	}
	fscanf(file, "%i\n", colorcount);
	if (colorcount == NULL|| *colorcount < 1) {
		fclose(file);
		return colors;
	}
	colors = malloc(sizeof(uint8_t*) * (*colorcount));
	rgb = malloc(sizeof(uint8_t) * 3);
	if (colors == NULL || rgb == NULL) {
		free(colors);
		free(rgb);
		fclose(file);
		return colors;
	}
	while (fscanf(file, "%hhi %hhi %hhi/n", &rgb[0], &rgb[1], &rgb[2]) == 3) {
		 if (index == *colorcount) {
			fclose(file);
			return colors;
		} else if (rgb[0] < 0 || rgb[0] > 255) {
			FreeData(colors, index);
			fclose(file);
			return colors;
		} else if (rgb[1] < 0 || rgb[1] > 255) {
			FreeData(colors, index);
			fclose(file);
			return colors;
		} else if (rgb[2] < 0 || rgb[2] > 255) {
			FreeData(colors, index);
			fclose(file);
			return colors;
		}
		colors[index] = rgb;
		rgb = malloc(sizeof(uint8_t) * 3);
		index++;
	}
	if (index < *colorcount) {
		FreeData(colors, index);
	} else {
		free(rgb);
	}
	fclose(file);
	return colors;
}
