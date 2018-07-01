	.extern _res_fi
	.extern _res_end
	.extern res_r0
	.extern res_r1
	.extern res_bp
	.extern res_sp
	.extern res_stack

	.global _res_add1br0r1
	.global _res_add1wr0r1
	.global _res_add1dr0r1
	.global _res_add1qr0r1

	.global _res_add1br1r0
	.global _res_add1wr1r0
	.global _res_add1dr1r0
	.global _res_add1qr1r0

	.globl _res_inc1br1
	.globl _res_inc1wr1
	.globl _res_inc1dr1
	.globl _res_inc1qr1
	.globl _res_inc1br0
	.globl _res_inc1wr0
	.globl _res_inc1dr0
	.globl _res_inc1qr0
	
	.globl _res_dec1br1
	.globl _res_dec1wr1
	.globl _res_dec1dr1
	.globl _res_dec1qr1
	.globl _res_dec1br0
	.globl _res_dec1wr0
	.globl _res_dec1dr0
	.globl _res_dec1qr0

	.globl _res_mov3br0r1
	.globl _res_mov3wr0r1
	.globl _res_mov3dr0r1
	.globl _res_mov3qr0r1

	.globl _res_mov3br1r0
	.globl _res_mov3wr1r0
	.globl _res_mov3dr1r0
	.globl _res_mov3qr1r0

	.globl _res_push1br1
	.globl _res_push1wr1
	.globl _res_push1dr1
	.globl _res_push1qr1

	.globl _res_push1br0
	.globl _res_push1wr0
	.globl _res_push1dr0
	.globl _res_push1qr0

	.globl _res_pop1br1
	.globl _res_pop1wr1
	.globl _res_pop1dr1
	.globl _res_pop1qr1

	.globl _res_pop1br0
	.globl _res_pop1wr0
	.globl _res_pop1dr0
	.globl _res_pop1qr0

	.globl _res_lbpr1
	.globl _res_lbpr0
	.globl _res_lspr1
	.globl _res_lspr0
_res_lbpr1:
	movq res_bp(%rip), %r8
	movq %r8, res_r1(%rip)
	jmp _res_fi
_res_lbpr0:
	movq res_bp(%rip), %r8
	movq %r8, res_r0(%rip)
	jmp _res_fi
_res_lspr1:
	movq res_sp(%rip), %r8
	movq %r8, res_r1(%rip)
	jmp _res_fi
_res_lspr0:
	movq res_sp(%rip), %r8
	movq %r8, res_r0(%rip)
	jmp _res_fi

_res_push1br1:
	movq res_sp(%rip), %r8
	cmpq $0, (%r8)
	je _res_end

	decq (%r8)

	movq res_stack(%rip), %r9
	addq (%r8), %r9
	movq res_r1(%rip), %r10
	movb (%r10), %r10b	
	movb %r10b, (%r9)
	jmp _res_fi
_res_push1wr1:
	movq res_sp(%rip), %r8
	cmpq $2, (%r8)
	jb _res_end

	subq $2, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r1(%rip), %r10
	movw (%r10), %r10w
	movw %r10w, (%r9)
	jmp _res_fi
_res_push1dr1:
	movq res_sp(%rip), %r8
	cmpq $4, (%r8)
	jb _res_end

	subq $4, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r1(%rip), %r10
	movl (%r10), %r10d  
	movl %r10d, (%r9)
	jmp _res_fi
_res_push1qr1:
	movq res_sp(%rip), %r8
	cmpq $8, (%r8)
	jb _res_end

	subq $8, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r1(%rip), %r10
	movq (%r10), %r10
	movq %r10, (%r9)
	jmp _res_fi
_res_push1br0:
	movq res_sp(%rip), %r8
	cmpq $0, (%r8)
 	je _res_end

	decq (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r0(%rip), %r10
	movb (%r10), %r10b  
	movb %r10b, (%r9)
	jmp _res_fi
_res_push1wr0:
	movq res_sp(%rip), %r8
	cmpq $2, (%r8)
	jb _res_end

	subq $2, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r0(%rip), %r10
	movw (%r10), %r10w  
	movw %r10w, (%r9)
	jmp _res_fi
_res_push1dr0:
	movq res_sp(%rip), %r8

	cmpq $4, (%r8)
	jb _res_end

	subq $4, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r0(%rip), %r10
	movl (%r10), %r10d
	movl %r10d, (%r9)
	jmp _res_fi
_res_push1qr0:
	movq res_sp(%rip), %r8
	cmpq $8, (%r8)
	jb _res_end

	subq $8, (%r8)

	movq res_stack(%rip), %r9 
	addq (%r8), %r9 
	movq res_r0(%rip), %r10
	movq (%r10), %r10
	movq %r10, (%r9)
	jmp _res_fi

_res_pop1br1:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	decq (%r8)
	movq res_r1(%rip), %r10
	movb (%r9), %r9b
	movb %r9b, (%r10)
	jmp _res_fi
_res_pop1wr1:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $2, (%r8)
	movq res_r1(%rip), %r10
	movw (%r9), %r9w
	movw %r9w, (%r10)
	jmp _res_fi
_res_pop1dr1:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $4, (%r8)
	movq res_r1(%rip), %r10
	movl (%r9), %r9d
	movl %r9d, (%r10)
	jmp _res_fi
_res_pop1qr1:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $8, (%r8)
	movq res_r1(%rip), %r10
	movq (%r9), %r9
	movq %r9, (%r10)
	jmp _res_fi
_res_pop1br0:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	decq (%r8)
	movq res_r0(%rip), %r10
	movb (%r9), %r9b
	movb %r9b, (%r10)
	jmp _res_fi
_res_pop1wr0:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $2, (%r8)
	movq res_r0(%rip), %r10
	movw (%r9), %r9w
	movw %r9w, (%r10)
	jmp _res_fi
_res_pop1dr0:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $4, (%r8)
	movq res_r0(%rip), %r10
	movl (%r9), %r9d
	movl %r9d, (%r10)
	jmp _res_fi
_res_pop1qr0:
	movq res_sp(%rip), %r8
	movq res_stack(%rip), %r9
	addq (%r8), %r9
	addq $8, (%r8)
	movq res_r0(%rip), %r10
	movq (%r9), %r9
	movq %r9, (%r10)
	jmp _res_fi

_res_mov3br0r1:
	movq res_r0(%rip), %r8
	movb (%r8), %r8b
	movq res_r1(%rip), %r9
	movb %r8b, (%r9)
	jmp _res_fi
_res_mov3wr0r1:
	movq res_r0(%rip), %r8
	movw (%r8), %r8w
	movq res_r1(%rip), %r9
	movw %r8w, (%r9)
	jmp _res_fi
_res_mov3dr0r1:
	movq res_r0(%rip), %r8
	movl (%r8), %r8d
	movq res_r1(%rip), %r9
	movl %r8d, (%r9) 
	jmp _res_fi
_res_mov3qr0r1:
	movq res_r0(%rip), %r8
	movq (%r8),   %r8
	movq res_r1(%rip), %r9
	movq %r8, (%r9)
	jmp _res_fi

_res_mov3br1r0:
	movq res_r1(%rip), %r8
	movb (%r8), %r8b
	movq res_r0(%rip), %r9
	movb %r8b, (%r9)
	jmp _res_fi
_res_mov3wr1r0:
	movq res_r1(%rip), %r8
	movw (%r8), %r8w
	movq res_r0(%rip), %r9
	movw %r8w, (%r9)
	jmp _res_fi
_res_mov3dr1r0:
	movq res_r1(%rip), %r8
	movl (%r8), %r8d
	movq res_r0(%rip), %r9
	movl %r8d, (%r9)
	jmp _res_fi
_res_mov3qr1r0:
	movq res_r1(%rip), %r8
	movq (%r8), %r8
	movq res_r0(%rip), %r9
	movq %r8, (%r9)
	jmp _res_fi

_res_add1br0r1:
	movq res_r0(%rip), %r8 
	movb (%r8), %r8b
	movq res_r1(%rip), %r9 
	addb %r8b, (%r9)
	jmp _res_fi
_res_add1wr0r1:
	movq res_r0(%rip), %r8 
	movw (%r8), %r8w
	movq res_r1(%rip), %r9 
	addw %r8w, (%r9)
	jmp _res_fi
_res_add1dr0r1:
	movq res_r0(%rip), %r8 
	movl (%r8), %r8d
	movq res_r1(%rip), %r9 
	addl %r8d, (%r9)
	jmp _res_fi
_res_add1qr0r1:
	movq res_r0(%rip), %r8 
	movq (%r8), %r8
	movq res_r1(%rip), %r9 
	addq %r8, (%r9)
	jmp _res_fi

_res_add1br1r0:
	movq res_r1(%rip), %r8 
	movb (%r8), %r8b
	movq res_r0(%rip), %r9 
	addb %r8b, (%r9)
	jmp _res_fi
_res_add1wr1r0:
	movq res_r1(%rip), %r8 
	movw (%r8), %r8w
	movq res_r0(%rip), %r9 
	addw %r8w, (%r9)
	jmp _res_fi
_res_add1dr1r0:
	movq res_r1(%rip), %r8 
	movl (%r8), %r8d
	movq res_r0(%rip), %r9 
	addl %r8d, (%r9)
	jmp _res_fi
_res_add1qr1r0:
	movq res_r1(%rip), %r8 
	movq (%r8), %r8
	movq res_r0(%rip), %r9 
	addq %r8, (%r9)
	jmp _res_fi

_res_inc1br1:
	movq res_r1(%rip), %r8
	incb (%r8)
	jmp _res_fi
_res_inc1wr1:
	movq res_r1(%rip), %r8
	incw (%r8)
	jmp _res_fi
_res_inc1dr1:
	movq res_r1(%rip), %r8
	incl (%r8)
	jmp _res_fi
_res_inc1qr1:
	movq res_r1(%rip), %r8
	incq (%r8)
	jmp _res_fi

_res_inc1br0:
	movq res_r0(%rip), %r8
	incb (%r8)
	jmp _res_fi
_res_inc1wr0:
	movq res_r0(%rip), %r8
	incw (%r8)
	jmp _res_fi
_res_inc1dr0:
	movq res_r0(%rip), %r8
	incl (%r8)
	jmp _res_fi
_res_inc1qr0:
	movq res_r0(%rip), %r8
	incq (%r8)
	jmp _res_fi

_res_dec1br1:
	movq res_r1(%rip), %r8
	decb (%r8)
	jmp _res_fi
_res_dec1wr1:
	movq res_r1(%rip), %r8
	decw (%r8)
	jmp _res_fi
_res_dec1dr1:
	movq res_r1(%rip), %r8
	decl (%r8)
	jmp _res_fi
_res_dec1qr1:
	movq res_r1(%rip), %r8
	decq (%r8)
	jmp _res_fi

_res_dec1br0:
	movq res_r0(%rip), %r8
	decb (%r8)
	jmp _res_fi
_res_dec1wr0:
	movq res_r0(%rip), %r8
	decw (%r8)
	jmp _res_fi
_res_dec1dr0:
	movq res_r0(%rip), %r8
	decl (%r8)
	jmp _res_fi
_res_dec1qr0:
	movq res_r0(%rip), %r8
	decq (%r8)
	jmp _res_fi
