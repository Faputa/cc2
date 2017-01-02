all:
	@gcc cc.c declare.c expr.c ident.c stmt.c token.c vm.c api.c parsegen.c type.c -o cc -Wall