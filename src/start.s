	.extern _ffstart
	.globl _start
_start:
	// align stack
	andq $0xfffffffffffffff0, %rsp
	// no point in calling function as its not going to return anyway
	jmp _ffstart
