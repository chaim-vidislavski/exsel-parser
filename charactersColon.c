#include "charactersColon.h"
#include "cellsRef.h"

/* Converts from a reference with a colon to a list of cells */
int changeColonToList(char * st, int start ,int colon, int end)
{

	int rowstart, columnstart, rowend, columnend, sizerow, sizecolumn, down, right, i;
	char  add[MAX_SIZE_STRING_TO_CELL] = { 0 };
	char columnstr[STRING_TEMP_REF] = { 0 }, rowstr[STRING_TEMP_REF] = { 0 };



	rowstart = stRowToNumber(st+start);
	columnstart = stColumnToNumber(st+start);

	rowend = stRowToNumber(st + colon + 1);
	columnend = stColumnToNumber(st + colon + 1);

	right = (columnstart <= columnend) ? 1 : -1;
	down = (rowstart <= rowend) ? 1 : -1;
	sizerow = rowend * down - rowstart * down + 1;
	sizecolumn = columnend * right - columnstart * right + 1;
	(st)[colon] = ',';
	
	for (i = 1; i < sizerow * sizecolumn - 1; i++)
	{

		if(writeColumnString(columnstr, i % sizecolumn * right + columnstart))
			return colon;
		if (writeRowString(rowstr, i / sizecolumn * down + rowstart))
			return colon;
		strcat(add, columnstr);
		strcat(add, rowstr);
		strcat(add, ",");
		columnstr[0] = rowstr[0] = '\0';
	}

	if (add[0] != '\0'&&(i=strlen(add)+strlen(st))<MAX_SIZE_STRING_TO_CELL)
	{
		
		strcat(add, st+colon+1);
		st[colon+1] = '\0';
		strcat(st, add);
		return i-1;
	}
	return end;
}
/* Converts a row number to a string */
int writeRowString(char* st, int row)
{
	if (row > pow(10, STRING_TEMP_REF-2))
		return 1;
	_itoa(row + 1, st, 10);
	return 0;
}
/* Converts a column number to a string */
int writeColumnString(char* st, int column)
{
	int i, size = 1;

	for (i = 26; column >= i; size++)
	{
		column -= i;
		i *= i;
	}
	if (size >= STRING_TEMP_REF - 1)
		return 1;
	writeColumnStringRecur(st, column, size);
	st[size] = '\0';
	return 0;
}
/* Auxiliary function for converting a column number*/
void writeColumnStringRecur(char* st, int column, int size)
{
	int i = column % 26;
	if (--size)
		writeColumnStringRecur(st, column / 26, size);
	st[size] = i + 'A';
}
