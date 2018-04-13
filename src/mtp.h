# ifndef __ffly__mtp__h
# define __ffly__mtp__h
# include "ffint.h"
# include "types.h"
# include "net.h"
enum {
	_ffly_mtp_rep_err	
};

enum {
	_ff_mtp_err_ptl,
	_ff_mtp_err_mst
};

typedef struct ffly_mtp_rep {
	ff_u8_t type;
	ff_u8_t err;
} *ffly_mtp_repp;


typedef struct ffly_mtp_hdr {
	char const *file;
} *ffly_mtp_hdrp;

char const* ffly_mtp_errst(ff_u8_t);

ff_err_t ffly_mtp_snd_report(FF_SOCKET*, ffly_mtp_repp);
ff_err_t ffly_mtp_rcv_report(FF_SOCKET*, ffly_mtp_repp);

ff_err_t ffly_mtp_snderr(ff_err_t, FF_SOCKET*);
ff_err_t ffly_mtp_rcverr(ff_err_t*, FF_SOCKET*);

ff_err_t ffly_mtp_sndhdr(FF_SOCKET*, ffly_mtp_hdrp);
ff_err_t ffly_mtp_rcvhdr(FF_SOCKET*, ffly_mtp_hdrp);

ff_err_t ffly_mtp_snd(FF_SOCKET*, void*, ff_uint_t);
void* ffly_mtp_rcv(FF_SOCKET*, ff_uint_t*, ff_err_t*);

ff_err_t ffly_mtpd_prepare(char const*);
ff_err_t ffly_mtpd_open();

ff_err_t ffly_mtpd_start();

ff_err_t ffly_mtpd_close();
ff_err_t ffly_mtpd_cleanup();
# endif /*__ffly__mtp__h*/
