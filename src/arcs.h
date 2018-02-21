# ifndef __ffly__arcs__h
# define __ffly__arcs__h
# include <mdlint.h>
# include "system/lat.h"
# include "system/flags.h"
# define REC_FLG_TOFREE 0x1

# define ffly_arcs_recw(__name, ...) \
	ffly_arc_recw(ffly_arc_lookup(__ffly_arccur__, *ffly_arcs_alias(__name)), no, __VA_ARGS__)
# define ffly_arcs_recr(__name, ...) \
	ffly_arc_recr(ffly_arc_lookup(__ffly_arccur__, *ffly_arcs_alias(__name)), __VA_ARGS__)

# define ffly_arcs_creatrec(__name, ...) \
	ffly_arc_creatrec(__ffly_arccur__, (*ffly_arcs_alias(__name) = ffly_arcs_alno()), __VA_ARGS__)

# define ffly_arcs_delrec(__name) \
	mdl_u64_t no = *ffly_arcs_alias(__name); \
	ffly_arc_delrec(__ffly_arccur__, ffly_arc_lookup(__ffly_arccur__, no)); \
	ffly_arcs_frno(no)

# define ffly_arcs_creatarc(__name) \
	ffly_creatarc(__ffly_arccur__, (*ffly_arcs_alias(__name) = ffly_arcs_alno()))

# define ffly_arcs_delarc(__name) \
	mdl_u64_t no = *ffly_arcs_alias(__name); \
	ffly_delarc((ffly_arcp)ffly_arc_lookup(__ffly_arccur__, no))->p; \
	ffly_arcs_frno(no)

# define ffly_arcs_tun(__name) \
	__ffly_arccur__ = (ffly_arcp)ffly_arc_lookup(__ffly_arccur__, *ffly_arcs_alias(__name))->p
# define ffly_arcs_bk(__name) \
	__ffly_arccur__ = __ffly_arccur__->bk;

enum {
	_ffly_rec_arc,
	_ffly_rec_def
};

typedef struct ffly_arc_rec {
	mdl_u64_t no;
	mdl_u8_t sort;
	ffly_flag_t flags;
	struct ffly_arc_rec *fd, *bk;
	void *p;
} *ffly_arc_recp;

typedef struct ffly_arc {
	ffly_arc_recp *rr, p;
	struct ffly_arc *bk;
} *ffly_arcp;

ffly_arc_recp ffly_arc_lookup(ffly_arcp, mdl_u64_t);
void ffly_arc_prepare(ffly_arcp);
void ffly_arc_free(ffly_arcp);

ffly_arc_recp ffly_arc_creatrec(ffly_arcp, mdl_u64_t, void*, mdl_u8_t, ffly_flag_t);
void ffly_arc_delrec(ffly_arcp, ffly_arc_recp);

ffly_arcp ffly_creatarc(ffly_arcp, mdl_u64_t);
void ffly_delarc(ffly_arcp);

void ffly_arc_recw(ffly_arc_recp, void*, mdl_uint_t, mdl_uint_t);
void ffly_arc_recr(ffly_arc_recp, void*, mdl_uint_t, mdl_uint_t);


mdl_u64_t* ffly_arcs_alias(char const*);

void ffly_arcs_init();
void ffly_arcs_de_init();

mdl_u64_t ffly_arcs_alno();
void ffly_arcs_frno(mdl_u64_t);

extern struct ffly_arc __ffly_arcroot__;
ffly_arcp extern __ffly_arccur__;
# endif /*__ffly__arcs__h*/
