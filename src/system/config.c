# include "config.h"
# include "../bole.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../mode.h"
# include "../dep/mem_set.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
#define DEF_MAX_THREADS 20
#define ALLOCA_SSIZE 400
#define BINFILE "syscf.bin"

/*
	TODO cleanup
*/
ff_u8_t ffly_sysconf[2048];
ff_u64_t static scdoff = 0;

ff_u8_t static ldcode[512];
ff_u8_t static *lc = ldcode;

void static* alloc(ff_uint_t __size) {
	ff_u32_t of;
	of = scdoff;
	scdoff+=__size;
	return (ffly_sysconf+of+SCF_SZ);
}

char const static* strdup(char const *__str) {
	char const *s;
	ff_uint_t size;
	s = (char const*)alloc(size = (ffly_str_len(__str)+1));
	ffly_mem_cpy(s, __str, size);
	return s;
}

#define _MAX_THREADS		0x0000000000000001
#define _ALSSIZE			0x0000000000000002
static ff_u64_t flags = 0;
#define isset(__what) \
	((flags&(__what)) == (__what))
#define isnset(__what) \
	!isset(__what)
void
ff_sysconf_init(void) {
	ffly_mem_set(ffly_sysconf, 0, 256);
	*sysconf_get(loaded) = -1;
	*sysconf_get(db_loaded) = -1;
}

void static
ld_def_max_threads(void) {
	*sysconf_get(max_threads) = DEF_MAX_THREADS;
}

void static
ld_def_alssize(void) {
	*sysconf_get(alssize) = ALLOCA_SSIZE;
}

void ffly_ld_sysconf_def(void) {
	ld_def_max_threads();
	ld_def_alssize();
}

void static
ld_max_threads(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "max_threads")) != NULL) {
		*sysconf_get(max_threads) = ffly_bole_int_u(p);
		flags |= _MAX_THREADS;
	}
	ffly_fprintf(ffly_log, "max threads: %u\n", *sysconf_get(max_threads));
}

void static
ld_alssize(ffbole *__cf) {
	void const *p;
	if ((p = ffly_bole_get(__cf, "alssize")) != NULL) {
		*sysconf_get(alssize) = ffly_bole_int_u(p);
		flags |= _ALSSIZE;
	}
	ffly_fprintf(ffly_log, "alloca ssize: %u\n", *sysconf_get(alssize));
}

void static
ld_moddir(ffbole *__cf) {
	void const *p;
	void *p0;
	if ((p = ffly_bole_get(__cf, "moddir")) != NULL) {
		p0 = *sysconf_get(moddir) = strdup(ffly_bole_str(p));
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_moddir;
		lc+=5;
	}
}

void static
ld_inidir(ffbole *__cf) {
	void const *p;
	void *p0;
	if ((p = ffly_bole_get(__cf, "inidir")) != NULL) {
		p0 = *sysconf_get(inidir) = strdup(ffly_bole_str(p));
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_inidir;
		lc+=5;
	}
}

void static
ld_modl(ffbole *__cf) {
	void const *p;
	void *p0, *p1;
	if ((p = ffly_bole_get(__cf, "modl")) != NULL) {
		ffly_printf("module list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i, l = ffly_bole_arr_len(p);
		p0 = *sysconf_get(modl) = (char const**)alloc((l+1)*sizeof(char const*));
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_modl;	
		lc+=5;

		i = 0;
		while(i != l) {
			p1 = (*sysconf_get(modl))[i] = strdup(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("module: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			*lc = SCF_LC_PP;
			*(ff_u16_t*)(lc+1) = offof(p1);
			*(ff_u16_t*)(lc+3) = offof(p0+(i*sizeof(void*)));
			lc+=5;
			i++;
		}
		(*sysconf_get(modl))[i] = NULL;
	}
}

void static
ld_inil(ffbole *__cf) {
	void const *p;
	void *p0, *p1;
	if ((p = ffly_bole_get(__cf, "inil")) != NULL) {
		ffly_printf("init list len: %u\n", ffly_bole_arr_len(p));
		ff_u8_t i, l = ffly_bole_arr_len(p);
		if (!(p0 = *sysconf_get(inil) = (char const**)alloc((l+1)*sizeof(char const*)))) {
			ffly_printf("error.\n");
			return;
		}

		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_inil;
		lc+=5;

		i = 0;
		while(i != l) {
			p1 = (*sysconf_get(inil))[i] = strdup(ffly_bole_str(ffly_bole_arr_elem(p, i)));
			ffly_printf("init: %s\n", ffly_bole_str(ffly_bole_arr_elem(p, i)));
			*lc = SCF_LC_PP;
			*(ff_u16_t*)(lc+1) = offof(p1);
			*(ff_u16_t*)(lc+3) = offof(p0+(i*sizeof(void*)));
			lc+=5;
			i++;
		}
		(*sysconf_get(inil))[i] = NULL;
	}
}

void static
ld_db(void const *__db) {
	void const *ip_addr;
	void const *port;
	void const *enckey;
	void const *user;
	void const *passwd;

	void *p0;
	ffly_printf("*db-composition:\n");
	if ((ip_addr = ffly_bole_struc_get(__db, "ip_addr")) != NULL) {
		p0 = *sysconf_get(db_ip_addr) = strdup(ffly_bole_str(ip_addr));
		ffly_printf("\t-ip addr: %s\n", *sysconf_get(db_ip_addr));
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_db_ip_addr;
		lc+=5;
	}

	if ((port = ffly_bole_struc_get(__db, "port")) != NULL) {
		*sysconf_get(db_port) = ffly_bole_16l_u(port);
		ffly_printf("\t-port: %u\n", *sysconf_get(db_port));
	}

	if ((enckey = ffly_bole_struc_get(__db, "enckey")) != NULL) {
		p0 = *sysconf_get(db_enckey) = strdup(ffly_bole_str(enckey));
		ffly_printf("\t-enckey: %s\n", *sysconf_get(db_enckey));
		*lc = SCF_LC_PP;
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_db_enckey;
		lc+=5;
	}

	if ((user = ffly_bole_struc_get(__db, "user")) != NULL) {
		p0 = *sysconf_get(db_user) = strdup(ffly_bole_str(user));
		ffly_printf("\t-username: %s\n", *sysconf_get(db_user));
		*lc = SCF_LC_PP;
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_db_user;
		lc+=5;
	}

	if ((passwd = ffly_bole_struc_get(__db, "passwd")) != NULL) {
		p0 = *sysconf_get(db_passwd) = strdup(ffly_bole_str(passwd));
		ffly_printf("\t-password: %s\n", *sysconf_get(db_passwd));
		*lc = SCF_LC_PP;
		*lc = SCF_LC_PP;
		*(ff_u16_t*)(lc+1) = offof(p0);
		*(ff_u16_t*)(lc+3) = SCF_db_passwd;
		lc+=5;
	}
	*sysconf_get(db_loaded) = 0;
}

# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../linux/stat.h"
# include "../oddity.h"
void static exec(ff_uint_t __n) {
	ff_u8_t *p, *e;
	p = ldcode;
	e = p+__n;

	ff_u8_t on;
	while(p != e) {
		on = *(p++);
		switch(on) {
			case 0x00:
				goto _end;
			case 0x01: {
				ffly_printf("op_pp.\n");
				ff_u16_t s, d;
				s = *(ff_u16_t*)p;
				d = *(ff_u16_t*)(p+2);
				*(void**)(ffly_sysconf+d) = ffly_sysconf+s;
				p+=4;
				break;
			}
			default:
				caught_oddity;

		}
	}
_end:
	return;
}

struct binhdr {
	ff_u32_t data;
	ff_u32_t code;
	ff_uint_t cs;
};

void static
save_bin(void) {
	int fd;
	fd = open(BINFILE, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	struct binhdr h;
	h.data = sizeof(struct binhdr);
	h.code = h.data+sizeof(ffly_sysconf);
	h.cs = lc-ldcode;
	ffly_printf("data: %u, code: %u, codelen: %u\n", h.data, h.code, h.cs);
	lseek(fd, h.data, SEEK_SET);
	write(fd, ffly_sysconf, sizeof(ffly_sysconf));
	write(fd, ldcode, h.cs);
	pwrite(fd, &h, sizeof(struct binhdr), 0);
	close(fd);
}

void static
load_bin(void) {
	int fd;
	fd = open(BINFILE, O_RDONLY, 0);
	struct binhdr h;
	read(fd, &h, sizeof(struct binhdr));
	ffly_printf("data: %u, code: %u, codelen: %u\n", h.data, h.code, h.cs);
	pread(fd, ffly_sysconf, sizeof(ffly_sysconf), h.data);
	pread(fd, ldcode, h.cs, h.code);
	exec(h.cs);
	close(fd);
}

# include "../linux/unistd.h"
ff_err_t ffly_ld_sysconf(char const *__path) {
	if (!access("syscf.bin", F_OK)) {
		load_bin();
		return;
	}

	struct ffly_bole conf;
	ff_err_t err = FFLY_SUCCESS;
	if (_err(err = ffly_bole_init(&conf))) {
		ffly_fprintf(ffly_err, "failed to init config.\n");
		return err;
	}

	if (_err(err = ffly_bole_ld(&conf, __path))) {
		ffly_fprintf(ffly_err, "failed to load config.\n");
		return err;
	}
	
	if (_err(err = ffly_bole_read(&conf))) {
		ffly_fprintf(ffly_err, "failed to read config.\n");
		return err;
	}

	ffbole cf;
	// deposit
	ffly_bole_depos(&conf, &cf);

	void const *version;
	
	version = ffly_bole_get(&cf, "version");
	if (!ffly_bole_is_str(version)) {
		ffly_fprintf(ffly_err, "can't read version as type does not match.\n");
		err = FFLY_FAILURE;
		goto _fail;
	}

	void const *root_dir;
	
	root_dir = ffly_bole_get(&cf, "root_dir");
	if (!ffly_bole_is_str(root_dir)) {

	}

	void const *db;
	if ((db = ffly_bole_get(&cf, "db")) != NULL)
		ld_db(db);

	ld_max_threads(&cf);
	ld_alssize(&cf);
	ld_moddir(&cf);
	ld_inidir(&cf);
	ld_modl(&cf);
	ld_inil(&cf);

	void *p0;

	p0 = *sysconf_get(version) = strdup(ffly_bole_str(version));
	*lc = SCF_LC_PP;
	*(ff_u16_t*)(lc+1) = offof(p0);
	*(ff_u16_t*)(lc+3) = SCF_version;
	lc+=5;


	p0 = *sysconf_get(root_dir) = strdup(ffly_bole_str(root_dir));
	*lc = SCF_LC_PP;
	*(ff_u16_t*)(lc+1) = offof(p0);
	*(ff_u16_t*)(lc+3) = SCF_root_dir;
	lc+=5;

	p0 = *sysconf_get(loaded) = 0;

	if (isnset(_MAX_THREADS)) {
		ffly_printf("'max threads' not specified in config, loading default.\n");
		ld_def_max_threads();
	}

	if (isnset(_ALSSIZE)) {
		ffly_printf("'alloca stack size' not specified in config, loading default.\n");
		ld_def_alssize();
	}
_fail:
	if (_err(err = ffly_bole_free(&conf))) {
		ffly_fprintf(ffly_err, "failed to free config:0.\n");
		return err;
	}

	if (_err(err = ffbole_free(&cf))) {
		ffly_fprintf(ffly_err, "failed to free config:1.\n");
		return err;
	}

	save_bin();
	return FFLY_SUCCESS;
}

# include "../macros.h"
void ffly_free_sysconf(void) {


}
