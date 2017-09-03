//表达式分析

#include "cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ptr_const(Type *type) {
	int i;
	if(tki == Null) i = 0;
	else if(tki == STR) {
		if(type->base->tykind == CHAR) i = sgetstr(tks)->offset;
		else error("line %d: error!\n", line);
	}
	else error("line %d: error!\n", line);
	next();
	return i;
}

void arr_init_glo(Type *type, int offset) {
	memset(data + offset, 0, type->count);
	if(!strcmp(tks, "{")) {
		next();
		int count = 0;
		if(strcmp(tks, "}")) {
			while(1) {
				count++;
				if(type->base->tykind == INT) data[offset] = expr_const("");
				else if(type->base->tykind == CHAR) data[offset] = expr_const("");
				else if(type->base->tykind == PTR) data[offset] = ptr_const(type->base);
				else if(type->base->tykind == ARR) arr_init_glo(type->base, offset);
				else error("line %d: error!\n", line);
				
				if(!strcmp(tks, "}")) {
					break;
				} else if(!strcmp(tks, ",")) {
					offset += type_size(type->base);
					next();
				} else error("line %d: error!\n", line);
			}
		}
		if(type->count < count) error("line %d: error!\n", line);
	} else if(tki == STR) {
		Sym *strid = sgetstr(tks);
		if(type->base->tykind != CHAR) error("line %d: error!\n", line);
		if(type->count < strid->type->count) error("line %d: error!\n", line);
		for(int i = 0; i < strid->type->count; i++) {
			data[offset + i] = data[strid->offset + i];
		}
	} else error("line %d: error!\n", line);
	next();
}

void arr_init_loc(Type *type, int offset) {
	if(!strcmp(tks, "{")) {
		next();
		int count = 0;
		if(strcmp(tks, "}")) {
			while(1) {
				count++;
				if(type->base->tykind == INT || type->base->tykind == CHAR || type->base->tykind == PTR) {
					*e++ = AL; *e++ = offset;
					*e++ = PUSH; *e++ = AX;
					type_check(type->base, expr("").type, "=");
					*e++ = ASS;
				}
				else if(type->base->tykind == ARR) arr_init_loc(type->base, offset);
				else error("line %d: error!\n", line);
				
				if(!strcmp(tks, "}")) {
					break;
				} else if(!strcmp(tks, ",")) {
					offset += type_size(type->base);
					next();
				} else error("line %d: error!\n", line);
			}
		}
		if(type->count < count) error("line %d: error!\n", line);
	} else if(tki == STR) {
		Sym *strid = sgetstr(tks);
		if(type->base->tykind != CHAR) error("line %d: error!\n", line);
		if(type->count < strid->type->count) error("line %d: error!\n", line);
		for(int i = 0; i < strid->type->count; i++) {
			*e++ = AL; *e++ = offset + i;
			*e++ = PUSH; *e++ = AX;
			*e++ = AG; *e++ = strid->offset + i;
			*e++ = VAL;
			*e++ = ASS;
		}
	} else error("line %d: error!\n", line);
	next();
}

static int lev(char *opr) { //优先级越高lev越大，其他符号lev为0
	char *oprs[] = {
		//")", "]",
		"", "&&", "||", "!",
		"", "==", "!=",
		"", ">", "<", ">=", "<=",
		"", "+", "-",
		"", "*", "/", "%",
		"", "=",
		"", "*_", "&_",
		"", "(", "["
	};
	int lev = 0;
	for(int i = 0; i < sizeof(oprs) / sizeof(*oprs); i++) {
		if(!strcmp(oprs[i], opr)) return lev;
		else if(!strcmp(oprs[i], "")) lev++;
	}
	return 0; //其他符号
}

int expr_const(char *last_opr) {
	int a;
	if(tki == INT) {
		a = atoi(tks);
		next();
	} else if(tki == CHAR) {
		a = tks[0];
	} else if(!strcmp(tks, "(")) {
		next();
		a = expr_const("");
		if(!strcmp(tks, ")")) next(); else error("line %d: error!\n", line);
	} else if(!strcmp(tks, "!")) {
		next();
		a = !expr_const("!");
	} else if(!strcmp(tks, "-")) {
		next();
		a = -expr_const("-");
	} else error("line %d: error!\n", line);
	
	while(lev(tks) > lev(last_opr)) {
		char *opr = tks;
		next();
		int b = expr_const(opr);
		if (!strcmp(opr, "+")) a += b;
		else if(!strcmp(opr, "-")) a -= b;
		else if(!strcmp(opr, "*")) a *= b;
		else if(!strcmp(opr, "/")) a /= b;
		else if(!strcmp(opr, "%")) a %= b;
		else if(!strcmp(opr, ">")) a = a > b;
		else if(!strcmp(opr, "<")) a = a < b;
		else if(!strcmp(opr, "==")) a = a == b;
		else if(!strcmp(opr, ">=")) a = a >= b;
		else if(!strcmp(opr, "<=")) a = a <= b;
		else if(!strcmp(opr, "!=")) a = a != b;
		else if(!strcmp(opr, "&&")) a = a && b;
		else if(!strcmp(opr, "||")) a = a || b;
		else error("line %d: error!\n", line);
	}
	return a;
}

Er expr(char *last_opr) { //1 + 2 ^ 3 * 4 == (1 + (2 ^ (3) * (4)))
	Er er = {NULL, 0, 1};
	if(tki == INT) {
		er.type = typeint;
		*e++ = SET; *e++ = AX; *e++ = atoi(tks);
		next();
	} else if(tki == CHAR) {
		er.type = typechar;
		*e++ = SET; *e++ = AX; *e++ = tks[0];
		next();
	} else if(tki == NUL) {
		er.type = typenull;
		*e++ = SET; *e++ = AX; *e++ = 0;
		next();
	} else if(tki == STR) {
		Sym *this_id = sgetstr(tks);
		*e++ = AG; *e++ = this_id->offset;
		er.type = this_id->type;
		er.is_lvalue = 1;
		next();
	} else if(tki == ID) {
		Sym *this_id = getid(tks);
		er.type = this_id->type;
		*e++ = this_id->idkind == GLO? AG: AL; *e++ = this_id->offset;
		if(er.type->tykind == INT || er.type->tykind == CHAR || er.type->tykind == PTR) {
			*e++ = VAL;
			er.is_const = 0;
		}
		er.is_lvalue = 1;
		next();
	} else if(!strcmp(tks, "(")) {
		next();
		er.type = expr("").type;
		if(!strcmp(tks, ")")) next(); else error("line %d: error!\n", line);
	} else if(!strcmp(tks, "*")) {
		next();
		er.type = expr("*_").type;
		if(er.type->tykind != PTR) error("line %d: error!\n", line);
		er.type = er.type->base;
		if(er.type->tykind == INT || er.type->tykind == CHAR || er.type->tykind == PTR) {
			*e++ = VAL;
			er.is_const = 0;
		}
		er.is_lvalue = 1;
	} else if(!strcmp(tks, "&")) {
		next();
		Er _er = expr("&_");
		if(!_er.is_lvalue) error("line %d: error!\n", line);
		if(_er.type->tykind == INT || _er.type->tykind == PTR) e--;
		er.type = type_derive(PTR, _er.type, 0);
	} else if(!strcmp(tks, "!")) {
		next();
		er.type = expr("!").type;
		if(er.type->tykind != INT) er.type = typeint;
		*e++ = NOT;
	} else if(!strcmp(tks, "-")) {
		next();
		*e++ = SET; *e++ = AX; *e++ = 0;
		*e++ = PUSH; *e++ = AX;
		er.type = expr("-").type;
		if(er.type->tykind == CHAR) er.type = typeint;
		else if(er.type->tykind != INT) error("line %d: error!\n", line);
		*e++ = SUB;
	} else error("line %d: error!\n", line);
	
	while(lev(tks) > lev(last_opr)) {
		int *_e1 = e;
		*e++ = PUSH; *e++ = AX;
		if(!strcmp(tks, "=")) {
			next();
			e -= 3; *e++ = PUSH; *e++ = AX;
			type_check(er.type, expr("").type, "=");
			if(!er.is_const) *e++ = ASS; else error("line %d: error!\n", line);
		} else if(!strcmp(tks, "(")) {
			next();
			if(er.type->tykind == PTR) er.type = er.type->base; //将函数指针转化为函数
			if(er.type->tykind != FUN && er.type->tykind != API) error("line %d: error!\n", line);
			int argc = 0;
			if(strcmp(tks, ")")) {
				int *_e1 = e;
				while(1) {
					int *_e2 = e;
					Type *type = expr("").type;
					if(argc < er.type->count) type_check((er.type->argtyls)[argc], type, "="); //参数类型检查
					*e++ = PUSH; *e++ = AX;
					int *_e3 = e;
					if(_e2 > _e1) {
						memcpy(_e3, _e1, (_e2 - _e1) * sizeof(int));
						memmove(_e1, _e2, (_e3 - _e1) * sizeof(int));
					}
					argc++;
					if(!strcmp(tks, ")")) break;
					else if(!strcmp(tks, ",")) next();
					else error("line %d: error!\n", line);
				}
			}
			next();
			*e++ = (er.type->tykind == FUN)? CALL: CAPI; *e++ = argc;
			*e++ = DEC; *e++ = SP; *e++ = argc + 1;
			er.type = er.type->base;
		} else if(!strcmp(tks, "+") || !strcmp(tks, "-") || !strcmp(tks, "[")) {
			char *opr = tks;
			next();
			Type *type;
			if(!strcmp(opr, "[")) {
				type = expr("").type;
				if(!strcmp(tks, "]")) next(); else error("line %d: error!\n", line);
			} else {
				type = expr(opr).type;
			}
			type_check(er.type, type, opr);
			int f1 = er.type->tykind == INT || er.type->tykind == CHAR;
			int f2 = type->tykind == INT || type->tykind == CHAR;
			if(f1 && f2) {
				*e++ = !strcmp(opr, "-")? SUB: ADD;
			} else if(!f1 && !f2) {
				*e++ = SUB;
				*e++ = PUSH; *e++ = AX;
				*e++ = SET; *e++ = AX; *e++ = type_size(er.type->base);
				*e++ = DIV;
				er.type = typeint;
			} else {
				if(f1) {
					er.type = type;
				}
				int *_e2 = e;
				*e++ = PUSH; *e++ = AX;
				*e++ = SET; *e++ = AX; *e++ = type_size(er.type->base);
				*e++ = MUL;
				int *_e3 = e;
				if(f1) {
					memcpy(_e3, _e1, (_e2 - _e1) * sizeof(int));
					memmove(_e1, _e2, (_e3 - _e1) * sizeof(int));
				}
				*e++ = !strcmp(opr, "-")? SUB: ADD;
				if(!strcmp(opr, "[")) {
					er.type = er.type->base;
					if(er.type->tykind == INT || er.type->tykind == CHAR || er.type->tykind == PTR) {
						*e++ = VAL;
						er.is_const = 0;
					}
					er.is_lvalue = 1;
				} else if(er.type->tykind == ARR) er.type = type_derive(PTR, er.type->base, 0);
			}
		} else {
			char *opr = tks;
			next();
			type_check(er.type, expr(opr).type, opr);
			er.type = typeint;
			if(!strcmp(opr, "*")) *e++ = MUL;
			else if(!strcmp(opr, "/")) *e++ = DIV;
			else if(!strcmp(opr, "%")) *e++ = MOD;
			else if(!strcmp(opr, "==")) *e++ = EQ;
			else if(!strcmp(opr, ">")) *e++ = GT;
			else if(!strcmp(opr, "<")) *e++ = LT;
			else if(!strcmp(opr, "!=")) { *e++ = EQ; *e++ = NOT; }
			else if(!strcmp(opr, ">=")) { *e++ = LT; *e++ = NOT; }
			else if(!strcmp(opr, "<=")) { *e++ = GT; *e++ = NOT; }
			else if(!strcmp(opr, "&&")) *e++ = AND;
			else if(!strcmp(opr, "||")) *e++ = OR;
			else error("line %d: error!\n", line);
		}
	}
	return er;
}
