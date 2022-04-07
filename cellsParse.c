#include "excelParser.h"
#include "formulaParse.h"
#include "cellsParse.h"
#include "csvDecoder.h"





/*Cell center management*/
void cellsManage()
{
	for (int i = 0; i < getSizeArr(); i++)
		parseCell(getPointCells() + i);
}

/*Secondary management of string decoding*/
void parseCell(cell* p)
{
	if (p->type != TYPE_UNDEFINE)
	{
		if(p->type== TYPE_FORMULA_IN_MIDDLE_DECODING)
			p->type = TYPE_CIRCULAR_REFERENCE_ERROR;
		return;
	}
	setCellTypeBasic(p);
	if (p->type == TYPE_NUMBER)
		p->num = atof(p->stp);
	else if (p->type == TYPE_FORMULA_TO_DECODING)
	{
		p->type = TYPE_FORMULA_IN_MIDDLE_DECODING;
		startPars(p);
	}
}



/*Classification of types of strings*/
void setCellTypeBasic(cell* p)
{
	int i, j;
	for (j = i = 0; p->stp[i] != '\0'; i++)
		if (!(isdigit(p->stp[i]) || i == 0 && p->stp[i] == '-' || p->stp[i] == '.' && i != 0 && j++ == 0))
			break;
	if (p->stp[i] == '\0')
		p->type = TYPE_NUMBER;
	else if (p->stp[0] == '=')
		p->type = TYPE_FORMULA_TO_DECODING;
	else
		p->type = TYPE_STRING;
}