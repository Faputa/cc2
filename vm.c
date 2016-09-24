//字节码打印与解释

#include "cc.h"
#include <stdio.h>
#include <malloc.h>

int *emit, *e, *data;

void vm_init(void) {
	static int is_init = 0;
	if(!is_init) {
		emit = e = (int*)malloc(MAXSIZE * sizeof(int));
		data = (int*)malloc(MAXSIZE * sizeof(int));
		is_init = 1;
	}
}

static int* print_emit(int *i) {
	#define PRINT_REG(x) { \
		if(x == IP) printf("IP "); \
		else if(x == BP) printf("BP "); \
		else if(x == SP) printf("SP "); \
		else if(x == AX) printf("AX "); \
	}
	if(*i == PUSH) {
		printf("PUSH ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
	} else if(*i == POP) {
		printf("POP  ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
	} else if(*i == SET) {
		printf("SET  ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
		printf("%d", *++i);
	} else if(*i == INC) {
		printf("INC  ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
		printf("%d", *++i);
	} else if(*i == DEC) {
		printf("DEC  ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
		printf("%d", *++i);
	} else if(*i == JMP) {
		printf("JMP  ");
		printf("[%d]", *++i);
	} else if(*i == JZ) {
		printf("JZ   ");
		printf("[%d]", *++i);
	} else if(*i == MOV) {
		printf("MOV  ");
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
		if(*++i >= IP && *i <= AX) PRINT_REG(*i)
		else printf("[%d]", *i);
	}
	else if(*i == ADD) printf("ADD");
	else if(*i == SUB) printf("SUB");
	else if(*i == MUL) printf("MUL");
	else if(*i == DIV) printf("DIV");
	else if(*i == MOD) printf("MOD");
	else if(*i == ASS) printf("ASS");
	else if(*i == EQ) printf("EQ");
	else if(*i == GT) printf("GT");
	else if(*i == LT) printf("LT");
	else if(*i == AND) printf("AND");
	else if(*i == OR) printf("OR");
	else if(*i == NOT) printf("NOT");
	else if(*i == AG) {
		printf("AG   ");
		printf("%d", *++i);
	} else if(*i == AL) {
		printf("AL   ");
		printf("%d", *++i);
	} else if(*i == VAL) printf("VAL");
	else if(*i == CALL) {
		printf("CALL ");
		printf("%d", *++i);
	} else if(*i == CAPI) {
		printf("CAPI ");
		printf("%d", *++i);
	} else if(*i == EXIT) printf("EXIT");
	else printf("[%d]", i - emit);
	return i;
	#undef PRINT_REG
}

void run_vm(int src, int debug) {
	//print
	if(src) {
		for(int *i = emit; i < e; i++) {
			printf("%d\t", i - emit);
			i = print_emit(i);
			printf("\n");
		}
	}
	//run..
	*(SP + data) = *(BP + data) = AX + 1; //sp = AX + 1;
	*(IP + data) = 0;//int *ax = NULL; //ip = 0;
	while(1) {
		if(debug) {
			printf("\n_%d_%d_%d_%d_\t", *(IP + data), *(BP + data), *(SP + data), *(data + AX));
			print_emit(emit + *(IP + data));
		}
		int i = *(emit + (*(IP + data))++);
		if(i == PUSH) {
			int opr = *(emit + (*(IP + data))++);
			*(data + (*(SP + data))++) = *(data + opr);//printf(" %d",*(data+*(SP+data)-1));
		} else if(i == POP) {
			int opr = *(emit + (*(IP + data))++);
			*(data + opr) = *(data + (--*(SP + data)));//printf(" %d",*(data+*(SP+data)));
		} else if(i == SET) {
			int opr1 = *(emit + (*(IP + data))++);
			int opr2 = *(emit + (*(IP + data))++);
			*(data + opr1) = opr2;
		} else if(i == INC) {
			int opr1 = *(emit + (*(IP + data))++);
			int opr2 = *(emit + (*(IP + data))++);
			*(data + opr1) += opr2;
		} else if(i == DEC) {
			int opr1 = *(emit + (*(IP + data))++);
			int opr2 = *(emit + (*(IP + data))++);
			*(data + opr1) -= opr2;
		} else if(i == JMP) {
			int opr = *(emit + (*(IP + data))++);
			*(IP + data) = opr;
		} else if(i == JZ) { //jump if zero
			int opr = *(emit + (*(IP + data))++);
			if(!*(data + AX)) *(IP + data) = opr;
		} else if(i == MOV) {
			int opr1 = *(emit + (*(IP + data))++);
			int opr2 = *(emit + (*(IP + data))++);
			*(data + opr1) = *(data + opr2);
		} else if(i == ADD) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr1 + opr2;
		} else if(i == SUB) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 - opr1;
		} else if(i == MUL) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr1 * opr2;
		} else if(i == DIV) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 / opr1;
		} else if(i == MOD) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 % opr1;
		} else if(i == ASS) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + opr2) = opr1;
		} else if(i == EQ) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 == opr1;
		} else if(i == GT) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 > opr1;
		} else if(i == LT) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 < opr1;
		} else if(i == AND) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 && opr1;
		} else if(i == OR) {
			int opr1 = *(data + AX);
			int opr2 = *(data + (--*(SP + data)));
			*(data + AX) = opr2 || opr1;
		} else if(i == NOT) {
			int opr = *(data + AX);
			*(data + AX) = !opr;
		} else if(i == AG) { //address global
			int opr = *(emit + (*(IP + data))++);
			*(data + AX) = opr;
		} else if(i == AL) { //address local
			int opr = *(emit + (*(IP + data))++);
			*(data + AX) = *(BP + data) + opr;//ax = bp + ax
		} else if(i == VAL) {
			int opr = *(data + AX);
			*(data + AX) = *(data + opr);
		} else if(i == CALL) {
			int opr = *(emit + (*(IP + data))++);
			*(IP + data) = *(data + *(SP + data) - opr - 2);
		} else if(i == CAPI) {
			int opr = *(emit + (*(IP + data))++);
			api_call(*(data + *(SP + data) - opr - 1));
		} else if(i == EXIT) {
			break;
		}//if(ax)printf(" >>%d",*ax);
	}
	//printf("\n%d\n",*(data + AX));
}
