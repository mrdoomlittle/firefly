	.globl FF_LP
.data
FF_LP: .quad
.text
	.extern _ffstart
	.globl _start
_start:
	movq %rsp, FF_LP(%rip)
	xorq %rbp, %rbp
	// align stack
	andq $~15, %rsp

	// as its a c function it will think that we are using call
	// push jump{return as not using call}
	push %rax
	// no point in calling function as its not going to return anyway
	jmp _ffstart
