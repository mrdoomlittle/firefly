# include "gen.h"
# ifndef __ffly_mscarcity
# define _ NULL
void(*emit_tbl[])(ff_compilerp, struct node*) = {
	_,	// syput
	_,	// ret
	_,	// if
	_,	// decl
	_,	// assign
	_,	// print
	_,	// literal
	_,	// print vall
	_,	// var
	_,	// eq
	_,	// neq
	_,	// gt
	_,	// lt
	_,	// func
	_,	// func call
	_,	// struct reg
	_,	// exit
	_,	// while
	_,	// incr
	_,	// decr
	_,	// match
	_,	// call
	_,	// addrof
	_,	// conv
	_,	// cast
	_,	// brk
	_,	// add
	_,	// sub
	_,	// mul
	_,	// div
	_,	// as
	_,	// out
	_,	// label
	_,	// jmpto
	_,	// va_args
	_	// deref
};
# endif
