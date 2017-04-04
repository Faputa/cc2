//符号表

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Id *gid, *lid;

static Id *gids, *lids;

void ident_init(void) {
	gids = gid = (Id*)malloc(MAXSIZE * sizeof(Id));
	lids = lid = (Id*)malloc(MAXSIZE * sizeof(Id));
	gid++->kind = GLO;
}

void print_ids(void) {
	printf("--- GLO ---\n");
	for(Id *i = gids; i < gid; i++) {
		if(i->kind == GLO) printf("GLO");
		else if(i->kind == STR) printf("STR : %s", i->name);
		else if(i->kind == ID) {
			printf("%s ", i->name);
			printf("%d ", i->offset);
			if(i->idkind == GLO) printf("GLO ");
			print_type(i);
		}
		printf("\n");
	}
	printf("--- LOC ---\n");
	for(Id *i = lids; i < lid; i++) {
		if(i->kind == FUN) printf("FUN");
		else if(i->kind == LOC) printf("LOC");
		else if(i->kind == ID) {
			printf("%s ", i->name);
			printf("%d ", i->offset);
			if(i->idkind == ARG) printf("ARG ");
			if(i->idkind == LOC) printf("LOC ");
			print_type(i);
		}
		printf("\n");
	}
	printf("\n");
}

Id* sgetstr(char *tks) {
	for(Id *i = gid - 1; i > gids; i--) {
		if(i->kind == STR && !strcmp(tks, i->name)) return i;
	}
	
	Id *this_id = gid++;
	Id *last_id = this_id - 1;
	
	this_id->name = tks;
	this_id->type = type_derive(ARR, typechar, strlen(tks) + 1);
	this_id->kind = STR;
	
	while(last_id->kind == ID && (last_id->type->tykind == FUN || last_id->type->tykind == API)) last_id--;
	if(last_id == gids) this_id->offset = MAXSIZE - type_size(this_id->type);
	else this_id->offset = last_id->offset - type_size(this_id->type);
	
	for(int i = 0; i < strlen(tks) + 1; i++) {
		(data + this_id->offset)[i] = tks[i];
	}
	
	return this_id;
}

void setid(Type* type, Id *id) {
	for(Id *i = id - 1; i->kind == ID || i->kind == STR; i--) {
		if(i->kind == ID && !strcmp(id->name, i->name)) error("line %d: error!\n", line);
	}
	
	id->type = type;
	id->kind = ID;
	
	Id *last_id = id - 1;
	if(id->idkind == GLO) {
		gid = id + 1;
		while(last_id->kind == ID && (last_id->type->tykind == FUN || last_id->type->tykind == API)) last_id--;
		if(last_id->kind == GLO) id->offset = MAXSIZE - type_size(type);
		else id->offset = last_id->offset - type_size(type);
	} else if(id->idkind == LOC) {
		lid = id + 1;
		while(last_id->kind == LOC) last_id--;
		if(last_id->kind == FUN || last_id->idkind == ARG) id->offset = 0;
		else id->offset = last_id->offset + type_size(last_id->type);
	} else if(id->idkind == ARG) {
		lid = id + 1;
		if(last_id->kind == FUN) id->offset = -2 - type_size(type);
		else id->offset = last_id->offset - type_size(type);
	} else error("line %d: error!\n", line);
}

Id* getid(char *tks) {
	for(Id *i = lid - 1; i > lids; i--) {
		if(i->kind == ID && !strcmp(tks, i->name)) return i;
	}
	for(Id *i = gid - 1; i > gids; i--) {
		if(i->kind == ID && !strcmp(tks, i->name)) return i;
	}
	error("line %d: error!\n", line);
	return NULL; //to avoid warnings
}

void inblock(void) {
	(lid++)->kind = LOC;
}

void outblock(void) {
	do {
		lid--;
	} while(lid->kind != LOC);
}

void infunc(void) {
	(lid++)->kind = FUN;
}

void outfunc(void) {
	lid = lids;
}
