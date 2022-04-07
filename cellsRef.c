#include "excelParser.h"
#include "cellsRef.h"
#include "cellsParse.h"
#include "csvDecoder.h "
#include "formulaParse.h"


/* Checks whether and how many characters are of the reference type */
int extentRef(char* st)
{
	int i = 0;
	if (isalpha(st[i]))
	{
		while (isalpha(st[++i]));
		if (isdigit(st[i]))
		{
			while (isdigit(st[++i]));
			if (st[i] != ':'&& !isalpha(st[i]))
				return i;
		}
	}
	return 0;
}
/*Checks back whetherand how many characters are of the reference point reference number*/
int extentRefReverse(char* st,int i)
{

	if (isdigit(st[i]))
	{
		while (--i>0&&isdigit(st[i]));
		if (i > 0 && isalpha(st[i]))
		{
			while (--i > 0 && isalpha(st[i]));
			if (!isdigit(st[i]))
				return i+1;
		}
	}
	return 0;
}

/* Returns the reference value of characters */
double evalRef(cell* p)
{
	int before=p->index;
	p->index += extentRef(p->stp + p->index);
	return referenceManage(p, before);
}
/*Main reference management*/
double referenceManage(cell* p, int i)
{
	int rowcolumn[2] = { EMPTY_REF,EMPTY_REF };
	cell* pointdirected;
	whereReference(p, i, rowcolumn);
	pointdirected = findcell(rowcolumn[0], rowcolumn[1]);
	if (!pointdirected)
		return 0;
	parseCell(pointdirected);
	if (pointdirected->type == TYPE_STRING)
	{
		p->type = TYPE_VALUE_ERROR;
		return 0;
	}
	p->type = pointdirected->type;
	return pointdirected->num;
}


/*put in array (cr) numbers of row and column*/
void whereReference(cell* p, int i, int rowcolumn[2])
{
	int row = 0, column = 0, j = -1;
	while (isalpha(p->stp[i]))
	{
		column = column * 26 + p->stp[i] - ((p->stp[i] < 'a') ? 'A' : 'a');
		i++;
		j++;
	}
	while (j > 0)
		column += (int)pow(26, j--);

	while (isdigit(p->stp[i]))
		row = row * 10 + p->stp[i++] - '0';
	row--;
	if (row >= MAX_ROW || column >= MAX_COLUMN)
		p->type = TYPE_EXCEEDING_CELL_SIZE_ERROR;
	else
	{
		rowcolumn[0] = row;
		rowcolumn[1] = column;
	}
}

/* Returns the number row from character reference */
int stRowToNumber(char* st)
{
	int i = -1, row= EMPTY_REF;
	while (isalpha(st[++i]));
	row = atoi(st + i)-1;
	if (row < MAX_ROW)
		return row;
	return EMPTY_REF;
}

/* Returns the number column from character reference */
int stColumnToNumber(char* st)
{
	int i =0,column=st[i] - 'A';
	while (isalpha(st[++i]))
		column = column * 26+st[i]-'A';
	while (--i)
		column += (int)pow(26, i);
	if (column < MAX_COLUMN)
		return column;
	return EMPTY_REF;
}




/* Returns cell reference given row and column numbers */
cell* findcell(int row, int column)
{
	int i = 0, low = 0, high = getSizeArr() - 1;
	cell* pcell = getPointCells();
	while (low < high)
	{
		i = (high + low) / 2;
		if (pcell[i].addressRow > row || pcell[i].addressRow == row && pcell[i].addressColumn > column)
			high = i - 1;
		else
			low = i + 1;
	}
	if (pcell[low].addressRow == row && pcell[low].addressColumn == column)
		return pcell + low;
	else if (low && pcell[low - 1].addressRow == row && pcell[low - 1].addressColumn == column)
		return pcell + low - 1;
	else
		return 0;
}





