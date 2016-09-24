//语句分析

#include "cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stmt(void) {
	if(!strcmp(tks, "{")) {
		inblock();
		next();
		while(strcmp(tks, "}")) {
			if(tki == Int || tki == Char) declare(LOC);
			else stmt();
			next();
		}
		outblock();
	} else if(tki == If) {
		next(); if(strcmp(tks, "(")) { printf("error70!\n"); exit(-1); }
		next(); expr(")");
		if(strcmp(tks, ")")) { printf("error71!\n"); exit(-1); }
		next();
		*e++ = JZ; int *_e1 = e++;
		stmt();
		char *_p = p;
		next();
		if(tki == Else) {
			*e++ = JMP; int *_e2 = e++;
			*_e1 = e - emit;
			next();
			stmt();
			*_e2 = e - emit;
		} else {
			*_e1 = e - emit;
			p = _p;
		}
	} else if(tki == Do) {
		int *_e1 = e;
		next();
		stmt();
		next();
		if(tki == While) next(); else { printf("error72!\n"); exit(-1); }
		if(strcmp(tks, "(")) { printf("error73!\n"); exit(-1); }
		next();
		expr(")");
		if(strcmp(tks, ")")) { printf("error74!\n"); exit(-1); }
		next();
		if(strcmp(tks, ";")) { printf("error75!\n"); exit(-1); }
		*e++ = JZ; int *_e2 = e++;
		*e++ = JMP; *e++ = _e1 - emit;
		*_e2 = e - emit;
	} else if(tki == For) {
		inblock();
		next();
		if(!strcmp(tks, "(")) next(); else { printf("error76!\n"); exit(-1); }
		if(strcmp(tks, ";")) {
			if(tki == Int || tki == Char) declare(LOC);
			else expr("");
			if(strcmp(tks, ";")) { printf("error77!\n"); exit(-1); }
		}
		next();
		int *_e1 = e;
		int *_e4;
		if(strcmp(tks, ";")) {
			expr("");
			if(strcmp(tks, ";")) { printf("error78!\n"); exit(-1); }
			*e++ = JZ; _e4 = e++;
		}
		*e++ = JMP; int *_e3 = e++; 
		next();
		int *_e2 = e;
		if(strcmp(tks, ")")) {
			expr(")");
			if(strcmp(tks, ")")) { printf("error79!\n"); exit(-1); }
			*e++ = JMP; *e++ = _e1 - emit;
		}
		next();
		*_e3 = e - emit;
		stmt();
		*e++ = JMP; *e++ = _e2 - emit;
		*_e4 = e - emit;
		outblock();
	} else if(tki == While) {
		int *_e1 = e;
		next(); if(strcmp(tks, "(")) { printf("error80!\n"); exit(-1); }
		next(); expr(")");
		if(strcmp(tks, ")")) { printf("error81!\n"); exit(-1); }
		next();
		*e++ = JZ; int *_e2 = e++;
		stmt();
		*e++ = JMP; *e++ = _e1 - emit;
		*_e2 = e - emit;
	} else if(tki == Return) {
		next(); expr("");
		*e++ = MOV; *e++ = SP; *e++ = BP; //sp = bp
		*e++ = POP; *e++ = BP;
		*e++ = POP; *e++ = IP;
	} else {
		if(strcmp(tks, ";")) {
			expr("");
			if(strcmp(tks, ";")) { printf("error82!\n"); exit(-1); }
		}
	}
}
