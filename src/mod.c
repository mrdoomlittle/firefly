# include "mod.h"
# include "types.h"
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
# include "mod/port.h"
# include "system/port.h"
# include "mode.h"
# include "system/config.h"
# include "dep/str_cpy.h"
# include "system/mutex.h"
# include "system/errno.h"
# define DSS 262144
void ffly_mod() {
	if (!__ffly_sysconf__.root_dir ||
		!__ffly_sysconf__.moddir ||
		!__ffly_sysconf__.modl) return;

	if (!*__ffly_sysconf__.modl) return;

	char buf[1024];
	char *bufp = buf;
	bufp+=ffly_str_cpy(bufp, __ffly_sysconf__.root_dir);
	*(bufp++) = '/';
	bufp+=ffly_str_cpy(bufp, __ffly_sysconf__.moddir);
	*(bufp++) = '/';
	char const **mod = __ffly_sysconf__.modl;
	
	while(*mod != NULL) {		
		ffly_str_cpy(bufp, *mod);	
		ffly_printf("module path: %s\n", buf);
		ff_mod_load((char const*)buf);
		mod++;
	}
}

ff_i8_t static shut = -1, done = -1;
ff_mlock_t static lock = FFLY_MUTEX_INIT;
void ff_mod_init() {
	ff_mod_port_open();
}

void ff_mod_de_init() {
	shut = 0;
	while(done<0);
	ff_mod_port_close();
}

void static
execmod() {
	/*
		50/50 chance might get stuck
	*/
	ffly_nanosleep(2, 0); // replace this 
	char const *file;
	__asm__("movq -8(%%rbp), %%rdi\n\t"
		"movq %%rdi, %0": "=m"(file) : : "rdi");

	char buf[128];
	ffly_nots((ff_u64_t)ffmod_port_shmid(), buf);
	ffly_printf("portno: %s\n", buf);
	char *argv[] = {(char*)buf, NULL};
	ff_s32_t res = execve(file, (char*const)argv, NULL);
	if (res < 0) {
		ffly_printf("error, %s\n", strerror(errno));
	}

	exit(0);
}

void static
ffmod_printf(ff_u32_t __band) {
	__ffmod_debug
		ffly_printf("printf.\n");
	ffpap p, bk;
	ff_err_t err;
	ffly_port_recv(__band, (void*)&p, sizeof(void*), ffmod_portno());

	ffcall(_ffcal_printf, NULL, &p);

	while(p != NULL) {
		__ffly_mem_free(p->p);
		bk = p;
		p = p->next;
		__ffly_mem_free(bk);
	}
}

void static
ffmod_malloc(ff_u32_t __band) {
	__ffmod_debug
		ffly_printf("malloc.\n");
	ff_uint_t bc;
	ff_err_t err;
	void *ret;
	ffly_port_recv(__band, &bc, sizeof(ff_uint_t), ffmod_portno());
	__ffmod_debug
		ffly_printf("inbound, bc: %u\n", bc);

	ret = __ffly_mem_alloc(bc);
	ffly_port_send(__band, &ret, sizeof(void*), ffmod_portno());
	__ffmod_debug
		ffly_printf("outbound: %p\n", ret);
}

void static
ffmod_free(ff_u32_t __band) {
	__ffmod_debug
		ffly_printf("free.\n");
	ff_err_t err;
	void *p;
	ffly_port_recv(__band, (void*)&p, sizeof(void*), ffmod_portno());
	__ffmod_debug
		ffly_printf("inbound, p: %p\n", p);
	__ffly_mem_free(p);
}

void static
ffmod_dcp(ff_u32_t __band) {
	__ffmod_debug
		ffly_printf("dcp.\n");
	ff_err_t err;
	void *src;
	ff_uint_t n;

	ffly_port_recv(__band, (void*)&src, sizeof(void*), ffmod_portno());
	ffly_port_recv(__band, &n, sizeof(ff_uint_t), ffmod_portno());
	ffly_port_send(__band, src, n, ffmod_portno());
}

void static
ffmod_scp(ff_u32_t __band) {
	__ffmod_debug
		ffly_printf("scp.\n");
	ff_err_t err;
	void *dst;
	ff_uint_t n;

	ffly_port_recv(__band, (void*)&dst, sizeof(void*), ffmod_portno());
	ffly_port_recv(__band, &n, sizeof(ff_uint_t), ffmod_portno());
	ffly_port_recv(__band, dst, n, ffmod_portno());
}

static void(*process[])(ff_u32_t) = {
	ffmod_printf,
	NULL,
	ffmod_malloc,
	ffmod_free,
	NULL,
	ffmod_dcp,
	ffmod_scp
};

struct module {
	ff_u8_t *stack;
	ff_u32_t band;
	__linux_pid_t pid;
	ff_i8_t skip;
};


struct module *mods[20];
struct module **mod = mods;

void ff_mod_port_open() {
	ffmod_port();
	ffly_printf("shm: %lu\n", ffmod_port_shmid());
}

void ff_mod_port_close() {
	ffmod_port_close();	
}

# include "dep/str_cmp.h"
void ff_mod_load(char const *__file) {
	ffly_mutex_lock(&lock);
	struct module *m = (*(mod++) = (struct module*)__ffly_mem_alloc(sizeof(struct module)));
	ffly_printf("---> %p\n", m);
	*mod = NULL;
	m->skip = 0;
	m->stack = (ff_u8_t*)__ffly_mem_alloc(DSS);
	ffly_printf("file: %s\n", __file);

	*(void**)(m->stack+(DSS-8)) = (void*)execmod;
	*(void**)(m->stack+(DSS-16)) = (void*)__file;

	if ((m->pid = clone(CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS,
		(ff_u64_t)(m->stack+(DSS-8)), NULL, NULL, 0)) == (__linux_pid_t)-1)
	{
		ffly_printf("error.\n");
		goto _fail;
	}

	ffly_printf("module pid: %u\n", m->pid);
	ffly_mutex_unlock(&lock);

	ffly_port_recv(MOD_LISTEN, &m->band, sizeof(ff_u32_t), ffmod_portno());
	ffly_printf("module band: %u\n", m->band);
	ffly_nanosleep(1, 0);
	m->skip = -1;
	ffly_printf("done.\n");
	return;
_fail:
	__ffly_mem_free(m->stack);
	__ffly_mem_free(m);
}

void static*
mod_listen(void *__arg) {
	struct module **cur;
	struct module *m;
	ffly_printf("listening.\n");
_again:
	cur = mods;
	while(mod != mods) {
		ffly_mutex_lock(&lock);

		m = *cur;
		if (!m) {
			ffly_printf("error.\n");
			ffly_mutex_unlock(&lock);
			break;
		}
		ff_u8_t no;
		ff_err_t err;

		if (!m->skip) {
			goto _end;
		}
		if (ffly_port_band(ffmod_portno(), m->band) == -1)
			goto _end;

		ffly_printf("--> %p\n", m);
		ffly_printf("working on band: %u\n", m->band);

		ffly_port_recv(m->band, &no, 1, ffmod_portno());

		if (no != 0xff) {
			if (no <= _ffcal_mod_scp) {
				process[no](m->band);
			} else {
				ffly_printf("somthing broke.\n");
				if (kill(m->pid, SIGKILL) == -1)
					ffly_printf("could not kill.\n");
				__ffly_mem_free(m->stack);
				__ffly_mem_free(m);
				*cur = *(--mod);
			}
		}

		if (no == 0xff) {
			ffly_printf("got terminator, band: %u\n", m->band);
			ffly_printf("waiting for prossess to finish, %u\n", m->pid);
			if (wait4(m->pid, NULL, __WALL|__WCLONE, NULL) == -1) {
				ffly_printf("error, %s\n", strerror(errno));
			}

			__ffly_mem_free(m->stack);
			__ffly_mem_free(m);	
		
			if (cur != mod-1)
				*cur = *(--mod);
			else
				*(--mod) = NULL;
			ffly_printf("module was killed off.\n");
		}
	
	_end:
		if (cur >= mod-1)
			cur = mods;
		else
			cur++;
		ffly_mutex_unlock(&lock);
	}

	if (shut<0)
		goto _again;
	ffly_printf("shuting down.\n");
	done = 0;
}

# include "system/thread.h"
void ff_mod_handle() {
	ff_tid_t tid;
	ffly_thread_create(&tid, mod_listen, NULL);
}

void ffmodldl(char const **__file) {
	char const **file = __file;
	while(*file != NULL) {
		ff_mod_load(*file);
		file++;
	}
}
//#define DEBUG
# ifdef DEBUG

# include "system/realpath.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char *file;
	ffmodld(file = ffly_realpath("../modules/a.out"));
	__ffly_mem_free(file);
}
# endif
