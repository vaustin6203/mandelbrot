/*********************
**  Complex Numbers
**  This file contains a few functions that will be useful when performing computations with complex numbers
**  It is advised that you work on this part first.
**********************/

#include "ComplexNumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>


typedef struct ComplexNumber
{
	double real;
	double imaginary;
}ComplexNumber;

//Returns a pointer to a new Complex Number with the given real and imaginary components
ComplexNumber* newComplexNumber(double real_component, double imaginary_component)
{
    struct ComplexNumber *new_num = (struct ComplexNumber*) malloc(sizeof(ComplexNumber));
    (*new_num).real = real_component;
    (*new_num).imaginary = imaginary_component;
    return new_num;
}

//Returns a pointer to a new Complex Number equal to a*b
ComplexNumber* ComplexProduct(ComplexNumber* a, ComplexNumber* b)
{
    double first = Re(a) * Re(b);
    double second = Re(a) * Im(b);
    double third = Re(b) * Im(a);
    double fourth = Im(a) * Im(b);
    double r = first - fourth;
    double i = second + third;
    struct ComplexNumber *new_num = newComplexNumber(r, i);
    return new_num;
}

//Returns a pointer to a new Complex Number equal to a+b
ComplexNumber* ComplexSum(ComplexNumber* a, ComplexNumber* b)
{
    double first = Re(a) + Re(b);
    double second = Im(a) + Im(b);
    struct ComplexNumber *new_num = newComplexNumber(first, second);
    return new_num;
}

//Returns the absolute value of Complex Number a
double ComplexAbs(ComplexNumber* a)
{
    double r_sqr = Re(a) * Re(a);
    double i_sqr = Im(a) * Im(a);
    double sum_sqr = r_sqr + i_sqr;
    double sqr_root = sqrt(sum_sqr);
    return sqr_root;
}

void freeComplexNumber(ComplexNumber* a)
{
	free(a);
	a = NULL;
}

double Re(ComplexNumber* a)
{
	return (*a).real;
}
double Im(ComplexNumber* a)
{
	return (*a).imaginary;
}


//Contains a few tests.
int test_complex_number()
{
	ComplexNumber* a = newComplexNumber(2.0, 1.0);
	if (a == NULL)
	{
		printf("Creation of complex numbers not implemented\n");
		return 0;
	}
	ComplexNumber* b = ComplexProduct(a, a);
	if (b == NULL)
	{
		printf("Multiplication of complex numbers not implemented\n");
		free(a);
		return 0;
	}
	ComplexNumber* c = ComplexSum(b, b);
	if (c == NULL)
	{
		printf("Addition of complex numbers not implemented\n");
		free(a);
		free(b);
		return 0;
	}
	float d = ComplexAbs(c);
	if (d == 0)
	{
		printf("Absolute Value of complex numbers not implemented\n");
		free(a);
		free(b);
		free(c);
		return 0;
	}
	else if (fabsf(d - 10) < 0.0001)
	{
		printf("Sample tests for complex numbers all passed\n");
	}
	else
	{
		printf("At least one of your functions is incorrect\n");
	}
	free(a);
	free(b);
	free(c);
	return 0;
}
