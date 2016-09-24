//词法分析

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *p, *tks;
int tki;

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
		is_init = 1;
	}
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
			tks = (char*)malloc(sizeof(char) * (len+1));
			strncpy(tks, _p, len);
			tks[len] = '\0';
			return;
		} else if(*p >= '0' && *p <= '9') {
			int len = 0; char *_p = p;
			while(*p >= '0' && *p <= '9') {
				len++; p++;
			}
			tki = INT;
			tks = (char*)malloc(sizeof(char) * (len+1));
			strncpy(tks, _p, len);
			tks[len] = '\0';
			return;
		} else if(*p == '"') {
			tki = STR;
			int len = 0; char *_p = ++p;
			while(*p != '"' && *p) {
				if(*p++ =='\\') p++;
				len++;
			}
			if(*p) p++;//printf("%d\n",len);
			tks = (char*)malloc(sizeof(char) * (len+1));
			int i = 0; while(*_p != '"') {
				if(*_p == '\\') {
					for(int j = 0; j < sizeof(trans) / sizeof(*trans); j+=2) {
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
			return;
		} else if(*p == '\'') {
			tki = CHAR;
			if(*++p != '\\') {
				tks = (char*)malloc(sizeof(char) * 2);
				tks[0] = *p; tks[1] = '\0';
			} else {
				for(int i = 0; i < sizeof(trans) / sizeof(*trans); i+=2) {
					if(!strncmp(trans[i], p, strlen(trans[i]))) {
						tks = trans[i+1];
						break;
					}
				}
			}
			while(*p != '\'' && *p) p++;
			if(*p) p++;
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
