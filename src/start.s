	.extern _ffstart
	.globl _start
_start:
	xorq %rbp, %rbp
	// align stack
	andq $~15, %rsp

	// no point in calling function as its not going to return anyway
	jmp _ffstart
