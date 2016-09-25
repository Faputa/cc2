//��������������
//�ܹ�����ָ�롢���顢����
//ǰ�÷��ŵݹ��ȡ�����÷���ѭ����ȡ

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static Type *ty, *tys;

void declare_init(void) {
	static int is_init = 0;
	if(!is_init) {
		tys = ty = (Type*)malloc(MAXSIZE * sizeof(Type));
		is_init = 1;
	}
}

Type* deriv_type(int base, Type *rely, int count) { //��������
	if(rely == NULL) {
		if(base == INT || base == CHAR || base == NUL) {
			for(Type *i = tys; i < ty; i++) {
				if(i -> base == base
				&& i -> rely == NULL) return i;
			}
			ty -> base = base;
			ty -> rely = NULL;
			return ty++;
		} else { printf("error7!\n"); exit(-1); }
	} else {
		if(base == PTR) {
			for(Type *i = tys; i < ty; i++) {
				if(i -> base == base
				&& i -> rely == rely) return i;
			}
			ty -> base = base;
			ty -> rely = rely;
			return ty++;
		} else if(base == ARR) {
			if(rely -> base == FUN) { printf("error8!\n"); exit(-1); }
			for(Type *i = tys; i < ty; i++) {
				if(i -> base == base
				&& i -> rely == rely
				&& i -> count == count) return i;
			}
			ty -> base = base;
			ty -> rely = rely;
			ty -> count = count;
			return ty++;
		} else if(base == FUN) {// || base == API) {
			if(rely -> base == FUN || rely -> base == ARR) { printf("error9!\n"); exit(-1); }
			Type **argtyls = getargtyls(count);
			for(Type *i = tys; i < ty; i++) {
				if(i -> base == base
				&& i -> rely == rely
				&& i -> count == count
				&& i -> argtyls == argtyls) return i;
			}
			ty -> base = base;
			ty -> rely = rely;
			ty -> count = count;
			ty -> argtyls = argtyls;
			return ty++;
		} else { printf("error10!\n"); exit(-1); }
	}
}

static void _print_type(Type *type) {
	if(type -> base == PTR) {
		printf("ָ��");
		_print_type(type -> rely);
		printf("��ָ��");
	} else if(type -> base == ARR) {
		printf("ӵ��%d������Ϊ", type -> count);
		_print_type(type -> rely);
		printf("��Ԫ�ص�����");
	} else if(type -> base == FUN) {
		//printf("��Ҫ%d�������ҷ���ֵΪ", type -> count);
		for(int i = 0; i < type -> count; i++) {
			printf("��%d������Ϊ", i + 1);
			_print_type(type -> argtyls[i]);
			printf("��");
		}
		printf("����ֵΪ");
		_print_type(type -> rely);
		printf("�ĺ���");
	} else if(type -> base == API) {
		printf("��Ҫ%d�������ҷ���ֵΪ", type -> count);
		_print_type(type -> rely);
		printf("��API");
	} else if(type -> base == INT) {
		printf("����");
	} else if(type -> base == CHAR) {
		printf("�ַ���");
	}
}

void print_type(Id *this_id) {
	printf("%sΪ", this_id -> name);
	_print_type(this_id -> type);
	//printf("\n");
}

static Type* specifier(void) {
	if(tki == Int) {
		next();
		return deriv_type(INT, NULL, 0);
	} else if(tki == Char) {
		next();
		return deriv_type(CHAR, NULL, 0);
	} else { printf("error11!\n"); exit(-1); }
}

static int lev(char *opr) {
	char *oprs[] = {
		")", "]",
		"", "*",
		"", "(", "["
	};
	int lev = 1;
	for(int i = 0; i < sizeof(oprs) / sizeof(*oprs); i++) {
		if(!strcmp(oprs[i], opr)) {
			return lev;
		} else if(!strcmp(oprs[i], "")) {
			lev++;
		}
	}
	return 0; //��������
}

static Id* declarator(Type *type, int env);
static int* complex(char *last_opr, int *cpx, int env) { //�������ͷ���
	//ǰ�÷���
	if(!strcmp(tks, "*")) { //ָ��
		next();
		cpx = complex("*", cpx, env);
		cpx++;
		*cpx++ = PTR;
	} else if(!strcmp(tks, "(")) { //����
		next();
		cpx = complex(")", cpx, env);
		if(strcmp(tks, ")")) { printf("error12!\n"); exit(-1); } //"("�޷�ƥ�䵽")"
		next();
	} else if(tki == ID) {
		if(env == GLO) gid -> name = tks;
		else lid -> name = tks;
		next();
	} else { printf("error13!\n"); exit(-1); }
	
	//next();
	//���÷���
	while(lev(tks) > lev(last_opr)) {
		if(!strcmp(tks, "[")) { //����
			next();
			int count = 0;
			if(strcmp(tks, "]")) {
				if(tki == INT) count = atoi(tks);
				else { printf("error14!\n"); exit(-1); }
				next();
			}
			*cpx++ = count;
			*cpx++ = ARR;
			if(strcmp(tks, "]")) { printf("error15!\n"); exit(-1); }
		} else if(!strcmp(tks, "(")) { //��������ָ��
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
					else { printf("error16!\n"); exit(-1); }
				}
			}
			*cpx++ = count;
			*cpx++ = FUN;
		} else { printf("error17!\n"); exit(-1); }
		next();
	}
	return cpx; //update cpx
}

static Id* declarator(Type *type, int env) {
	int cpxs[BUFSIZE]; //��������ջ
	int *cpx = cpxs; //��������ջջ��ָ��
	cpx = complex("", cpx, env);
	while(cpx > cpxs) {
		int base = *--cpx;
		int count = *--cpx;
		type = deriv_type(base, type, count);
	}
	return setid(type, env);
}

void declare(int env) {
	static int varc;
	if(env == GLO) {
		Type *type = specifier();
		Id *this_id = declarator(type, GLO);
		if(this_id -> type -> base == FUN) {
			if(!strcmp(tks, "{")) {
				infunc();
				varc = 0;
				this_id -> offset = e - emit;
				*e++ = PUSH; *e++ = BP;
				*e++ = MOV; *e++ = BP; *e++ = SP; //bp = sp
				*e++ = INC; *e++ = SP; int *_e = e++;
				next();
				while(strcmp(tks, "}")) {
					if(tki == Int || tki == Char) declare(LOC);
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
			} else { printf("error19!\n"); exit(-1); }
		} else {
			while(1) {
				if(!strcmp(tks, "=")) {
					next();
					if(this_id -> type -> base == INT) {
						*(data + this_id -> offset) = const_expr("");
					} else if(this_id -> type -> base == CHAR) {
						*(data + this_id -> offset) = const_expr("");
					} else if(this_id -> type -> base == PTR) {
						if(tki == STR) {
							if(this_id -> type -> rely -> base == CHAR) {
								*(data + this_id -> offset) = const_str();
							} else { printf("error20!\n"); exit(-1); }
						} else *(data + this_id -> offset) = const_null();
					} else if(this_id -> type -> base == ARR) {
						expr_arr(GLO, this_id -> type, this_id -> offset);
					} else { printf("error20!\n"); exit(-1); }
				} else {
					if(this_id -> type -> base == INT) *(data + this_id -> offset) = 0;
					else if(this_id -> type -> base == CHAR) *(data + this_id -> offset) = 0;
					else if(this_id -> type -> base == PTR) *(data + this_id -> offset) = 0;
					else if(this_id -> type -> base == ARR) memset(data + this_id -> offset, 0, this_id -> type -> count);
					else { printf("error21!\n"); exit(-1); }
				}
				if(!strcmp(tks, ";")) break;
				else if(!strcmp(tks, ",")) {
					next();
					this_id = declarator(type, GLO);
				} else { printf("error22!\n"); exit(-1); }
			}
		}
	} else if(env == LOC) {
		Type *type = specifier();
		while(1) {
			//varc++;
			Id *this_id = declarator(type, LOC);
			if(!strcmp(tks, "=")) {
				next();
				if(this_id -> type -> base == INT) {
					*e++ = AL; *e++ = this_id -> offset;
					*e++ = PUSH; *e++ = AX;
					if(this_id -> type != expr("").type) { printf("error23!\n"); exit(-1); }
					*e++ = ASS;
				} else if(this_id -> type -> base == CHAR) {
					*e++ = AL; *e++ = this_id -> offset;
					*e++ = PUSH; *e++ = AX;
					if(this_id -> type != expr("").type) { printf("error24!\n"); exit(-1); }
					*e++ = ASS;
				} else if(this_id -> type -> base == PTR) {
					*e++ = AL; *e++ = this_id -> offset;
					*e++ = PUSH; *e++ = AX;
					if(this_id -> type != expr("").type) { printf("error24!\n"); exit(-1); }
					*e++ = ASS;
				} else if(this_id -> type -> base == ARR) {
					expr_arr(LOC, this_id -> type, this_id -> offset);
				}
			}
			varc += typesize(this_id -> type);
			if(!strcmp(tks, ";")) break;
			else if(!strcmp(tks, ",")) next();
			else { printf("error25!\n"); exit(-1); }
		}
	}
}