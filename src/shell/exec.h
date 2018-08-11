# ifndef __ffly__shell__exec__h
# define __ffly__shell__exec__h
# include "../ffint.h"
# include "hash.h"

ff_i8_t extern* ffsh_run;
extern void(*ffsh_er)(char const*, ff_uint_t, char const*[]);
struct arg_s {
	char const *p;
	ff_uint_t l;
};

enum {
	_cmd_help,
	_cmd_exit,
	_cmd_ff5,
	_cmd_ff6,
	_cmd_info,
	_cmd_exec,
	_cmd_er
};

void ffsh_cmdput(struct hash*, char const*, ff_uint_t);
void* ffsh_cmdget(struct hash*, char const*);
void ffsh_exec_cmd(void*, ff_uint_t, struct arg_s**);
# endif /*__ffly__shell__exec__h*/
