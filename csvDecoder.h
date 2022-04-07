#include "excelParser.h"
#define MAX_CHAR_IN_CSV 1000
#define NO_VALUE -1
cell* createsCells(char* dir);
int cntArrCellSize(char* fcsv);
int setCells(cell* p, char* stcsv);
void freeCells();

cell* pointCells(cell* p, int setget);
void setPointCells(cell* p);
cell* getPointCells();

int rebootCell(cell* p, char* st, int row, int column);

int sizeArr(int num, int setget);
void setSizeArr(int num);
int getSizeArr();

