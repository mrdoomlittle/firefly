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
	"u8_t",
	"u16_t",
	"u32_t",
	"u64_t",
	"out",
	"typedef",
	"exit",
	"goto",
	"if",
	"struct",
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

# include "opt.h"
# include "depart.h"
# include "dep/str_cpy.h"
# include "dep/str_cmp.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffoe_prep();
	char const *infile;
	char const *outfile;
	char const *gen;
	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;

	ffoe(ffoe_pcll, (void*)&pcl);
	infile = ffopt_getval(ffoe_get("i"));
	outfile = ffopt_getval(ffoe_get("o"));
	gen = ffopt_getval(ffoe_get("g"));
	ffoe_end();

	if (!infile || !outfile || !gen) {
		ffly_printf("missing -i, -g or -o.\n");
		reterr;
	}

	if ((fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		ffly_printf("failed to open file.\n");
		reterr;
	}
	struct ffly_compiler compiler;
	compiler.out = out;
	ffly_compiler_prepare(&compiler);
	ffc_ldlang(&compiler, _ffc_ff);
	ldkeywds(&compiler);	
	ffly_compiler_ld(&compiler, infile);
	if (!ffly_str_cmp(gen, "resin"))
		ffly_ff_resin();
	else if (!ffly_str_cmp(gen, "amd64"))
		ffly_ff_amd64();
	else {
		// error
	}

	ffc_build(&compiler);
	ffly_compiler_free(&compiler);
	close(fd);
/*
	if (access("as/as", F_OK) == -1) {
		ffly_printf("assembler doesen't exist.\n");
		goto _end;
	}
	__linux_pid_t pid;
	if ((pid = fork()) == 0) {
		char *argv[] = {"as", "-f", "ffef", "-i", "s.out", "-o", ".o", NULL};
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
		char *argv[] = {"bond", "-s", ".o", "-d", "a.out", NULL};
		char *envp[] = {NULL};
		execve("bond/bond", argv, envp);
		exit(0);
	}

	ffly_printf("waiting for linker.\n");
	wait4(pid, NULL, __WALL, NULL);
_end:*/
	ffly_depart(NULL);
	reterr;
}
