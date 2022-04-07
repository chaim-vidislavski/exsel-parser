#include "excelParser.h"
#include "csvDecoder.h"
#include "cellsParse.h"
#include "cellsPrinter.h"
#include "cellsRef.h"



int main(int argc, char* argv[])
{
	if (argc != 2 && 0)
	{
		printf("The number of arguments is incorrect");
		return 0;
	}

	setPointCells(createsCells(argv[1]));
	if (getPointCells() == NULL)
		exit(((getSizeArr() == NO_VALUE) ? 0 : 1));

	cellsManage();

	printCells();

	freeCells();


	return 0;
}
