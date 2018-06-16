# include "mould.h"
# include "../stdio.h"
# include "../ffly_def.h"
# include "../string.h"
# include "../system/errno.h"
# include "../env.h"
# include "../malloc.h"
# include "job.h"
// return buffer
# define RTBUF_SIZE 20

objp rtbuf[RTBUF_SIZE];
objp static *retto = rtbuf;
static objp _o;
static objp *_op;

void static
op_cp(void) {

}

void static
op_jump(void) {
	if (retto-rtbuf >= RTBUF_SIZE) {
		printf("rtbuf overflow.\n");
		return;
	}
	*(retto++) = _o->next;
	*_op = _o->to;
}

void static
op_echo(void) { 
	struct frag *f = (struct frag*)_o->p;

	while(f != NULL) {
		printf("%s", (char*)f->p);
		f = f->next;
	}
}

void static
op_end(void) {
	*_op = *(--retto);
}

void static
op_shell(void) {
	struct shell *s = (struct shell*)_o->p;
//	printf("base: %s\n", s->base);
	char **arg = s->args;
	char *argv[100];
	*argv = s->base;
	char **cur = argv+1;
	char *bed;
	ff_uint_t l;
	while(*arg != NULL) {
//		printf("arg: %s\n", *arg);
		char *p = *(arg++);
_again:
		bed = p;
		while(*p != ' ' && *p != '\0')
			p++;

		*cur = malloc((l = (p-bed))+1);
		memcpy(*cur, bed, l);
		*(char*)((*cur)+l) = '\0';
		cur++;
		if (*p != '\0') {
			p++;
			goto _again;
		}
	}
	*cur = NULL;
/*
	printf("command: %s ", s->base);
	cur = argv+1;
	while(*cur != NULL) {
		printf("%s | ", *cur);
		cur++;
	}
	printf("\n");
*/
	char const *envp[] = {
		"PATH=/usr/bin",
		NULL
	};

	mould_job(s->base, argv, envp);

	cur = argv+1;
	while(*cur != NULL) {
		free(*(cur++));
	}
}

void static
op_jobs_wait(void) {
	jobs_wait();	
}

static void(*op[])(void) = {
	op_cp,
	NULL,
	op_jump,
	op_echo,
	op_end,
	op_shell,
	op_jobs_wait
};

void mould_exec(objp __top) {
	printf("exec.\n");
	objp cur = __top, bk;
	_op = &cur;
	while(cur != NULL) {
		if (cur->op == _op_exit) {
			printf("goodbye.\n");
			break;
		}
		bk = cur;
		_o = cur;
		op[cur->op]();
		if (bk != cur)
			continue;
		cur = cur->next;
	}
}
