# ifndef __ffly__mug__h
# define __ffly__mug__h
# include "ffint.h"
# include "types.h"
# include "net.h"

/*
	rename and redesign

	purpose:
		providing a means to access infomation regarding
		the underlying mechanics of protocols used and other additional information
		that may be useful to the client.

		short like http but text and low level just a text document that
		may be of use to the client to know like status, users.

		NOTE:
			only to be used for human infomation.
*/
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

char const* ffly_mtp_errst(ff_u8_t);

ff_err_t ffly_mtp_snd_report(FF_SOCKET*, ffly_mtp_repp);
ff_err_t ffly_mtp_rcv_report(FF_SOCKET*, ffly_mtp_repp);

ff_err_t ffly_mtp_snd(FF_SOCKET*, void*, ff_uint_t);
void* ffly_mtp_rcv(FF_SOCKET*, ff_uint_t*, ff_err_t*);

void* ffly_mtp_req(FF_SOCKET*, char const*, ff_uint_t*);

ff_err_t ffly_mtpd_prepare(char const*);
ff_err_t ffly_mtpd_open();

ff_err_t ffly_mtpd_start();

ff_err_t ffly_mtpd_close();
ff_err_t ffly_mtpd_cleanup();
# endif /*__ffly__mtp__h*/
