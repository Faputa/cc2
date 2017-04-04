all:
	@gcc api.c cc.c declare.c error.c expr.c symbol.c parsegen.c stmt.c token.c type.c vm.c -o cc -Wall