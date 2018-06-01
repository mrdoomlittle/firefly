# include "connect.h"
# include "../ffly_def.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../system/util/hash.h"
# include "../dep/str_dup.h"
# include "../system/io.h"
# include "../dep/str_cmp.h"
# include "../system/string.h"
# include "../dep/str_len.h"
# include "../system/map.h"
typedef struct dct {
	char const *ident;
	char const *params[20];
} *dctp;

ff_uint_t
read_part(char *__buf, char **__p) {
	char *p = *__p;
	char *bufp = __buf;
	while(*p == ' ' && *p != '\0') p++;
	if (*p == '\0') {
		*__buf = '\0';
		return 0;
	}
	ff_i8_t s;
	if (*p == '"') {
		s = 0;
		p++;
	} else
		s = -1;
	while((*p != ' ' && *p != '\0' && s<0) || (!s && *p != '"'))
		*(bufp++) = *(p++);
	if (!s)
		p++;
	ff_uint_t l = bufp-__buf;
	*(__buf+l) = '\0';
	*__p = p;
	return l;
}

void dct_free(dctp __dct) {
	__ffly_mem_free(__dct->ident);
	char const **cur = __dct->params;
	while(*cur != NULL)
		__ffly_mem_free(*(cur++));
	__ffly_mem_free(__dct);
}

dctp back = NULL;
dctp read_dct(char *__line) {
	char buf[128];
	char *p = __line;
	dctp ret = (dctp)__ffly_mem_alloc(sizeof(struct dct));
	read_part(buf, &p);
	ret->ident = ffly_str_dup(buf);

	char const **param = ret->params;
	while(*p != '\0') {
		read_part(buf, &p);
		*(param++) = ffly_str_dup(buf);
	}
	*param = NULL;

	if (back != NULL)
		dct_free(back);
	back = ret;
	return ret;
}

struct cmd {
	char const *ident;
	void(*p)();
};

void _shutdown();
void _connect();
void _disconnect();
void _login();
void _logout();
void _creat_pile();
void _del_pile();
void _creat_record();
void _del_record();
void _write();
void _read();
void _record_alloc();
void _record_free();
void _rivet();
void _derivet();
void _rivetto();
void _bind();
void _acquire_slot();
void _scrap_slot();
void _ls_piles();
void _ls_records();
void _exist();
void _bufal();
void _buffr();
void _bufld();
void _showbuf();
void _help();
void _exit();

struct cmd *cmdl[] = {
	&(struct cmd){"shutdown", _shutdown},
	&(struct cmd){"connect", _connect},
	&(struct cmd){"disconnect", _disconnect},
	&(struct cmd){"login", _login},
	&(struct cmd){"logout", _logout},
	&(struct cmd){"creatpile", _creat_pile},
	&(struct cmd){"delpile", _del_pile},
	&(struct cmd){"creatrecord", _creat_record},
	&(struct cmd){"delrecord", _del_record},
	&(struct cmd){"write", _write},
	&(struct cmd){"read", _read},
	&(struct cmd){"recordalloc", _record_alloc},
	&(struct cmd){"recordfree", _record_free},
	&(struct cmd){"rivet", _rivet},
	&(struct cmd){"derivet", _derivet},
	&(struct cmd){"rivetto", _rivetto},
	&(struct cmd){"bind", _bind},
	&(struct cmd){"acquireslot", _acquire_slot},
	&(struct cmd){"scrapslot", _scrap_slot},
	&(struct cmd){"lspiles", _ls_piles},
	&(struct cmd){"lsrecords", _ls_records},
	&(struct cmd){"exist", _exist},
	&(struct cmd){"bufal", _bufal},
	&(struct cmd){"buffr", _buffr},
	&(struct cmd){"bufld", _bufld},
	&(struct cmd){"showbuf", _showbuf},
	&(struct cmd){"help", _help},
	&(struct cmd){"exit", _exit},
	NULL
};

void lddct(ffly_mapp __map) {
	struct cmd **cur = cmdl;
	while(*cur != NULL) {
		struct cmd *p = *cur;
		ffly_map_put(__map, p->ident, ffly_str_len(p->ident), p->p);
		cur++;
	}
}

char const *help = "commands:\n"
	"	shutdown\n"
	"	connect\n"
	"	disconnect\n"
	"	login\n"
	"	logout\n"
	"	creatpile\n"
	"	delpile\n"
	"	creatrecord\n"
	"	delrecord\n"
	"	write\n"
	"	read\n"
	"	recordalloc\n"
	"	recordfree\n"
	"	rivet\n"
	"	derivet\n"
	"	rivetto\n"
	"	bind\n"
	"	acquireslot\n"
	"	scrapslot\n"
	"	exist\n"
	"	bufal\n"
	"	buffr\n"
	"	bufld\n"
	"	help\n"
	"	exit";

# define jmpagain __asm__("jmp _again")
# include "../dep/str_cpy.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	void *buf = NULL;
	char ln[200];
	ff_db_ctrp ctor;
	ff_i8_t conn = -1;
	ff_i8_t loggedin = -1;
	ff_err_t err;

	FF_MAP map;
	ffly_map_init(&map, _ffly_map_127);
	lddct(&map);
	
	dctp cur;
	void *to;
	__asm__("_again:\n\t");
	ffly_printf("~: ");
	ffly_fdrain(ffly_out);
	ffly_rdline(ln, 200, ffly_in);
	cur = read_dct(ln);

	to = ffly_map_get(&map, cur->ident, ffly_str_len(cur->ident), &err);
	if (!to || _err(err)) {
		ffly_printf("command does not exist.\n");
		jmpagain;
	}

	__asm__("jmp *%0" : : "r"(to));

	__asm__("_shutdown:\n\t"); {
		ff_db_ctr_shutdown(ctor);
	}
	jmpagain;

	__asm__("_connect:\n\t"); {
		char const *ip_adr = *cur->params;
		char const *port = cur->params[1];
		char const *enckey = cur->params[2];
		ffly_printf("port: %s, address: %s\n", port, ip_adr);
		ctor = ff_db_ctr(ffly_hash(enckey, ffly_str_len(enckey)), ip_adr, ffly_stno(port), &err);
		if (_err(err)) {
			ffly_printf("failed to connect.\n");
		} else
			conn = 0;
	}
	jmpagain;

	__asm__("_disconnect:\n\t"); {
		if (!conn) {
			ff_db_ctr_disconnect(ctor);
			ff_db_ctr_destroy(ctor);
		} else
			ffly_printf("your not connected.\n");
	}
	jmpagain;

	__asm__("_login:\n\t"); {
		if (loggedin == -1) {
			char const *id = *cur->params;
			char const *passwd = cur->params[1];
			err = ff_db_ctr_login(ctor, id, ffly_hash(passwd, ffly_str_len(passwd)));
			if (_ok(err))
				loggedin = 0;
			else
				ffly_printf("failure to login.\n");
		} else
			ffly_printf("already loggedin.\n");
	}
	jmpagain;

	__asm__("_logout:\n\t"); {
		if (!loggedin) {
			ff_db_ctr_logout(ctor);
			loggedin = -1;
		} else
			ffly_printf("not loggedin.\n");
	}
	jmpagain;

	__asm__("_creat_pile:\n\t"); {
		ff_uint_t slotno;
		ff_db_ctr_creat_pile(ctor, &slotno);
		ffly_printf("pile-slotno: %u\n", slotno);
	}
	jmpagain;

	__asm__("_del_pile:\n\t"); {
		ff_uint_t slotno = ffly_stno(*cur->params);
		ff_db_ctr_del_pile(ctor, slotno);
	}
	jmpagain;

	__asm__("_creat_record:\n\t"); {
		ff_uint_t pile = ffly_stno(*cur->params);
		ff_uint_t size = ffly_stno(cur->params[1]);
		ff_uint_t slotno;
		ff_db_ctr_creat_record(ctor, pile, &slotno, size);
		ffly_printf("record-slotno: %u\n", slotno);
	}
	jmpagain;

	__asm__("_del_record:\n\t"); {
		ff_uint_t pile = ffly_stno(*cur->params);
		ff_uint_t slotno = ffly_stno(cur->params[1]);
		ff_db_ctr_del_record(ctor, pile, slotno);
	}
	jmpagain;

	__asm__("_write:\n\t"); {
		ff_uint_t pile = ffly_stno(*cur->params);
		ff_uint_t rec = ffly_stno(cur->params[1]);
		ff_uint_t offset = ffly_stno(cur->params[2]);
		ff_uint_t size = ffly_stno(cur->params[3]);
		ff_db_ctr_write(ctor, pile, rec, offset, buf, size);
	}
	jmpagain;

	__asm__("_read:\n\t"); {
		ff_uint_t pile = ffly_stno(*cur->params);
		ff_uint_t rec = ffly_stno(cur->params[1]);
		ff_uint_t offset = ffly_stno(cur->params[2]);
		ff_uint_t size = ffly_stno(cur->params[3]);
		ff_db_ctr_read(ctor, pile, rec, offset, buf, size);
	}
	jmpagain;

	__asm__("_record_alloc:\n\t"); {
		ff_uint_t slotno = ffly_stno(*cur->params);
		ff_db_ctr_record_alloc(ctor, slotno);	
	}
	jmpagain;

	__asm__("_record_free:\n\t"); {
		ff_uint_t slotno = ffly_stno(*cur->params);
		ff_db_ctr_record_free(ctor, slotno);
	}
	jmpagain;

	__asm__("_rivet:\n\t"); {
		ff_u16_t rivetno = ffly_stno(*cur->params);
		ff_uint_t slotno = ffly_stno(cur->params[1]);
		ff_db_ctr_rivet(ctor, rivetno, slotno);
	}
	jmpagain;

	__asm__("_derivet:\n\t"); {
		ff_u16_t rivetno = ffly_stno(cur->params[1]);
		ff_db_ctr_derivet(ctor, rivetno);
	}
	jmpagain;

	__asm__("_rivetto:\n\t"); {
		ff_u16_t rivetno = ffly_stno(*cur->params);
		ff_uint_t slotno = ffly_stno(cur->params[1]);
		ff_db_ctr_rivetto(ctor, rivetno, slotno);
	}
	jmpagain;

	__asm__("_bind:\n\t"); {
		ff_u16_t rivetno = ffly_stno(*cur->params);
		ff_uint_t slotno = ffly_stno(cur->params[1]);
		char const *type = cur->params[2];
		ff_int_t offset = -1;
		if (!ffly_str_cmp(type, "record"))
			offset = offsetof(struct ffdb_record, no);
		else if (!ffly_str_cmp(type, "pile"))
			offset = offsetof(struct ffdb_pile, no);
		if (offset>0)
			ff_db_ctr_bind(ctor, rivetno, slotno, offset);
	}
	jmpagain;

	__asm__("_acquire_slot:\n\t"); {
		ff_uint_t slotno;
		ff_db_ctr_acquire_slot(ctor, &slotno);
		ffly_printf("slotno: %u\n", slotno);
	}
	jmpagain;

	__asm__("_scrap_slot:\n\t"); {
		ff_db_ctr_scrap_slot(ctor, ffly_stno(*cur->params));
	}
	jmpagain;

	__asm__("_ls_piles:\n\t"); {
		
	}
	jmpagain;

	__asm__("_ls_records:\n\t"); {
		
	}
	jmpagain;

	__asm__("_bufal:\n\t"); {
		buf = __ffly_mem_alloc(ffly_stno(*cur->params));
	}
	jmpagain;

	__asm__("_buffr:\n\t"); {
		__ffly_mem_free(buf);
		buf = NULL;
	}
	jmpagain;

	__asm__("_bufld:\n\t"); {
		char const *file = *cur->params;
		int fd;
		if ((fd = open(file, O_RDONLY, 0))>=0) {
			struct stat st;
			fstat(fd, &st);
			read(fd, buf, st.st_size);
			close(fd);
		} else
			ffly_printf("failed to open file\n");
	}
	jmpagain;

	__asm__("_showbuf:\n\t"); {
		ffly_printf("%s\n", buf);
	}
	jmpagain;

	__asm__("_exist:\n\t"); {
		ff_u16_t rivetno = ffly_stno(*cur->params);
		ffly_printf("%s\n", ff_db_ctr_exist(ctor, rivetno, &err) == -1?"doesen't exist.":"does exist.");
	}
	jmpagain;

	__asm__("_help:\n\t"); {
		ffly_printf("%s\n", help);
	}
	jmpagain;

	__asm__("_exit:\n\t");
	ffly_map_de_init(&map);
	dct_free(back);

	if (buf != NULL)
		__ffly_mem_free(buf);
}
