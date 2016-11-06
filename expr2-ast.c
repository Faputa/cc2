//表达式计算器
//递归下降分析
//遇到终结符必须主动移动tk

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define MAXSIZE 1000

enum {
	//type
	INT,
	//node kind
	ADD, SUB, MUL, DIV, ATOM
};

typedef struct Node {
	int kind;
	int value;
	struct Node *child[2];
} Node;

int tki;
char *tks, *p;

void next() {
	tks = ""; tki = -1;
	while(*p) {
		if(*p >= '0' && *p <= '9') {
			int len = 0; char *_p = p;
			while(*p >= '0' && *p <= '9') {
				len++; p++;
			}
			tki = INT;
			tks = (char*)malloc(sizeof(char) * (len+1));
			strncpy(tks, _p, len);
			tks[len] = '\0';
			return;
		}
		else if(*p == '+') { tks = "+"; p++; return; }
		else if(*p == '-') { tks = "-"; p++; return; }
		else if(*p == '*') { tks = "*"; p++; return; }
		else if(*p == '/') { tks = "/"; p++; return; }
		else if(*p == '(') { tks = "("; p++; return; }
		else if(*p == ')') { tks = ")"; p++; return; }
		else { //跳过不能识别的符号
			p++;
		}
	}
}

Node* newNode() {
	Node *n = (Node*)malloc(sizeof(Node));
	n->child[0] = n->child[1] = NULL;
	return n;
}

Node* expr();
Node* atom() { //atom -> int | "(" expr ")"
	Node *n;
	if(tki == INT) {
		n = newNode();
		n->kind = ATOM;
		n->value = atoi(tks);
		next();
	} else if(!strcmp(tks, "(")) {
		next();
		n = expr();
		if(strcmp(tks, ")")) { printf("error!\n"); exit(-1); } //"("无法匹配到")"
		next();
	} else { printf("error!\n"); exit(-1); }
	return n;
}

Node* muldiv() { //muldiv -> atom ["*" muldiv | "/" muldiv]
	Node *n = atom();
	if(!strcmp(tks, "*")) {
		next();
		Node *_n = n;
		n = newNode();
		n->kind = MUL;
		n->child[0] = _n;
		n->child[1] = muldiv();
	} else if(!strcmp(tks, "/")) {
		next();
		Node *_n = n;
		n = newNode();
		n->kind = DIV;
		n->child[0] = _n;
		n->child[1] = muldiv();
	}
	return n;
}

Node* addsub() { //addsub -> muldiv ["+" addsub | "-" addsub]
	Node *n = muldiv();
	if(!strcmp(tks, "+")) {
		next();
		Node *_n = n;
		n = newNode();
		n->kind = ADD;
		n->child[0] = _n;
		n->child[1] = addsub();
	} else if(!strcmp(tks, "-")) {
		next();
		Node *_n = n;
		n = newNode();
		n->kind = SUB;
		n->child[0] = _n;
		n->child[1] = addsub();
	}
	return n;
}

Node* expr() { //expr -> addsub
	return addsub();
}

int runNode(Node *n) {
	if(n->kind == ATOM) return n->value;
	int lopr = runNode(n->child[0]);
	int ropr = runNode(n->child[1]);
	switch(n->kind) {
	case ADD: return lopr + ropr;
	case SUB: return lopr - ropr;
	case MUL: return lopr * ropr;
	case DIV: return lopr / ropr;
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) { printf("error!\n"); exit(-1); }
	p = argv[1];
	next();
	int r = runNode(expr());
	if(strcmp(tks, ";") && strcmp(tks, "")) { printf("error!\n"); exit(-1); }
	printf("%d\n", r);
	return 0;
}
