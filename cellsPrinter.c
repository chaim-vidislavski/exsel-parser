#include "excelParser.h"
#include "cellsPrinter.h"
#include "csvDecoder.h"
#include "formulaParse.h"

/*Prints cells*/
void printCells()
{
	cell* p = getPointCells();
	int size = getSizeArr();
	for (int i = 0 ; i <size; i++)
	{


		printf("cell %d\naddress ", i + 1);

		printAddressColumn(p[i].addressColumn);
		printf("%d\nstring is %s", p[i].addressRow + 1, p[i].stp);
		if (!IsTypeError(p+i)&&p[i].type!= TYPE_STRING)
			printf("\nval = %g", p[i].num);
		else
			printStatusError(p[i].type);
		printf("\n-----------------------\n\n");
	}
}
/*Cell Printer Returns cell caption by number for prints*/
void printAddressColumn(int num)
{
	int i, size = 1;
	for (i = 26; num >= i; size++)
	{
		num -= i;
		i *= i;
	}
	printAddressColumnRecur(num, size);
}
/*Help print cell caption*/
void printAddressColumnRecur(int num, int size)
{
	int i = num % 26;
	if (--size)
		printAddressColumnRecur(num / 26, size);
	printf("%c", i + 'A');
}

/*error print*/
void printStatusError(int type)
{
	switch (type)
	{
	case TYPE_EXCEEDING_CELL_SIZE_ERROR:
		printf("\nUnrecognized referral error");
		break;

	case TYPE_VALUE_ERROR:
		printf("\nerror value");
		break;
	case TYPE_ZERO_DIVISION_ERROR:
		printf("\nzero division error");
		break;
	case TYPE_CIRCULAR_REFERENCE_ERROR:
		printf("\ncircular reference error");
		break;

	}
}