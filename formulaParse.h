#include "excelParser.h"

int IsTypeError(cell* p);
int strNumFirstEqual(char* st1, char* st2);

double parsDigitChars(cell* p);
int lenNumberChars(char* st);

double parsFormulaVal(cell* p, int oper);
void startPars(cell* p);
double bracketsOpen(cell* p);
double funcAverageSum(cell* p, int as);
double funcMod(cell* p);
double  funcSinCos(cell* p, int sc);

double parsFormulaSign(cell* p, double num);
double addSign(cell* p, double num);
double subSign(cell* p, double num);
double mulSign(cell* p, double num);
double divSign(cell* p, double num);


#define OPER_ADDSUB 1
#define OPER_MULDIV 2
#define OPER_EMPTY 3
#define OPER_BRACKETS 4