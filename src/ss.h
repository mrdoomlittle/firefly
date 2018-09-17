# ifndef __ffly__ss__h
# define __ffly__ss__h
# include "ffint.h"
# include "system/dict.h"
# define SS_TOFREE 0x01

# define ss_dir_creat(__name) \
	*ffly_ss_alias(__name) = ffly_ss_dir_creat()
# define ss_dir_del(__name) \
	ffly_ss_dir_del(*ffly_ss_lookup(__name))
# define ss_file_creat(__name, ...) \
	*ffly_ss_alias(__name) = ffly_ss_file_creat(__VA_ARGS__)
# define ss_file_del(__name) \
	ffly_ss_file_del(*ffly_ss_lookup(__name))
# define ss_tun(__name) \
	ffly_ss_tun(*ffly_ss_lookup(__name))
# define ss_bk ffly_ss_bk()
typedef struct ffly_ss_file {
	ff_u8_t sort;
	ff_u8_t flags;
	ff_u64_t chip;
	struct ffly_ss_file *fd, **bk;
	void *p;
} *ffly_ss_filep;

typedef struct ffly_ss_dir {
	ffly_ss_filep files;

	ff_u64_t chip;
	struct ffly_ss_dir *bk;
} *ffly_ss_dirp;

void ffly_ss_prime(void);
void ffly_ss_fin(void);

enum {
	_ffly_ss_def,
	_ffly_ss_dir
};

ff_u64_t ffly_ss_file_creat(void*, ff_u8_t, ff_u8_t);
void ffly_ss_file_del(ff_u64_t);

ff_u64_t ffly_ss_dir_creat(void);
void ffly_ss_dir_del(ff_u64_t);

ff_u64_t* ffly_ss_alias(char const*);
ff_u64_t* ffly_ss_lookup(char const*);
void ffly_ss_tun(ff_u64_t);
void ffly_ss_bk(void);
extern ffly_ss_dirp __ffly_ssrt__;
extern ffly_ss_dirp __ffly_sscur__;
# endif /*__ffly__ss__h*/
