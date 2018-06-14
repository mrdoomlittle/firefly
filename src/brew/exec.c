# include "brew.h"
# include "../stdio.h"
# include "../ffly_def.h"
# include "../linux/mman.h"
# include "../linux/sched.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../linux/types.h"
# include "../linux/signal.h"
# include "../linux/wait.h"
# include "../string.h"
# include "../system/errno.h"
# include "../env.h"
# include "../malloc.h"
// return buffer
# define RTBUF_SIZE 20

objp rtbuf[RTBUF_SIZE];
objp static *retto = rtbuf;
void static
op_cp(objp *__p) {

}

void static
op_jump(objp *__p) {
	if (retto-rtbuf >= RTBUF_SIZE) {
		printf("rtbuf overflow.\n");
		return;
	}
	*(retto++) = (*__p)->next;
	*__p = (*__p)->to;
}

void static
op_echo(objp *__p) {
	objp p = *__p;

	struct frag *f = (struct frag*)p->p;

	while(f != NULL) {
		printf("%s", (char*)f->p);
		f = f->next;
	}
}

void static
op_end(objp *__p) {
	objp p = *__p;
	*__p = *(--retto);
}

void static
op_shell(objp *__p) {
	objp p = *__p;
	struct shell *s = (struct shell*)p->p;
	printf("base: %s\n", s->base);
	char **arg = s->args;
	char *argv[100];
	*argv = s->base;
	char **cur = argv+1;
	char *bed;
	ff_uint_t l;
	while(*arg != NULL) {
		printf("arg: %s\n", *arg);
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

	printf("command: %s ", s->base);
	cur = argv+1;
	while(*cur != NULL) {
		printf("%s | ", *cur);
		cur++;
	}
	printf("\n");

	__linux_pid_t pid;

	char const *envp[] = {
		"PATH=/usr/bin",
		NULL
	};
	
	pid = fork();
	if (pid == 0) {
		if (execve(s->base, argv, envp) == -1) {
			// error
		}
		exit(0);
	}
	wait4(pid, NULL, __WALL, NULL);

	cur = argv+1;
	while(*cur != NULL) {
		free(*(cur++));
	}
}

static void(*op[])(objp*) = {
	op_cp,
	NULL,
	op_jump,
	op_echo,
	op_end,
	op_shell
};

void brew_exec(objp __top) {
	printf("exec.\n");
	objp cur = __top, bk;
	while(cur != NULL) {
		if (cur->op == _op_exit) {
			printf("goodbye.\n");
			break;
		}
		bk = cur;
		op[cur->op](&cur);
		if (bk != cur)
			continue;
		cur = cur->next;
	}
}
