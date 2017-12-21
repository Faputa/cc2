//��������������
//�ܹ�����ָ�롢���顢����
//ǰ�÷��ŵݹ��ȡ�����÷���ѭ����ȡ

#include "cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int varc, *cpx;

void declare_init(void) {
	cpx = (int*)malloc(MAXSIZE * sizeof(int));
}

static Type* specifier(void) {
	Type *t;
	if(tki == Int) t = typeint;
	else if(tki == Char) t = typechar;
	else if(tki == Void) t = type_derive(VOID, NULL, 0);
	else error("line %d: error!\n", line);
	next();
	return t;
}

static int lev(char *opr) {
	char *oprs[] = {
		//")", "]",
		"", "*",
		"", "(", "["
	};
	int lev = 0;
	for(int i = 0; i < sizeof(oprs) / sizeof(*oprs); i++) {
		if(!strcmp(oprs[i], opr)) return lev;
		else if(!strcmp(oprs[i], "")) lev++;
	}
	return 0; //��������
}

static Id* decl_expr(Type *type, int scope);
static void complex(char *last_opr, Id *id) { //�������ͷ���
	//ǰ�÷���
	if(!strcmp(tks, "*")) { //ָ��
		next();
		complex("*", id);
		cpx++;
		*cpx++ = PTR;
	} else if(!strcmp(tks, "(")) { //����
		next();
		complex("", id);
		if(strcmp(tks, ")")) error("line %d: error!\n", line);
		next();
	} else if(tki == ID) {
		id->name = tks;
		next();
	} else error("line %d: error!\n", line);
	
	//���÷���
	while(lev(tks) > lev(last_opr)) {
		if(!strcmp(tks, "[")) { //����
			next();
			int count = 0;
			if(strcmp(tks, "]")) {
				count = expr_const("");
			}
			*cpx++ = count;
			*cpx++ = ARR;
			if(strcmp(tks, "]")) error("line %d: error!\n", line);
		} else if(!strcmp(tks, "(")) { //��������ָ��
			int count = 0;
			infunc();
			next();
			if(strcmp(tks, ")")) {
				while(1) {
					count++;
					Type *type = specifier();
					decl_expr(type, ARG);
					if(!strcmp(tks, ")")) break;
					else if(!strcmp(tks, ",")) next();
					else error("line %d: error!\n", line);
				}
			}
			*cpx++ = count;
			*cpx++ = FUN;
		} else error("line %d: error!\n", line);
		next();
	}
}

static Id* decl_expr(Type *type, int scope) {
	Id *id = (scope == GLO)? gid: lid;
	id->idkind = scope;
	int *_cpx = cpx;
	complex("", id); //���ɸ�������ջ
	while(cpx > _cpx) {
		int tykind = *--cpx;
		int count = *--cpx;
		type = type_derive(tykind, type, count);
	}
	if(type->tykind == FUN && scope == ARG) {
		type = type_derive(PTR, type, 0);
	} else if(type->tykind == ARR && scope == ARG) {
		type = type_derive(PTR, type->base, 0);
	}
	setid(type, id);
	return id;
}

void declare_loc(void) {
	Type *type = specifier();
	while(1) {
		//varc++;
		Id *id = decl_expr(type, LOC);
		if(!strcmp(tks, "=")) {
			next();
			if(id->type->tykind == INT || id->type->tykind == CHAR || id->type->tykind == PTR) {
				*e++ = AL; *e++ = id->offset;
				*e++ = PUSH; *e++ = AX;
				type_check(id->type, expr("").type, "=");
				*e++ = ASS;
			} else if(id->type->tykind == ARR) {
				arr_init_loc(id->type, id->offset);
			}
		}
		varc += type_size(id->type);
		if(!strcmp(tks, ";")) break;
		else if(!strcmp(tks, ",")) next();
		else error("line %d: error!\n", line);
	}
}

void declare_glo(void) {
	Type *type = specifier();
	Id *id = decl_expr(type, GLO);
	if(id->type->tykind == FUN) {
		if(!strcmp(tks, "{")) {
			varc = 0;
			id->offset = e - emit;
			*e++ = PUSH; *e++ = BP;
			*e++ = MOV; *e++ = BP; *e++ = SP; //bp = sp
			*e++ = INC; *e++ = SP; int *_e = e++;
			next();
			while(strcmp(tks, "}")) {
				if(tki == Int || tki == Char || tki == Void) declare_loc();
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
		} else error("line %d: error!\n", line);
	} else {
		while(1) {
			if(!strcmp(tks, "=")) {
				next();
				if(id->type->tykind == INT) data[id->offset] = expr_const("");
				else if(id->type->tykind == CHAR) data[id->offset] = expr_const("");
				else if(id->type->tykind == PTR) data[id->offset] = ptr_const(id->type);
				else if(id->type->tykind == ARR) arr_init_glo(id->type, id->offset);
				else error("line %d: error!\n", line);
			} else {
				if(id->type->tykind == INT) data[id->offset] = 0;
				else if(id->type->tykind == CHAR) data[id->offset] = 0;
				else if(id->type->tykind == PTR) data[id->offset] = 0;
				else if(id->type->tykind == ARR) memset(data + id->offset, 0, id->type->count);
				else error("line %d: error!\n", line);
			}
			if(!strcmp(tks, ";")) break;
			else if(!strcmp(tks, ",")) {
				next();
				id = decl_expr(type, GLO);
			} else error("line %d: error!\n", line);
		}
	}
}
