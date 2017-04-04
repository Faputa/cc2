#ifndef _CC_H_
#define _CC_H_

#define MAXSIZE 10000
#define BUFSIZE 100

typedef struct Type {
	int tykind;
	int count;
	struct Type *base;
	struct Type **argtyls;
} Type;

typedef struct {
	int kind;
	int idkind;
	int offset;
	char *name;
	Type *type;
} Sym;

typedef struct {
	Type *type;
	int is_lvalue;
	int is_const;
} Er;

typedef void (*Api)(void);

enum {
	//keyword
	Int, Char, Void, If, Else, While, Do, For, Return, Null,
	//other tag
	ID, STR, GLO, LOC, ARG,
	//type
	INT, CHAR, VOID, NUL, FUN, API, PTR, ARR,
	//opcode
	PUSH, POP, SET, INC, DEC, JMP, JZ, MOV, ADD, SUB, MUL, DIV, MOD, EQ, GT, LT, AND, OR, NOT, AG, AL, VAL, ASS, CALL, CAPI, EXIT,
	//reg
	IP = 0, BP, SP, AX
};

extern Sym *gsym, *lsym;
extern char *tks, *p;
extern int tki, line, *e, *emit, *data;
extern Type *typeint, *typechar, *typenull;

//symbol.c
void symbol_init(void);
void print_sym(void);
Sym* sgetstr(char *tks);
void setid(Type *type, Sym *id);
Sym* getid(char *tks);
void inblock(void);
void outblock(void);
void infunc(void);
void outfunc(void);

//type.c
void type_init(void);
int type_size(Type *type);
void type_check(Type *type1, Type *type2, char *opr);
Type* type_derive(int tykind, Type *base, int count);
void print_type(Sym *this_id);

//declare.c
void declare_init(void);
void declare_loc(void);
void declare_glo(void);

//error.c
void error(char *fmt, ...);
void warning(char *fmt, ...);

//stmt.c
void stmt(void);

//expr.c
int ptr_const(Type *type);
void arr_init_glo(Type *type, int offset);
void arr_init_loc(Type *type, int offset);
int expr_const(char *last_opr);
Er expr(char *last_opr);

//token.c
void token_init(void);
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
