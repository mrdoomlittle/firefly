# ifndef __ffly__mtp__h
# define __ffly__mtp__h
# include <mdlint.h>
# include "net.h"
# include "types/err_t.h"
enum {
	_ffly_mtp_rep_err	
};

enum {
	_ff_mtp_err_ptl,
	_ff_mtp_err_mst
};

typedef struct ffly_mtp_rep {
	mdl_u8_t type;
	mdl_u8_t err;
} *ffly_mtp_repp;


typedef struct ffly_mtp_hdr {
	char const *file;
} *ffly_mtp_hdrp;

char const* ffly_mtp_errst(mdl_u8_t);

ffly_err_t ffly_mtp_snd_report(FF_SOCKET*, ffly_mtp_repp);
ffly_err_t ffly_mtp_rcv_report(FF_SOCKET*, ffly_mtp_repp);

ffly_err_t ffly_mtp_snderr(ffly_err_t, FF_SOCKET*);
ffly_err_t ffly_mtp_rcverr(ffly_err_t*, FF_SOCKET*);

ffly_err_t ffly_mtp_sndhdr(FF_SOCKET*, ffly_mtp_hdrp);
ffly_err_t ffly_mtp_rcvhdr(FF_SOCKET*, ffly_mtp_hdrp);

ffly_err_t ffly_mtp_snd(FF_SOCKET*, void*, mdl_uint_t);
void* ffly_mtp_rcv(FF_SOCKET*, mdl_uint_t*, ffly_err_t*);

ffly_err_t ffly_mtpd_prepare(char const*);
ffly_err_t ffly_mtpd_open();

ffly_err_t ffly_mtpd_start();

ffly_err_t ffly_mtpd_close();
ffly_err_t ffly_mtpd_cleanup();
# endif /*__ffly__mtp__h*/
