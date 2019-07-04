#pragma once

#ifndef EQUATION
#define EQUATION

//该文件包括公式的存储结构，处理方式等内容

struct complex
{
	double real = 0;
	double imag = 0;
};

struct head
{
	struct complex result;
	struct child *start;			//指向算式的第一个节点
	struct child *tail;				//指向算式的第最后一个节点
	struct child *bra;				//指向第一个括号
};

struct child
{
	struct complex C;				//复数 C
	char oper = 'N';				//操作符oper
	struct child *next;				//指向下一个子节点，当下一个节点是括号算式时不赋值
	int bra_n;						//括号状态
};

void child_value(child *P, double r, double i, char o);

void calc(struct head *equation);
void calc_bra(struct child *bra_head, struct head *equation);

void After(struct child *P);

void comInit(complex* C, double r, double i);
struct complex complexA(struct complex c1, struct complex c2);
struct complex complexA(struct complex c1, double n);
struct complex complexI(struct complex c1, struct complex c2);
struct complex complexI(struct complex c1, double n);
struct complex complexI(double n, struct complex c1);
struct complex complexM(struct complex c1, struct complex c2);
struct complex complexD(struct complex c1, struct complex c2);

void child_value(child *P, double r, double i, char o)
{
	P->C.real = r;
	P->C.imag = i;
	P->oper = o;
}

void comInit(complex* C, double r, double i)
{
	C->real = r;
	C->imag = i;
}

//针对括号外等式的计算函数
void calc(struct head *equation)
{
	complex tmp;
	struct child *P = equation->start;
	while (P->next != equation->start)
	{
		switch (P->oper)
		{
		case '*':
			P->C = complexM(P->C, P->next->C);
			After(P);
			break;
		case '/':
			P->C = complexD(P->C, P->next->C);
			After(P);
			break;
		default:
			P = P->next;
			break;
		}
	}
	P = equation->start;
	while (P->next != equation->start)
	{
		switch (P->oper)
		{
		case '+':
			P->C = complexA(P->C, P->next->C);
			After(P);
			break;
		case '-':
			P->C = complexI(P->C, P->next->C);
			After(P);
			break;
		default:	break;
		}
	}
	equation->tail = equation->start;
	equation->result = equation->start->C;
	equation->start->C = tmp;
	equation->start->oper = '+';
}

void calc_bra(struct child *bra_head, struct head *equation)
{
	struct child *P = bra_head->next;
	while (P->bra_n >= 0)
	{
		switch (P->oper)
		{
		case '*':
			P->C = complexM(P->C, P->next->C);
			After(P);
			break;
		case '/':
			P->C = complexD(P->C, P->next->C);
			After(P);
			break;
		default:
			P = P->next;
			break;
		}
	}
	P = bra_head->next;
	while (P->bra_n >= 0)
	{
		switch (P->oper)
		{
		case '+':
			P->C = complexA(P->C, P->next->C);
			After(P);
			break;
		case '-':
			P->C = complexI(P->C, P->next->C);
			After(P);
			break;
		default:	break;
		}
	}
	P->bra_n = 0;
	bra_head->next->bra_n ++;
	equation->tail = P;
}

void After(struct child *P)
{
	struct child *tmp = P->next;
	P->oper = tmp->oper;
	P->next = tmp->next;
	P->bra_n = tmp->bra_n;
	free(tmp);
}

struct complex complexA(struct complex c1, struct complex c2)
{
	struct complex v;
	v.real = c1.real + c2.real;
	v.imag = c1.imag + c2.imag;
	return v;
}
struct complex complexA(struct complex c1, double n)
{
	struct complex v;
	v.real = c1.real + n;
	v.imag = c1.imag;
	return v;
}
struct complex complexI(struct complex c1, struct complex c2)
{
	struct complex v;
	v.real = c1.real - c2.real;
	v.imag = c1.imag - c2.imag;
	return v;
}
struct complex complexI(struct complex c1, double n)
{
	struct complex v;
	v.real = c1.real - n;
	v.imag = c1.imag;
	return v;
}
struct complex complexI(double n, struct complex c1)
{
	struct complex v;
	v.real = n - c1.real;
	v.imag = -c1.imag;
	return v;
}
struct complex complexM(struct complex c1, struct complex c2)
{
	struct complex v;
	v.real = c1.real*c2.real - c1.imag*c2.imag;
	v.imag = c1.real*c2.imag + c1.imag*c2.real;
	return v;
}
struct complex complexD(struct complex c1, struct complex c2)    //c1/c2
{
	struct complex v;
	double abs_sq = c2.real*c2.real + c2.imag*c2.imag;
	v.real = (c1.real*c2.real + c1.imag*c2.imag) / abs_sq;
	v.imag = (c1.imag*c2.real - c1.real*c2.imag) / abs_sq;
	return v;
}

#endif