//复杂声明解析器
//能够解析指针、数组、函数
//前置符号递归读取，后置符号循环读取

#include "cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Type* specifier(void) {
	if(tki == Int) {
		next();
		return type_derive(INT, NULL, 0);
	} else if(tki == Char) {
		next();
		return type_derive(CHAR, NULL, 0);
	} else if(tki == Void) {
		next();
		return type_derive(VOID, NULL, 0);
	} else { printf("line %d: error11!\n", line); exit(-1); }
}

static int lev(char *opr) {
	char *oprs[] = {
		")", "]",
		"", "*",
		"", "(", "["
	};
	int lev = 1;
	for(int i = 0; i < sizeof(oprs) / sizeof(*oprs); i++) {
		if(!strcmp(oprs[i], opr)) return lev;
		else if(!strcmp(oprs[i], "")) lev++;
	}
	return 0; //其他符号
}

static Id* declarator(Type *type, int scope);
static int* complex(char *last_opr, int *cpx, int scope) { //复杂类型分析
	//前置符号
	if(!strcmp(tks, "*")) { //指针
		next();
		cpx = complex("*", cpx, scope);
		cpx++;
		*cpx++ = PTR;
	} else if(!strcmp(tks, "(")) { //括号
		next();
		cpx = complex(")", cpx, scope);
		if(strcmp(tks, ")")) { printf("line %d: error12!\n", line); exit(-1); } //"("无法匹配到")"
		next();
	} else if(tki == ID) {
		setid1(tks, scope);
		next();
	} else { printf("line %d: error13!\n", line); exit(-1); }
	
	//next();
	//后置符号
	while(lev(tks) > lev(last_opr)) {
		if(!strcmp(tks, "[")) { //数组
			next();
			int count = 0;
			if(strcmp(tks, "]")) {
				count = const_expr("");
			}
			*cpx++ = count;
			*cpx++ = ARR;
			if(strcmp(tks, "]")) { printf("line %d: error15!\n", line); exit(-1); }
		} else if(!strcmp(tks, "(")) { //函数或函数指针
			int count = 0;
			inparam();
			next();
			if(strcmp(tks, ")")) {
				while(1) {
					count++;
					Type *type = specifier();
					declarator(type, ARG);
					if(!strcmp(tks, ")")) break;
					else if(!strcmp(tks, ",")) next();
					else { printf("line %d: error16!\n", line); exit(-1); }
				}
			}
			*cpx++ = count;
			*cpx++ = FUN;
		} else { printf("line %d: error17!\n", line); exit(-1); }
		next();
	}
	return cpx; //update cpx
}

static Id* declarator(Type *type, int scope) {
	int cpxs[BUFSIZE]; //复杂类型栈
	int *cpx = cpxs; //复杂类型栈栈顶指针
	cpx = complex("", cpx, scope);
	while(cpx > cpxs) {
		int base = *--cpx;
		int count = *--cpx;
		type = type_derive(base, type, count);
	}
	return setid2(type, scope);
}

void declare(int scope) {
	static int varc;
	if(scope == GLO) {
		Type *type = specifier();
		Id *id = declarator(type, GLO);
		if(id -> type -> base == FUN) {
			if(!strcmp(tks, "{")) {
				infunc();
				varc = 0;
				id -> offset = e - emit;
				*e++ = PUSH; *e++ = BP;
				*e++ = MOV; *e++ = BP; *e++ = SP; //bp = sp
				*e++ = INC; *e++ = SP; int *_e = e++;
				next();
				while(strcmp(tks, "}")) {
					if(tki == Int || tki == Char || tki == Void) declare(LOC);
					else stmt();
					next();
				}
				*_e = varc;
				*e++ = MOV; *e++ = SP; *e++ = BP; //sp = bp
				*e++ = POP; *e++ = BP;
				*e++ = POP; *e++ = IP;
				outfunc();
			} else if(!strcmp(tks, ";")) {
				outfunc();
			} else { printf("line %d: error18!\n", line); exit(-1); }
		} else {
			while(1) {
				if(!strcmp(tks, "=")) {
					next();
					if(id -> type -> base == INT) data[id -> offset] = const_expr("");
					else if(id -> type -> base == CHAR) data[id -> offset] = const_expr("");
					else if(id -> type -> base == PTR) data[id -> offset] = const_ptr(id -> type);
					else if(id -> type -> base == ARR) expr_arr(GLO, id -> type, id -> offset);
					else { printf("line %d: error19!\n", line); exit(-1); }
				} else {
					if(id -> type -> base == INT) data[id -> offset] = 0;
					else if(id -> type -> base == CHAR) data[id -> offset] = 0;
					else if(id -> type -> base == PTR) data[id -> offset] = 0;
					else if(id -> type -> base == ARR) memset(data + id -> offset, 0, id -> type -> count);
					else { printf("line %d: error20!\n", line); exit(-1); }
				}
				if(!strcmp(tks, ";")) break;
				else if(!strcmp(tks, ",")) {
					next();
					id = declarator(type, GLO);
				} else { printf("line %d: error21!\n", line); exit(-1); }
			}
		}
	} else if(scope == LOC) {
		Type *type = specifier();
		while(1) {
			//varc++;
			Id *id = declarator(type, LOC);
			if(!strcmp(tks, "=")) {
				next();
				if(id -> type -> base == INT || id -> type -> base == CHAR || id -> type -> base == PTR) {
					*e++ = AL; *e++ = id -> offset;
					*e++ = PUSH; *e++ = AX;
					type_check(id -> type, expr("").type, "=");
					*e++ = ASS;
				} else if(id -> type -> base == ARR) {
					expr_arr(LOC, id -> type, id -> offset);
				}
			}
			varc += type_size(id -> type);
			if(!strcmp(tks, ";")) break;
			else if(!strcmp(tks, ",")) next();
			else { printf("line %d: error22!\n", line); exit(-1); }
		}
	}
}
