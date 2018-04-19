# define __ffly_ff
# include "compiler.h"
# include "ffint.h"
# include "types.h"
# include "system/err.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
# include "linux/types.h"
# include "linux/wait.h"
# include "linux/sched.h"
char const static *keywords[] = {
	"as",
	"void",
	"ret",
	NULL
};

void static
ldkeywds(ffly_compilerp __compiler) {
	char const **keywd = keywords;
	while(*keywd != NULL) {
		ffly_compiler_ldkeywd(__compiler, ffly_compiler_kwno(*keywd));
		ffly_printf("loaded keyword {'%s'}\n", *keywd);
		keywd++;
	}
}

int static fd;
void static
out(void *__p, ff_uint_t __n) {
	write(fd, __p, __n);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	fd = open("main.out", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	struct ffly_compiler compiler;
	compiler.out = out;
	ffly_compiler_prepare(&compiler);
	ffc_ldlang(&compiler, _ffc_ff);
	ldkeywds(&compiler);	
	ffly_compiler_ld(&compiler, "ff/main.ff");

	ffc_build(&compiler);
	ffly_compiler_free(&compiler);
	close(fd);

	if (access("as/as", F_OK) == -1) {
		ffly_printf("assembler doesen't exist.\n");
		goto _end;
	}
	__linux_pid_t pid;
	if ((pid = fork()) == 0) {
		char *argv[] = {"as", "-f", "ffef", "-i", "main.out", "-o", "main.o", NULL};
		char *envp[] = {NULL};
		execve("as/as", argv, envp);
		exit(0);
	}

	ffly_printf("waiting for assembler.\n");
	wait4(pid, NULL, __WALL, NULL);	

	if (access("bond/bond", F_OK) == -1) {
		ffly_printf("linker doesen't exist.\n");
		goto _end;
	}

	if ((pid = fork()) == 0) {
		char *argv[] = {"bond", "-s", "main.o", "-d", "a.out", NULL};
		char *envp[] = {NULL};
		execve("bond/bond", argv, envp);
		exit(0);
	}

	ffly_printf("waiting for linker.\n");
	wait4(pid, NULL, __WALL, NULL);
_end:
	reterr;
}
