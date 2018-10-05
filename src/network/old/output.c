# include "../ffint.h"
# include "../types.h"
# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
ff_size_t
ff_net_sendto(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len,
	void const *__buf, ff_uint_t __size, ff_err_t *__err)
{
	FF_NET_HDR *hdr;
	ff_u8_t seg[FF_MTU];
	hdr = (FF_NET_HDR*)seg;
	ff_u8_t *bufp = (ff_u8_t*)__buf;
	ff_u8_t *bufend = bufp+__size;
	ff_u8_t *p = seg+sizeof(FF_NET_HDR);
	ff_u8_t *end = seg+FF_MTU;
	ff_uint_t seg_no = 0, left = 0;
_next:
	left = __size-(bufp-(ff_u8_t*)__buf);

	hdr->seg_no = seg_no++;
	hdr->size = left>=(end-p)?(end-p):left;
	ffly_mem_cpy(p, bufp, hdr->size);
	bufp+= hdr->size;

	ff_uint_t size = hdr->size+sizeof(FF_NET_HDR);

	ffly_sock_sendto(__sock, seg, size, 0, __addr, __len, __err);
	if (_err(*__err)) {
		ffly_fprintf(ffly_err, "failed to send segment.\n");
		return 0;
	}

	if (bufp != bufend)
		goto _next;
}

ff_size_t
ff_net_send(FF_SOCKET *__sock, void const *__buf, ff_uint_t __size, ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	FF_NET_HDR hdr = {
		.size = __size
	};

	if (_err(*__err = ff_net_sndhdr(__sock, &hdr))) {
		ffly_fprintf(ffly_err, "failed to send header.\n");
		return 0; 
	}

	ff_uint_t seq = 0, sent = 0;
	ff_size_t res;
	ff_uint_t left = 0;
	ff_u8_t *p = (ff_u8_t*)__buf;
	goto _sk;
_send:
	ffly_printf("seq: %u\n", seq++);
	if ((res = ffly_sock_send(__sock, p, FF_MTU, 0, __err))<FF_MTU) {
		if (_err(*__err)) {
			ffly_fprintf(ffly_err, "failed to send segment.\n");
			return 0;
		} else {
			sent+=res;
			goto _fault;
		}
	}
	p+= FF_MTU;
_sk:
	if (__size-(p-(ff_u8_t*)__buf) >= FF_MTU) {
		goto _send;
	}

	left = __size-(p-(ff_u8_t*)__buf);
	if (left>0) {
		if ((res = ffly_sock_send(__sock, p, left, 0, __err))<left) {
			if (_err(*__err)) {
				ffly_fprintf(ffly_err, "failed to send leftover.\n");
				return 0;
			} else {
				sent+=res;
				goto _fault;
			}
		}
	}
	return __size;
_fault:
	return sent;
}
