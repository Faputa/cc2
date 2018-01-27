//类型表、参数类型表

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Type *typeint, *typechar, *typenull;

static Type *ty, *tys;
static Type **argtyls, **argtylss;

void type_init(void) {
	tys = ty = (Type*)malloc(MAXSIZE * sizeof(Type));
	argtylss = argtyls = (Type**)malloc(MAXSIZE * sizeof(Type*));
	typeint = type_derive(INT, NULL, 0);
	typechar = type_derive(CHAR, NULL, 0);
	typenull = type_derive(NUL, NULL, 0);
}

static Type** getargtyls(int count) {
	if(count == 0) return NULL;
	Id *id = lid - count;
	Type **_argtyls = argtylss;
	int i = 0;
	while(_argtyls < argtyls) {
		i = 0;
		while(i < count && i < argtyls - _argtyls) {
			if(id[i].type != _argtyls[i]) break;
			i++;
		}
		if(i == count) return _argtyls;
		else if(i == argtyls - _argtyls) break;
		else _argtyls++;
	}
	while(i < count) {
		*argtyls++ = id[i++].type;
	}
	return _argtyls;
}

Type* type_derive(int tykind, Type *base, int count) { //类型生成
	if(base == NULL) {
		if(tykind == INT || tykind == CHAR || tykind == VOID || tykind == NUL) {
			for(Type *i = tys; i < ty; i++) {
				if(i->tykind == tykind
				&& i->base == NULL) return i;
			}
			ty->tykind = tykind;
			ty->base = NULL;
			return ty++;
		} else error("line %d: error!\n", line);
	} else {
		if(tykind == PTR) {
			for(Type *i = tys; i < ty; i++) {
				if(i->tykind == tykind
				&& i->base == base) return i;
			}
			ty->tykind = tykind;
			ty->base = base;
			return ty++;
		} else if(tykind == ARR) {
			if(base->tykind == FUN || base->tykind == VOID) error("line %d: error!\n", line);
			for(Type *i = tys; i < ty; i++) {
				if(i->tykind == tykind
				&& i->base == base
				&& i->count == count) return i;
			}
			ty->tykind = tykind;
			ty->base = base;
			ty->count = count;
			return ty++;
		} else if(tykind == FUN) {// || tykind == API) {
			if(base->tykind == FUN || base->tykind == ARR) error("line %d: error!\n", line);
			Type **argtyls = getargtyls(count);
			for(Type *i = tys; i < ty; i++) {
				if(i->tykind == tykind
				&& i->base == base
				&& i->count == count
				&& i->argtyls == argtyls) return i;
			}
			ty->tykind = tykind;
			ty->base = base;
			ty->count = count;
			ty->argtyls = argtyls;
			return ty++;
		} else error("line %d: error!\n", line);
	}
	return NULL; //to avoid warnings
}

static void _print_type(Type *type) {
	if(type->tykind == PTR) {
		printf("指向");
		_print_type(type->base);
		printf("的指针");
	} else if(type->tykind == ARR) {
		printf("拥有%d个类型为", type->count);
		_print_type(type->base);
		printf("的元素的数组");
	} else if(type->tykind == FUN) {
		for(int i = 0; i < type->count; i++) {
			printf("第%d个参数为", i + 1);
			_print_type(type->argtyls[i]);
			printf("、");
		}
		printf("返回值为");
		_print_type(type->base);
		printf("的函数");
	} else if(type->tykind == API) {
		for(int i = 0; i < type->count; i++) {
			printf("第%d个参数为", i + 1);
			_print_type(type->argtyls[i]);
			printf("、");
		}
		printf("返回值为");
		_print_type(type->base);
		printf("的API");
	} else if(type->tykind == INT) {
		printf("整型");
	} else if(type->tykind == CHAR) {
		printf("字符型");
	} else if(type->tykind == VOID) {
		printf("空");
	}
}

void print_type(Id *id) {
	printf("%s为", id->name);
	_print_type(id->type);
	//printf("\n");
}

int type_size(Type *type) {
	if(type->tykind == INT) return 1;
	else if(type->tykind == CHAR) return 1;
	else if(type->tykind == PTR) return 1;
	else if(type->tykind == ARR) return type_size(type->base) * type->count;
	return 0;
}

void type_check(Type *type1, Type *type2, char *opr) {
	if(!strcmp(opr, "=")) {
		if(type1->tykind == INT || type1->tykind == CHAR) {
			if(type2->tykind == INT || type2->tykind == CHAR) {}
			else error("line %d: error!\n", line);
		} else if(type1->tykind == PTR) {
			if(type1 == type2
			|| type2->tykind == NUL
			|| (type2->tykind == FUN && type2 == type1->base)
			|| (type2->tykind == ARR && type2->base == type1->base)
			|| (type2->tykind == PTR && type2->base->tykind == VOID)) {}
			else error("line %d: error!\n", line);
		}
	} else if(!strcmp(opr, "+") || !strcmp(opr, "-") || !strcmp(opr, "[")) {
		if(type1->tykind == INT || type1->tykind == CHAR) {
			if(type2->tykind == INT || type2->tykind == CHAR) {
				if(!strcmp(opr, "[")) error("line %d: error!\n", line);
			} else if(type2->tykind == PTR || type2->tykind == ARR) {
				if(!strcmp(opr, "-")) error("line %d: error!\n", line);
			} else error("line %d: error!\n", line);
		} else if(type1->tykind == PTR || type1->tykind == ARR) {
			if(type2->tykind == PTR || type2->tykind == ARR) {
				if(type1->base != type2->base || strcmp(opr, "-")) error("line %d: error!\n", line);
			}
			else if(type2->tykind == INT || type2->tykind == CHAR) {}
			else error("line %d: error!\n", line);
		} else error("line %d: error!\n", line);
	} else if(!strcmp(opr, "*") || !strcmp(opr, "/") || !strcmp(opr, "%")) {
		if(type1->tykind == INT || type1->tykind == CHAR) {
			if(type2->tykind == INT || type2->tykind == CHAR) {}
			else error("line %d: error!\n", line);
		} else error("line %d: error!\n", line);
	} else if(!strcmp(opr, "==") || !strcmp(opr, ">") || !strcmp(opr, "<") || !strcmp(opr, "!=") || !strcmp(opr, ">=") || !strcmp(opr, "<=")) {
		if(type1->tykind == INT || type1->tykind == CHAR) {
			if(type2->tykind == INT || type2->tykind == CHAR) {}
			else error("line %d: error!\n", line);
		} else if(type1->tykind == PTR) {
			if(type1 == type2 || type2->tykind == NUL) {}
			else error("line %d: error!\n", line);
		} else if(type1->tykind == NUL) {
			if(type2->tykind == NUL || type2->tykind == PTR) {}
			else error("line %d: error!\n", line);
		} else error("line %d: error!\n", line);
	} else if(!strcmp(opr, "&&") || !strcmp(opr, "||")) {
		if(type1->tykind == INT || type1->tykind == CHAR) {
			if(type2->tykind == INT || type2->tykind == CHAR) {}
			else error("line %d: error!\n", line);
		} else error("line %d: error!\n", line);
	} else error("line %d: error!\n", line);
}
