all:
	@gcc cc.c declare.c expr.c id.c stmt.c token.c vm.c api.c gen.c -o cc -Wall