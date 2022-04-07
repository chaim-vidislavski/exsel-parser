#include "excelParser.h"
#include"csvDecoder.h"
#include "cellsParse.h"
#include "cellsRef.h"
#include "charactersColon.h"

/*Count how many cells are needed for the program*/
int cntArrCellSize(char* stcsv)
{
	char incell = 0, inwrap = 0;
	int cnt = 0,i=0;
	while (stcsv[i] != '\0')
	{
		if (!incell && stcsv[i] != ',' && stcsv[i] != '\n')
		{
			cnt++;
			incell = 1;
		}
		if (stcsv[i] == '"')
			inwrap = !(inwrap);
		if (incell && !inwrap && (stcsv[i] == ',' || stcsv[i] == '\n') )
			incell = 0;
		i++;
	}
	return cnt;
}


/*Runs the entire file and creates cells*/
cell* createsCells(char* dir)
{
	FILE* fcsv = fopen(dir, "rt");
	cell* p = NULL;
	char stcsv[MAX_CHAR_IN_CSV] = {0};
	

	if (fcsv == NULL)
	{
		printf("Error opening file");
		return NULL;
	}	
	
	int i = 0;
	while (fgets(stcsv + i, MAX_CHAR_IN_CSV - i - 1, fcsv) != NULL && MAX_CHAR_IN_CSV - i - 1 > 1)
		i = strlen(stcsv);

	char last = fgetc(fcsv);
	fclose(fcsv);
	if (last !=EOF)
	{
		printf("error The file is too large for the array you assigned  (in MAX_CHAR_IN_CSV)");
		return NULL;
	}

	setSizeArr(cntArrCellSize(stcsv));

	if (getSizeArr()==0)
	{
		printf("The file is empty");
		setSizeArr(NO_VALUE);
		return NULL;
	}
	
	p = (cell*)malloc(sizeof(cell) * (getSizeArr()));
	if (p == NULL)
	{
		printf("Error There is not enough free memory");
		return NULL;
	}
	if (setCells(p, stcsv))
		return NULL;
	return p;
}
/* Initializes cells */
int rebootCell(cell* p,char * st,int row,int column)
{
	int len = strlen(st);

	p->addressColumn = column;
	p->addressRow = row;
	p->type = TYPE_UNDEFINE;
	p->num = 0;
	p->index = 0;

	if (st[0] == '"')
	{	
		st++;
		len -= 2;
		st[len] = '\0';
	}
	if (len < DEFAULT_SIZE_STRING_TO_CELL)
	{
		strcpy(p->str, st);
		p->stp = p->str;
		return 0;
	}
	p->str[0] = '\0';
	p->stp = (char*)malloc(sizeof(char) * (len + 1));
	if (p->stp == NULL)
	{
		printf("dynamic memory allocation erorr");
		return 1;
	}
	strcpy(p->stp, st);
	return 0;
}

/*Puts the string in the cell and updates a row and column number and initializes values*/
int setCells(cell* p,char* stcsv)
{
	char str[MAX_SIZE_STRING_TO_CELL];
	int i = 0, j = 0,k=0,l=0, row = 0, column = 0, flag = 0, flagcolon = 0,startcolon,endcolon,colon;
	while (stcsv[k] != '\0')
	{

		if ((str[j] = stcsv[k]) == '"'&& !flagcolon)
			flag = !(flag);
		//sender change from colon reference to list 
		if (str[j] == ':' && (str[0] == '='||str[0]=='"'&&str[1]=='='))
			if((l = endcolon = extentRef(stcsv + k + 1)) != 0 && (startcolon = extentRefReverse(str, j - 1)) != 0)
				{
					colon = j;
					flagcolon = 1;
				}
		if (flagcolon && l-- == 0)
		{
			str[++j] = '\0';
			j = changeColonToList(str, startcolon, colon, endcolon+colon);
			flagcolon = 0;
		}
	
		if (!flagcolon&&((!flag) && stcsv[k] == ',' || stcsv[k] == '\n'))
		{
			str[j] = '\0';
			if (j > 0)
			{				
				if (rebootCell(p + i, str, row, column))
					return 1;
				i++;
				j = 0;
				str[0] = '\0';
			}
			if (stcsv[k] == ',')
				column++;
			if (stcsv[k] == '\n')
			{
				column = 0;
				row++;
			}
		}
		else
			j++;
		if (j + 2 >= MAX_SIZE_STRING_TO_CELL)
		{
			printf("error MAX SIZE STRING TO CELL Not enough");
			return 1;
		}
		k++;	
	}
	return 0;
}

/*free Memory*/
void freeCells()
{
	cell* p = getPointCells();
	int size = getSizeArr();
	for (int i = 0; i < size; i++)
		if ((p + i)->str[0] == 0 && (p + i)->stp && (p + i)->stp[0])
			free((p + i)->stp);
	free(p);
}
/* Static variable to the size of the array of cells*/
int sizeArr(int num, int setget)
{
	static int sizearr = 0;
	if (setget == 0)
		sizearr = num;
	return sizearr;
}

/* seter for sizearr variable */
void setSizeArr(int num)
{
	sizeArr(num, 0);
}
/* geter for sizearr variable */
int getSizeArr()
{
	return sizeArr(0, 1);
}

/* Pointer to the array of cells */
cell* pointCells(cell* p, int setget)
{
	static cell* pointcells = NULL;
	if (setget == 0)
		pointcells = p;
	return pointcells;
}
/* seter for Pointer cells */
void setPointCells(cell* p)
{
	pointCells(p, 0);
}
/* geter for Pointer cells */
cell* getPointCells()
{
	return pointCells(NULL, 1);
}

