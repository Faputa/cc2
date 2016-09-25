//词法分析

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *p, *tks;
int tki;

static char *tksls;
static int ti;

static char *keyword[] = {
	"int", "char", "void", "if", "else", "while", "do", "for", "return", "NULL"
};
static char *point[] = {
	"==", ">=", "<=", "&&", "||", "+=", "-=", "*=", "/=",
	"+", "-", "*", "/", "%", "=", ">", "<", "!", "^", "&", "|", "(", ")", "{", "}", "[", "]", "?", ":", ",", ";"
};
static char *trans[] = {
	"\\n", "\n",
	"\\\\", "\\",
	"\\t", "\t",
	"\\\"", "\"",
	"\\0", "\0"
};

void token_init(void) {
	static int is_init = 0;
	if(!is_init) {
		p = (char*)malloc(MAXSIZE * sizeof(char));
		tksls = (char*)malloc(MAXSIZE * sizeof(char));
		ti = 0;
		is_init = 1;
	}
}

static char* tksalloc(int size) {
	int _ti = ti;
	ti += size;
	return tksls + _ti;
}

static char* tkstidy(char *tks) {
	for(int i = 0; tksls + i < tks; i += strlen(tksls + i) + 1) {
		if(!strcmp(tks, tksls + i)) {
			ti -= strlen(tks) + 1;
			return tksls + i;
		}
	}
	return tks;
}

static char* tksbychar(char c) {
	int i = 0;
	while(i < ti) {
		if(tksls[i] == c) return tksls + i;
		i++;
	}
	tksls[ti++] = c;
	tksls[ti++] = '\0';
	return tksls + i;
}

void next(void) {
	tks = ""; tki = -1;
	while(*p) {
		if(!strncmp(p, "//", 2) || *p == '#') { while(*p != '\n') p++; p++; }
		else if(!strncmp(p, "/*", 2)) { while(strncmp(p, "*/", 2)) p++; p+=2; }
		else if((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_') {
			int len = 0; char *_p = p;
			while((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_') {
				len++; p++;
			}
			for(int i = 0; i < sizeof(keyword) / sizeof(*keyword); i++) {
				if(strlen(keyword[i]) == len && !strncmp(keyword[i], _p, len)) {
					tki = i;
					return;
				}
			}
			tki = ID;
			tks = tksalloc(len + 1);
			strncpy(tks, _p, len);
			tks[len] = '\0';
			tks = tkstidy(tks);
			return;
		} else if(*p >= '0' && *p <= '9') {
			int len = 0; char *_p = p;
			while(*p >= '0' && *p <= '9') {
				len++; p++;
			}
			tki = INT;
			tks = tksalloc(len + 1);
			strncpy(tks, _p, len);
			tks[len] = '\0';
			tks = tkstidy(tks);
			return;
		} else if(*p == '"') {
			tki = STR;
			int len = 0; char *_p = ++p;
			while(*p != '"' && *p) {
				if(*p++ =='\\') p++;
				len++;
			}
			if(*p) p++;
			tks = tksalloc(len + 1);
			int i = 0;
			while(*_p != '"') {
				if(*_p == '\\') {
					for(int j = 0; j < sizeof(trans) / sizeof(*trans); j += 2) {
						if(!strncmp(trans[j], _p, strlen(trans[j]))) {
							tks[i] = *trans[j+1];
							_p += strlen(trans[j]);
							break;
						}
					}
				} else {
					tks[i] = *_p++;
				}
				i++;
			}
			tks[i] = '\0';
			tks = tkstidy(tks);
			return;
		} else if(*p == '\'') {
			tki = CHAR;
			if(*++p == '\\') {
				for(int i = 0; i < sizeof(trans) / sizeof(*trans); i += 2) {
					if(!strncmp(trans[i], p, strlen(trans[i]))) {
						tks = trans[i + 1];
						break;
					}
				}
			} else {
				/*tks = tksalloc(2);
				tks[0] = *p; tks[1] = '\0';
				tks = tkstidy(tks);*/
				tks = tksbychar(*p);
			}
			while(*p != '\'' && *p) p++;
			if(*p) p++;
			return;
		} else {
			for(int i = 0; i < sizeof(point) / sizeof(*point); i++) {
				if(!strncmp(point[i], p, strlen(point[i]))) {
					tks = point[i];
					p += strlen(point[i]);
					return;
				}
			}
			p++;
		}
	}
}
