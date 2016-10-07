//符号表、参数类型表

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Id *gid, *lid;

static Id *gids, *lids;
static Type **argtyls, **argtylss;

void id_init(void) {
	static int is_init = 0;
	if(!is_init) {
		gids = gid = (Id*)malloc(MAXSIZE * sizeof(Id));
		lids = lid = (Id*)malloc(MAXSIZE * sizeof(Id));
		argtylss = argtyls = (Type**)malloc(MAXSIZE * sizeof(Type*));
		gid++ -> csmk = GLO;
		is_init = 1;
	}
}

Type** getargtyls(int count) {
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

void print_ids(void) {
	printf("--- GLO ---\n");
	for(Id *i = gids; i < gid; i++){
		if(i -> csmk == GLO) printf("GLO");
		else if(i -> csmk == STR) printf("STR : %s", i -> name);
		else if(i -> csmk == ID) {
			printf("%s ", i -> name);
			printf("%d ", i -> offset);
			if(i -> class == GLO) printf("GLO ");
			print_type(i);
		}
		printf("\n");
	}
	printf("--- LOC ---\n");
	for(Id *i = lids; i < lid; i++){
		if(i -> csmk == FUN) printf("FUN");
		else if(i -> csmk == LOC) printf("LOC");
		else if(i -> csmk == ID) {
			printf("%s ", i -> name);
			printf("%d ", i -> offset);
			if(i -> class == ARG) printf("ARG ");
			if(i -> class == LOC) printf("LOC ");
			print_type(i);
		}
		printf("\n");
	}
	printf("\n");
}

int typesize(Type *type) {
	if(type -> base == INT) return 1;
	else if(type -> base == CHAR) return 1;
	else if(type -> base == PTR) return 1;
	else if(type -> base == ARR) return typesize(type -> rely) * type -> count;
	return 0;
}

Id* sgetstr(char *tks) {
	for(Id *i = gid - 1; i > gids; i--) {
		if(i -> csmk == STR && !strcmp(tks, i -> name)) return i;
	}
	
	Id *this_id = gid++;
	Id *last_id = this_id - 1;
	
	this_id -> name = tks;
	this_id -> type = deriv_type(ARR, deriv_type(CHAR, NULL, 0), strlen(tks) + 1);
	this_id -> csmk = STR;
	
	while(last_id -> csmk == ID &&
	      (last_id -> type -> base == FUN ||
	       last_id -> type -> base == API)) last_id--;
	
	if(last_id == gids) this_id -> offset = MAXSIZE - typesize(this_id -> type);
	else this_id -> offset = last_id -> offset - typesize(this_id -> type);
	
	for(int i = 0; i < strlen(tks) + 1; i++) {
		(data + this_id -> offset)[i] = tks[i];
	}
	
	return this_id;
}

Id* setid(Type* type, int env) {
	if(type -> base == PTR) {
		Type *rely = type -> rely;
		while(rely -> base == PTR) rely = rely -> rely;
		if(rely -> base == FUN) lid -= type -> rely -> count + 1;
	} else if(type -> base == FUN && env == ARG) {
		lid -= type -> count + 1;
		type = deriv_type(PTR, type, 0);
	} else if(type -> base == ARR && env == ARG) {
		type = deriv_type(PTR, type -> rely, 0);
	}
	
	Id *this_id;
	if(env == GLO) this_id = gid++;
	else this_id = lid++;
	
	for(Id *i = this_id - 1; i -> csmk == ID; i--) {
		if(!strcmp(this_id -> name, i -> name)) { printf("line %d: error67!\n", line); exit(-1); }
	}
	
	//this_id -> name = tks;
	this_id -> type = type;
	this_id -> csmk = ID;
	this_id -> class = env;
	
	Id *last_id = this_id - 1;
	if(env == GLO) {
		while(last_id -> csmk == ID &&
			  (last_id -> type -> base == FUN ||
			   last_id -> type -> base == API)) last_id--;
		if(last_id -> csmk == GLO) this_id -> offset = MAXSIZE - typesize(type);
		else this_id -> offset = last_id -> offset - typesize(type);
	} else {
		while(last_id -> csmk == LOC) last_id--;
		if(last_id -> csmk == FUN || last_id -> class == ARG) this_id -> offset = 0;
		else this_id -> offset = last_id -> offset + typesize(last_id -> type);
	}
	return this_id;
}

Id* getid(char *tks) {
	for(Id *i = lid - 1; i > lids; i--) {
		if(i -> csmk == ID && !strcmp(tks, i -> name)) return i;
	}
	for(Id *i = gid - 1; i > gids; i--) {
		if(i -> csmk == ID && !strcmp(tks, i -> name)) return i;
	}
	printf("line %d: error68!\n", line); exit(-1);
}

void inblock(void) {
	(lid++) -> csmk = LOC;
}

void outblock(void) {
	do {
		lid--;
	} while(lid -> csmk != LOC);
}

void inparam(void) {
	(lid++) -> csmk = FUN;
}

void infunc(void) {
	Id *i = lid - 1;
	int argc = 0;
	while(i -> csmk != FUN) {
		(i--) -> offset -= (argc++) + 3;
	}
}

void outfunc(void) {
	lid = lids;
}
