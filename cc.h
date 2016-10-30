#ifndef _CC_H_
#define _CC_H_

#define MAXSIZE 10000
#define BUFSIZE 100

typedef struct Type {
	int base;
	int count;
	struct Type *rely;
	struct Type **argtyls;
} Type;

typedef struct {
	int csmk; //class mark
	char *name;
	Type *type;
	int offset;
	int class;
} Id;

typedef struct {
	Type *type;
	int is_lvalue;
	int is_const;
} Er;

typedef void (*Api)(void);

enum {
	//keyword
	Int, Char, Void, If, Else, While, Do, For, Return, Null,
	//type
	INT, CHAR, VOID, NUL, FUN, API, PTR, ARR,
	//other integer
	ID, GLO, LOC, ARG, STR,
	//opcode
	PUSH, POP, SET, INC, DEC, JMP, JZ, MOV, ADD, SUB, MUL, DIV, MOD, EQ, GT, LT, AND, OR, NOT, AG, AL, VAL, ASS, CALL, CAPI, EXIT,
	//reg
	IP = 0, BP, SP, AX
};

extern Id *gid, *lid;
extern char *tks;
extern int tki, line, *e, *emit, *data;

//id.c
void id_init(void);
Type** getargtyls(int count);
void print_ids(void);
int typesize(Type *type);
Id* sgetstr(char *tks);
Id* setid(Type *type, int scope);
Id* getid(char *tks);
void inblock(void);
void outblock(void);
void inparam(void);
void infunc(void);
void outfunc(void);

//declare.c
void declare_init(void);
Type* deriv_type(int base, Type *rely, int count);
void print_type(Id *this_id);
void declare(int scope);

//stmt.c
void stmt(void);

//expr.c
int const_ptr(Type *type);
int const_expr(char *last_opr);
void expr_arr(int scope, Type *type, int offset);
void type_check(Type *type1, Type *type2, char *opr);
Er expr(char *last_opr);

//token.c
void token_init(void);
void tokensrc(char *str);
void next(void);
void peek(void);

//parsegen.c
void parsegen_init(void);
void parsegen(char *str);

//vm.c
void vm_init(void);
void vm_run(int src, int debug);

//api.c
void api_init(void);
void api_register(Api fun, char *proto);
void api_call(int offset);
int api_getint(int index);
char api_getchar(int index);
char* api_getstr(int index);
void api_setint(int i);
void api_setchar(char c);
void api_setstr(char *s);

#endif