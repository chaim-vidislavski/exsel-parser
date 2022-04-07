#include "excelParser.h"
#define STRING_TEMP_REF 10

int changeColonToList(char* st, int start, int colon, int end);
int writeColumnString(char* st, int column);
int writeRowString(char* st, int row);
void writeColumnStringRecur(char* st, int column, int size);