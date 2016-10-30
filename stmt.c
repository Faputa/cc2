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
			if(tki == Int || tki == Char || tki == Void) declare(LOC);
			else stmt();
			next();
		}
		outblock();
	} else if(tki == If) {
		next(); if(strcmp(tks, "(")) { printf("line %d: error70!\n", line); exit(-1); }
		next(); expr(")");
		if(strcmp(tks, ")")) { printf("line %d: error71!\n", line); exit(-1); }
		next();
		*e++ = JZ; int *_e1 = e++;
		stmt();
		peek();
		if(tki == Else) {
			next();
			*e++ = JMP; int *_e2 = e++;
			*_e1 = e - emit;
			next();
			stmt();
			*_e2 = e - emit;
		} else {
			*_e1 = e - emit;
		}
	} else if(tki == Do) {
		int *_e1 = e;
		next();
		stmt();
		next();
		if(tki == While) next(); else { printf("line %d: error72!\n", line); exit(-1); }
		if(strcmp(tks, "(")) { printf("line %d: error73!\n", line); exit(-1); }
		next();
		expr(")");
		if(strcmp(tks, ")")) { printf("line %d: error74!\n", line); exit(-1); }
		next();
		if(strcmp(tks, ";")) { printf("line %d: error75!\n", line); exit(-1); }
		*e++ = JZ; int *_e2 = e++;
		*e++ = JMP; *e++ = _e1 - emit;
		*_e2 = e - emit;
	} else if(tki == For) {
		inblock();
		next();
		if(!strcmp(tks, "(")) next(); else { printf("line %d: error76!\n", line); exit(-1); }
		if(strcmp(tks, ";")) {
			if(tki == Int || tki == Char || tki == Void) declare(LOC);
			else expr("");
			if(strcmp(tks, ";")) { printf("line %d: error77!\n", line); exit(-1); }
		}
		next();
		int *_e1 = e;
		int *_e4;
		if(strcmp(tks, ";")) {
			expr("");
			if(strcmp(tks, ";")) { printf("line %d: error78!\n", line); exit(-1); }
			*e++ = JZ; _e4 = e++;
		}
		*e++ = JMP; int *_e3 = e++; 
		next();
		int *_e2 = e;
		if(strcmp(tks, ")")) {
			expr(")");
			if(strcmp(tks, ")")) { printf("line %d: error79!\n", line); exit(-1); }
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
		next(); if(strcmp(tks, "(")) { printf("line %d: error80!\n", line); exit(-1); }
		next(); expr(")");
		if(strcmp(tks, ")")) { printf("line %d: error81!\n", line); exit(-1); }
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
			if(strcmp(tks, ";")) { printf("line %d: error82!\n", line); exit(-1); }
		}
	}
}
