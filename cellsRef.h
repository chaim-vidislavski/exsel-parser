#include "excelParser.h"

#define MAX_ROW 1048576
#define MAX_COLUMN 16384

#define EMPTY_REF -1

void whereReference(cell *p, int i, int rowcolumn[2]);

int stRowToNumber(char* st);
int stColumnToNumber(char* st);
double referenceManage(cell* p, int q);
cell* findcell(int row, int column);
double evalRef(cell* p);
int extentRef(char* st);
int extentRefReverse(char* st, int i);
