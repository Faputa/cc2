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
	gid++->csmk = GLO;
}

void print_ids(void) {
	printf("--- GLO ---\n");
	for(Id *i = gids; i < gid; i++) {
		if(i->csmk == GLO) printf("GLO");
		else if(i->csmk == STR) printf("STR : %s", i->name);
		else if(i->csmk == ID) {
			printf("%s ", i->name);
			printf("%d ", i->offset);
			if(i->class == GLO) printf("GLO ");
			print_type(i);
		}
		printf("\n");
	}
	printf("--- LOC ---\n");
	for(Id *i = lids; i < lid; i++) {
		if(i->csmk == FUN) printf("FUN");
		else if(i->csmk == LOC) printf("LOC");
		else if(i->csmk == ID) {
			printf("%s ", i->name);
			printf("%d ", i->offset);
			if(i->class == ARG) printf("ARG ");
			if(i->class == LOC) printf("LOC ");
			print_type(i);
		}
		printf("\n");
	}
	printf("\n");
}

Id* sgetstr(char *tks) {
	for(Id *i = gid - 1; i > gids; i--) {
		if(i->csmk == STR && !strcmp(tks, i->name)) return i;
	}
	
	Id *this_id = gid++;
	Id *last_id = this_id - 1;
	
	this_id->name = tks;
	this_id->type = type_derive(ARR, typechar, strlen(tks) + 1);
	this_id->csmk = STR;
	
	while(last_id->csmk == ID && (last_id->type->base == FUN || last_id->type->base == API)) last_id--;
	if(last_id == gids) this_id->offset = MAXSIZE - type_size(this_id->type);
	else this_id->offset = last_id->offset - type_size(this_id->type);
	
	for(int i = 0; i < strlen(tks) + 1; i++) {
		(data + this_id->offset)[i] = tks[i];
	}
	
	return this_id;
}

void setid(Type* type, Id *id) {
	for(Id *i = id - 1; i->csmk == ID || i->csmk == STR; i--) {
		if(!strcmp(id->name, i->name) && i->csmk != STR) error("line %d: error!\n", line);
	}
	
	id->type = type;
	id->csmk = ID;
	
	Id *last_id = id - 1;
	if(id->class == GLO) {
		gid = id + 1;
		while(last_id->csmk == ID && (last_id->type->base == FUN || last_id->type->base == API)) last_id--;
		if(last_id->csmk == GLO) id->offset = MAXSIZE - type_size(type);
		else id->offset = last_id->offset - type_size(type);
	} else if(id->class == LOC) {
		lid = id + 1;
		while(last_id->csmk == LOC) last_id--;
		if(last_id->csmk == FUN || last_id->class == ARG) id->offset = 0;
		else id->offset = last_id->offset + type_size(last_id->type);
	} else if(id->class == ARG) {
		lid = id + 1;
		if(last_id->csmk == FUN) id->offset = -3;
		else id->offset = last_id->offset - type_size(last_id->type);
	} else error("line %d: error!\n", line);
}

Id* getid(char *tks) {
	for(Id *i = lid - 1; i > lids; i--) {
		if(i->csmk == ID && !strcmp(tks, i->name)) return i;
	}
	for(Id *i = gid - 1; i > gids; i--) {
		if(i->csmk == ID && !strcmp(tks, i->name)) return i;
	}
	error("line %d: error!\n", line);
	return NULL; //make compiler happy
}

void inblock(void) {
	(lid++)->csmk = LOC;
}

void outblock(void) {
	do {
		lid--;
	} while(lid->csmk != LOC);
}

void infunc(void) {
	(lid++)->csmk = FUN;
}

void outfunc(void) {
	lid = lids;
}
