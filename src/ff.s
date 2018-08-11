	.file	"ff.c"
	.text
	.local	cmdmap
	.comm	cmdmap,16,16
	.section	.rodata
.LC0:
	.string	"help"
	.section	.data.rel.local,"aw"
	.align 16
	.type	__compound_literal.0, @object
	.size	__compound_literal.0, 16
__compound_literal.0:
	.quad	.LC0
	.long	0
	.zero	4
	.section	.rodata
.LC1:
	.string	"exit"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.1, @object
	.size	__compound_literal.1, 16
__compound_literal.1:
	.quad	.LC1
	.long	1
	.zero	4
	.section	.rodata
.LC2:
	.string	"info"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.2, @object
	.size	__compound_literal.2, 16
__compound_literal.2:
	.quad	.LC2
	.long	4
	.zero	4
	.section	.rodata
.LC3:
	.string	"ff5"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.3, @object
	.size	__compound_literal.3, 16
__compound_literal.3:
	.quad	.LC3
	.long	2
	.zero	4
	.section	.rodata
.LC4:
	.string	"ff6"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.4, @object
	.size	__compound_literal.4, 16
__compound_literal.4:
	.quad	.LC4
	.long	3
	.zero	4
	.section	.rodata
.LC5:
	.string	"exec"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.5, @object
	.size	__compound_literal.5, 16
__compound_literal.5:
	.quad	.LC5
	.long	5
	.zero	4
	.section	.rodata
.LC6:
	.string	"p"
	.section	.data.rel.local
	.align 16
	.type	__compound_literal.6, @object
	.size	__compound_literal.6, 16
__compound_literal.6:
	.quad	.LC6
	.long	6
	.zero	4
	.align 32
	.type	cmd_tbl, @object
	.size	cmd_tbl, 64
cmd_tbl:
	.quad	__compound_literal.0
	.quad	__compound_literal.1
	.quad	__compound_literal.2
	.quad	__compound_literal.3
	.quad	__compound_literal.4
	.quad	__compound_literal.5
	.quad	__compound_literal.6
	.quad	0
	.text
	.type	ldcmds, @function
ldcmds:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	leaq	cmd_tbl(%rip), %rax
	movq	%rax, -8(%rbp)
	jmp	.L2
.L3:
	movq	-16(%rbp), %rax
	movl	8(%rax), %edx
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	cmdmap(%rip), %rdi
	call	ffsh_cmdput@PLT
	addq	$8, -8(%rbp)
.L2:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L3
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	ldcmds, .-ldcmds
	.local	line
	.comm	line,512,32
	.local	off
	.comm	off,4,4
	.local	e
	.comm	e,4,4
	.type	_get, @function
_get:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movq	%rdx, -16(%rbp)
	movl	off(%rip), %eax
	movl	%eax, %edx
	leaq	line(%rip), %rax
	leaq	(%rdx,%rax), %rsi
	movl	-8(%rbp), %edx
	movq	-16(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	movq	%rcx, %rdi
	call	ffly_mem_cpy@PLT
	movl	off(%rip), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, off(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_get, .-_get
	.type	_at_eof, @function
_at_eof:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	off(%rip), %edx
	movl	e(%rip), %eax
	cmpl	%eax, %edx
	setnb	%al
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_at_eof, .-_at_eof
	.section	.rodata
.LC7:
	.string	"connect"
.LC8:
	.string	"disconnect"
.LC9:
	.string	"meminfo"
	.text
	.type	_er, @function
_er:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	ffly_str_cmp@PLT
	testb	%al, %al
	jne	.L8
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	ffly_stno@PLT
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	ff_p_connect@PLT
	jmp	.L11
.L8:
	movq	-24(%rbp), %rax
	leaq	.LC8(%rip), %rsi
	movq	%rax, %rdi
	call	ffly_str_cmp@PLT
	testb	%al, %al
	jne	.L10
	call	ff_p_disconnect@PLT
	jmp	.L11
.L10:
	movq	-24(%rbp), %rax
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	call	ffly_str_cmp@PLT
	testb	%al, %al
	jne	.L11
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	ffly_meminfo@PLT
.L11:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_er, .-_er
	.local	run
	.comm	run,1,1
	.section	.rodata
.LC10:
	.string	"\n: line: %s\n"
.LC11:
	.string	"command: %s\n"
.LC12:
	.string	"command does not exist.\n"
.LC13:
	.string	"failed to set tc attr.\n"
	.text
	.globl	ffmain
	.type	ffmain, @function
ffmain:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$176, %rsp
	movl	%edi, -164(%rbp)
	movq	%rsi, -176(%rbp)
	leaq	run(%rip), %rax
	movq	%rax, ffsh_run(%rip)
	movq	ffly_in(%rip), %rax
	movl	8(%rax), %eax
	leaq	-80(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	tcgetattr@PLT
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, -128(%rbp)
	movq	%rdx, -120(%rbp)
	movq	-64(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	%rax, -112(%rbp)
	movq	%rdx, -104(%rbp)
	movl	-48(%rbp), %eax
	movl	%eax, -96(%rbp)
	movl	-68(%rbp), %eax
	andl	$-11, %eax
	movl	%eax, -68(%rbp)
	movq	ffly_in(%rip), %rax
	movl	8(%rax), %eax
	leaq	-80(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	tcsetattr@PLT
	leaq	_er(%rip), %rax
	movq	%rax, ffsh_er(%rip)
	leaq	_get(%rip), %rax
	movq	%rax, get(%rip)
	leaq	_at_eof(%rip), %rax
	movq	%rax, at_eof(%rip)
	leaq	cmdmap(%rip), %rdi
	call	hash_init@PLT
	call	ldcmds
.L13:
	leaq	line(%rip), %rax
	movq	%rax, -16(%rbp)
.L23:
	movq	ffly_out(%rip), %rax
	movq	%rax, %rdi
	call	ffly_l_show@PLT
	movl	$0, -4(%rbp)
	movq	ffly_in(%rip), %rax
	movl	8(%rax), %eax
	leaq	-160(%rbp), %rcx
	movl	$26, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read@PLT
	movl	%eax, -20(%rbp)
	jmp	.L14
.L22:
	movl	-4(%rbp), %eax
	leaq	-160(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -21(%rbp)
	cmpb	$10, -21(%rbp)
	je	.L29
	cmpb	$27, -21(%rbp)
	jne	.L17
	addl	$1, -4(%rbp)
	addl	$1, -4(%rbp)
	movzbl	-158(%rbp), %eax
	cmpb	$68, %al
	jne	.L18
	movl	l_cursor_pos(%rip), %eax
	subl	$1, %eax
	movl	%eax, l_cursor_pos(%rip)
	jmp	.L20
.L18:
	movzbl	-158(%rbp), %eax
	cmpb	$67, %al
	jne	.L20
	movl	l_cursor_pos(%rip), %eax
	addl	$1, %eax
	movl	%eax, l_cursor_pos(%rip)
	jmp	.L20
.L17:
	cmpb	$127, -21(%rbp)
	jne	.L21
	call	ffly_l_del@PLT
	jmp	.L20
.L21:
	cmpb	$31, -21(%rbp)
	jle	.L20
	cmpb	$127, -21(%rbp)
	je	.L20
	movsbl	-21(%rbp), %eax
	movl	%eax, %edi
	call	ffly_l_put@PLT
.L20:
	addl	$1, -4(%rbp)
.L14:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jne	.L22
	jmp	.L23
.L29:
	nop
.L16:
	leaq	line(%rip), %rdi
	call	ffly_l_load@PLT
	movl	%eax, %eax
	addq	%rax, -16(%rbp)
	call	ffly_l_reset@PLT
	movq	-16(%rbp), %rax
	movb	$0, (%rax)
	leaq	line(%rip), %rsi
	leaq	.LC10(%rip), %rdi
	movl	$0, %eax
	call	ffly_printf@PLT
	movq	ffly_out(%rip), %rax
	movq	%rax, %rdi
	call	ffly_fdrain@PLT
	leaq	line(%rip), %rdx
	movq	-16(%rbp), %rax
	subq	%rdx, %rax
	movl	%eax, e(%rip)
	movl	$0, off(%rip)
	call	ffsh_parse@PLT
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	je	.L24
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L24
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	ffly_printf@PLT
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rsi
	leaq	cmdmap(%rip), %rdi
	call	ffsh_cmdget@PLT
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L25
	leaq	.LC12(%rip), %rdi
	movl	$0, %eax
	call	ffly_printf@PLT
	jmp	.L24
.L25:
	movq	-32(%rbp), %rax
	leaq	24(%rax), %rdx
	movq	-32(%rbp), %rax
	movl	184(%rax), %ecx
	movq	-40(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	ffsh_exec_cmd@PLT
.L24:
	call	mem_cleanup@PLT
	movzbl	run(%rip), %eax
	testb	%al, %al
	jne	.L26
	jmp	.L13
.L26:
	leaq	cmdmap(%rip), %rdi
	call	hash_destroy@PLT
	movq	ffly_in(%rip), %rax
	movl	8(%rax), %eax
	leaq	-128(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	tcsetattr@PLT
	cmpl	$-1, %eax
	jne	.L27
	leaq	.LC13(%rip), %rdi
	movl	$0, %eax
	call	ffly_printf@PLT
.L27:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	ffmain, .-ffmain
	.ident	"GCC: (Debian 8.1.0-12) 8.1.0"
	.section	.note.GNU-stack,"",@progbits
