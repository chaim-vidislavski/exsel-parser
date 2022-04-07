#include "excelParser.h"
#include "formulaParse.h"
#include "cellsRef.h"
#include "charactersColon.h"



/* Compares strings by first size */
int strNumFirstEqual(char* st1, char* st2)
{
	return !strncmp(st1, st2, strlen(st1));
}

/* Tester is an error type */
int IsTypeError(cell* p)
{
	if (p->type == TYPE_VALUE_ERROR || p->type == TYPE_ZERO_DIVISION_ERROR || p->type == TYPE_CIRCULAR_REFERENCE_ERROR)
		return 1;
	return 0;
}
/* parser formula Responsible for the values in the formula */
double parsFormulaVal(cell* p, int oper)
{

	if (IsTypeError(p))
		return 0;

	double num = 0;
	if (isdigit(p->stp[p->index]) || p->stp[p->index] == '-' && isdigit(p->stp[p->index + 1]))
		num = parsDigitChars(p);
	else if (extentRef(p->stp + p->index))
		num = evalRef(p);	
	
	else if (p->stp[p->index] == '(')
		num = bracketsOpen(p);
	else if (strNumFirstEqual("SUM(", p->stp + p->index))
		num = funcAverageSum(p, 1);
	else if (strNumFirstEqual("AVERAGE(", p->stp + p->index))
		num = funcAverageSum(p, 0);
	else if (strNumFirstEqual("MOD(", p->stp + p->index))
		num = funcMod(p);
	else if (strNumFirstEqual("SIN(", p->stp + p->index))
		num = funcSinCos(p, 0);
	else if (strNumFirstEqual("COS(", p->stp + p->index))
		num = funcSinCos(p, 1);
	else
	{
		p->type = TYPE_VALUE_ERROR;
		return 0;
	}

	while (p->stp[p->index] != '\0' && p->stp[p->index] != ')' && p->stp[p->index] != ',' && !IsTypeError(p))
	{
		if ((p->stp[p->index] == '+' || p->stp[p->index] == '-') && oper != OPER_EMPTY || oper == OPER_MULDIV)
			break;
		num = parsFormulaSign(p, num);
	}
	return num;
}
/* start of formula parser */
void startPars(cell* p)
{
	double numloc;
	/* skip the '=' sign */
	p->index++;
	numloc = parsFormulaVal(p, OPER_EMPTY);
	
	if (!IsTypeError(p))
	{
		if (p->stp[p->index] == '\0')
		{
			p->type = TYPE_FORMULA_DECODED;
			p->num = numloc;
			return;
		}
			p->type = TYPE_VALUE_ERROR;
	}
	p->num = 0;
}
/* Responsible for the numbers in the formula returns a number */
double parsDigitChars(cell* p)
{
	double numloc = atof(p->stp + p->index);
	p->index += lenNumberChars(p->stp + p->index);
	return numloc;
}

/*function that receives points to a string and returns the length of its numeric part*/
int lenNumberChars(char* st)
{
	if (st == 0)
		return 0;
	int i = 0, j = 0;
	while ((isdigit(st[i]) || i == 0 && st[i] == '-' || st[i] == '.' && i != 0 && j++ == 0) && st[i++] != '\0');
	return(i == 1 && st[0] == '-') ? 0 : i;
}

/* Responsible for functions average and sum (as=0 to average  or 1 to sum) */
double funcAverageSum(cell* p, int as)
{
	p->index += ((as) ? 4 : 8);
	int i = 1;
	double numloc = 0;
	while (p->stp[p->index] != ')')
	{
		numloc += parsFormulaVal(p, OPER_EMPTY);
		if (p->stp[p->index] == ',')
		{
			i++;
			p->index++;
			if(p->stp[p->index] == ')')
				p->type = TYPE_VALUE_ERROR;
		}
		if (p->stp[p->index] == '\0')
			p->type = TYPE_VALUE_ERROR;
		if (IsTypeError(p))
			return 0;

	}
	p->index++;
	return (as) ? numloc : numloc / i;
}
/* Responsible for the modulo function */
double  funcMod(cell* p)
{
	p->index += 4;
	double numloc = parsFormulaVal(p, OPER_EMPTY);
	if (p->stp[p->index] == ',')
	{
		p->index++;	
		double numlocdiv = parsFormulaVal(p, OPER_EMPTY);
		if (p->stp[p->index] == ')')
		{
			if ((int)numlocdiv == 0 && !IsTypeError(p))
				p->type = TYPE_ZERO_DIVISION_ERROR;
			if (IsTypeError(p))
				return 0;
			p->index++;
			numloc *= 10000;
			numlocdiv *= 10000;
			if ((numloc > 0) == (numlocdiv > 0) || numloc == 0)
				numloc = (double)((int)numloc % (int)numlocdiv);
			else
				numloc = (double)((int)numloc % (int)numlocdiv + numlocdiv);
			return numloc / 10000;
		}
	}
	p->type = TYPE_VALUE_ERROR;
	return 0;
}
/* Responsible for functions sin and cos (as=0 to sin  or 1 to cos) */
double  funcSinCos(cell* p, int sc)
{
	p->index += 4;
	double numloc = parsFormulaVal(p, OPER_EMPTY);
	if (p->stp[p->index] != ')' && !IsTypeError(p))
		p->type = TYPE_VALUE_ERROR;
	if (IsTypeError(p))
		return 0;
	p->index++;
	return (sc) ? cos(numloc) : sin(numloc);
}

/* Bracket manager */
double bracketsOpen(cell* p)
{
	p->index++;
	double numloc = parsFormulaVal(p, OPER_EMPTY);
	if (p->stp[p->index] == ')')
	{
		p->index++;
		return numloc;
	}
	p->type = TYPE_VALUE_ERROR;
	return 0;
}

/* Calls functions by operator characters */
double parsFormulaSign(cell* p, double num)
{
	switch (p->stp[p->index])
	{
	case '+':
		return addSign(p, num);
		break;
	case '-':
		return subSign(p, num);
		break;
	case '*':
		return mulSign(p, num);
		break;
	case '/':
		return divSign(p, num);
		break;
	default:
		//printf("(-:");
		p->type = TYPE_VALUE_ERROR;
		p->num = 0;
		return 0;
		break;
	}
}



/*perator plus */
double addSign(cell* p, double num)
{
	p->index++;
	return num + parsFormulaVal(p, OPER_ADDSUB);
}
/*perator minus */
double subSign(cell* p, double num)
{
	p->index++;
	return num - parsFormulaVal(p, OPER_ADDSUB);
}

/*perator multiplication */
double mulSign(cell* p, double num)
{
	p->index++;
	return num * parsFormulaVal(p, OPER_MULDIV);
}
/* perator Division */
double divSign(cell* p, double num)
{
	p->index++;
	double numloc = 0;
	numloc = parsFormulaVal(p, OPER_MULDIV);
	if (numloc != 0)
		return num / numloc;
	p->type = TYPE_ZERO_DIVISION_ERROR;
	return 0;
}