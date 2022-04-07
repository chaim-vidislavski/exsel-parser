#ifndef EXCELP_PARSER_H
#define EXCELP_PARSER_H


#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE_STRING_TO_CELL 1000
#define DEFAULT_SIZE_STRING_TO_CELL  50

#define TYPE_UNDEFINE 0
#define TYPE_FORMULA_TO_DECODING 2
#define TYPE_FORMULA_IN_MIDDLE_DECODING 3
#define TYPE_FORMULA_DECODED 4
#define TYPE_STRING 5
#define TYPE_NUMBER 6
#define TYPE_EXCEEDING_CELL_SIZE_ERROR 12
#define TYPE_VALUE_ERROR 13
#define TYPE_ZERO_DIVISION_ERROR 14
#define TYPE_CIRCULAR_REFERENCE_ERROR 15

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include <string.h>
#include <math.h>
#include <ctype.h> 

typedef struct {
	int addressRow;
	int addressColumn;
	char str[DEFAULT_SIZE_STRING_TO_CELL], * stp;
	int type;
	double num;
	int index;
}cell;







#endif /* ifdef EXCELP_PARSER_H */



