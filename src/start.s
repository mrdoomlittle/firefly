	.extern _ffstart
	.globl _start
_start:
	xorq %rbp, %rbp
	// align stack
	andq $~15, %rsp

	// push jump{return as not using call}
	push %rax
	// no point in calling function as its not going to return anyway
	jmp _ffstart
