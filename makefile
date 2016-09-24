all:
	@gcc cc.c declare.c expr.c id.c stmt.c token.c vm.c api.c -o cc -Wall