# include "dus.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "../string.h"
# include "../malloc.h"
# include "../linux/mman.h"
# include "../linux/sched.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../linux/types.h"
# include "../linux/signal.h"
# include "../linux/wait.h"
# include "../system/errno.h"
# include "../env.h"
# include "mm.h"

static struct hash sy;
char const *opstr(ff_u8_t __op) {
	switch(__op) {
		case _op_copy:		return "copy";
		case _op_assign:	return "assign";
		case _op_push:		return "push";
		case _op_pop:		return "pop";
		case _op_fresh:		return "fresh";
		case _op_free:		return "free";
		case _op_out:		return "out";
	}
	return "unknown";
}

objp static objs[20];
objp static* top = objs;

void static
op_copy(objp __obj) {
	ff_u8_t *src = (*(objp*)__obj->src)->p;
	ff_u8_t *dst = (*(objp*)__obj->dst)->p;
	memcpy(dst, src, __obj->len);
}

void static
op_fresh(objp __obj) {
	__obj->p = ff_dus_mmap(__obj->size);
}

void static
op_free(objp __obj) {
	ff_dus_munmap(__obj->p);
}

void static
op_push(objp __obj) {
	*(top++) = __obj->p;
}

void static
op_pop(objp __obj) {
	__obj->p = *(--top);
}

void static
op_assign(objp __obj) {
	ff_u8_t *dst;
	dst = __obj->to->p;
	memcpy(dst, __obj->p, __obj->len);
}

void static
op_out(objp __obj) {
	objp src = *(objp*)__obj->p;
	char *p = (char*)src->p;
	printf("%s\n", p);
}

void static
op_cas(objp __obj) {
	objp dst = __obj->dst;
	objp src = *(objp*)__obj->p;
	dst->p = ff_dus_mmap(1048);
	char *p = src->p;
	ff_u8_t *dp = dst->p;
	char buf[128];
	char *bufp;
_again:
	if (*p == '$') {
		p++;
		bufp = buf;
		while(*p != ' ' && *p != '\0' && ((*p >= 'a' && *p <= 'z') || *p == '_'))
			*(bufp++) = *(p++);
		*bufp = '\0';
		objp o = (objp)hash_get(&sy, buf, bufp-buf);
		if (!o) {
			printf("error, %s\n", buf);
		} else {
			memcpy(dp, o->p, o->size);
			dp+=o->size-1;
		}
	} else {
		while(*p != '$' && *p != '\0')
			*(dp++) = *(p++);
	}

	if (*p != '\0') {
		goto _again;
	}

	*dp = '\0';
	dst->size = (dp-(ff_u8_t*)dst->p)+1;
}

void static
op_syput(objp __obj) {
	hash_put(&sy, __obj->name, __obj->len, __obj->p);
//	printf("put symbol, %s\n", __obj->name);
}
# include "../system/nanosleep.h"
void static
op_shell(objp __obj) {
	char *p = (*(objp*)__obj->p)->p;
//	printf("shell command: %s\n", p);
	char const *argv[100];
	char const **cur = argv;
	char *bed;
	ff_uint_t l;

	char base[128];
	char *bp = base;
	while(*p != ' ' && *p != '\0')
		*(bp++) = *(p++);
	*bp = '\0';
	
	while(*p == ' ') p++;
	*(cur++) = base;
//	printf("base: %s\n", base);
_again:
	bed = p;
	while(*p != ' ' && *p != '\0')
		p++;

	*cur = malloc((l = (p-bed))+1);
	memcpy(*cur, bed, l);
	*(char*)((*cur)+l) = '\0';
//	printf("--> %s\n", *cur);
	cur++;

	if (*p != '\0') {
		while(*p == ' ') p++;
		goto _again;
	}

	*cur = NULL;

	char const *envp[] = {
		"PATH=/usr/bin",
		NULL
	};

	__linux_pid_t pid;
	pid = fork();
	if (pid == 0) {
		int fd = open("stdout", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);
		if (execve(base, argv, envp) == -1) {
			// failure
		}
		exit(0);
	}
	wait4(pid, NULL, __WALL, NULL);

	cur = argv+1;
	while(*cur != NULL) {
		free(*cur);
		cur++;
	}
}

void static
op_set(objp __obj) {
	objp src = *(objp*)__obj->src;
	objp dst = *(objp*)__obj->dst;

	
	dst->p = ff_dus_mmap(dst->size = src->size);
	memcpy(dst->p, src->p, src->size);
}

static void(*op[])(objp) = {	
	op_copy,
	op_assign,
	op_push,
	op_pop,
	op_fresh,
	op_free,
	op_out,
	op_cas,
	op_syput,
	op_shell,
	op_set
};

void ff_dus_run(objp __top) {
	hash_init(&sy);
	objp cur = __top;
	while(cur != NULL) {
		op[cur->op](cur);
//		printf("op: %s\n", opstr(cur->op));
		cur = cur->next;
	}
	hash_destroy(&sy);
}
