# include "types/err_t.h"
# include "call.h"
# include "system/io.h"
# include "linux/mman.h"
# include "linux/sched.h"
# include "linux/unistd.h"
# include "linux/stat.h"
# include "linux/types.h"
# include "linux/signal.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/nanosleep.h"
# include "linux/wait.h"
# include "system/errno.h"
# include "system/string.h"
# include "mod/pipe.h"
# include "system/pipe.h"
# define DSS 100000 // i dont know how big the stack is all i know is its quite large
void static
execmod() {
	/*
		50/50 chance might get stuck
	*/
	ffly_nanosleep(1, 0); // replace this 
	char const *file;
	__asm__("movq -8(%%rbp), %%rdi\n\t"
		"movq %%rdi, %0": "=m"(file) : : "rdi");

	char buf[128];
	ffly_nots((mdl_u64_t)ffmod_pipe_shmid(), buf);
	ffly_printf("pipeno: %s\n", buf);
	char *argv[] = {buf, NULL};
	mdl_s32_t res = execve(file, argv, NULL);
	if (res < 0) {
		ffly_printf("error, %s\n", strerror(errno));
	}

	exit(0);
}

void ffmodld(char const *__file) {
	ffmod_pipe();
	ffly_printf("shm: %lu\n", ffmod_pipe_shmid());
	mdl_u8_t *stack = (mdl_u8_t*)__ffly_mem_alloc(DSS);
	ffly_printf("file: %s\n", __file);

	*(void**)(stack+(DSS-8)) = (void*)execmod;
	*(void**)(stack+(DSS-16)) = (void*)__file;

	__linux_pid_t pid;
	if ((pid = clone(CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS|SIGCHLD,
		(mdl_u64_t)(stack+(DSS-8)), NULL, NULL, 0)) == (__linux_pid_t)-1)
	{
		ffly_printf("error.\n");
	}

	ffly_pipe_listen(ffmod_pipeno());
	mdl_u8_t no;
	_again:
	if ((no = ffly_pipe_rd8l(ffmod_pipeno())) != 0xff) {
		switch(no) {
			case _ffcal_malloc: {
				ffly_printf("malloc.\n");
				mdl_uint_t bc;
				void *ret;
				ffly_pipe_read(&bc, sizeof(mdl_uint_t), ffmod_pipeno());
				ffly_printf("got, bc: %u\n", bc);
				ret = __ffly_mem_alloc(bc);
				ffly_pipe_wr64l((mdl_u64_t)ret, ffmod_pipeno());

				ffly_printf("sent: %p\n", ret);
				break;
			}

			case _ffcal_free: {
				ffly_printf("free.\n");
				void *p;
				p = (void*)ffly_pipe_rd64l(ffmod_pipeno());
				ffly_printf("got, p: %p\n", p);
				break;
			}
		}

		ffly_printf("...\n");
		goto _again;
	}

	if (no == 0xff) {
		ffly_printf("got end.\n");
	}
	
	wait4(pid, NULL, __WALL|__WCLONE, NULL);
	__ffly_mem_free(stack);	
	ffmod_pipe_close();
}

# include "system/realpath.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	char *file;
	ffmodld(file = ffly_realpath("../modules/a.out"));
	__ffly_mem_free(file);
}
