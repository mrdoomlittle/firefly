# include "../ffint.h"
# include "../types.h"
# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
ff_size_t
ff_net_recvfrom(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t *__len,
	void *__buf, ff_uint_t __size, ff_err_t *__err)
{
	FF_NET_HDR *hdr;
	ff_u8_t seg[FF_MTU];
	hdr = (FF_NET_HDR*)seg;
	ff_u8_t *p = seg+sizeof(FF_NET_HDR);
	ff_u8_t *bufp = (ff_u8_t*)__buf;
	ff_uint_t togo = __size;
_next:
	ffly_sock_recvfrom(__sock, seg, FF_MTU, 0, __addr, __len, __err);
	if (_err(*__err)) {
		ffly_fprintf(ffly_err, "failed to receive segment.\n");
		return 0;
	}
 
	ffly_mem_cpy(bufp+(hdr->seg_no*(FF_MTU-sizeof(FF_NET_HDR))), p, hdr->size);
	togo-=hdr->size;
	if (togo>0)
		goto _next;
}

ff_size_t
ff_net_recv(FF_SOCKET *__sock, void *__buf, ff_uint_t __size, ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
	FF_NET_HDR hdr;

	if (_err(*__err = ff_net_rcvhdr(__sock, &hdr))) {
		ffly_fprintf(ffly_err, "failed to receive header.\n");
		return 0;
	}

	ff_uint_t seq = 0, recved = 0;
	ff_size_t res;
	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_uint_t left = 0;
	goto _sk;
_recv:
	ffly_printf("seq: %u\n", seq++);
	if ((res = ffly_sock_recv(__sock, p, FF_MTU, 0, __err))<FF_MTU) {
		if (_err(*__err)) {
			ffly_fprintf(ffly_err, "failed to receive segment.\n");
			return 0;
		} else {
			recved+=res;
			goto _fault;
		}
	} 
	p+=FF_MTU;
_sk:
	if (__size-(p-(ff_u8_t*)__buf) >= FF_MTU) {
		goto _recv;
	}

	left = __size-(p-(ff_u8_t*)__buf);
	if (left>0) {
		if ((res = ffly_sock_recv(__sock, p, left, 0, __err))<left) {
			if (_err(*__err)) {
				ffly_fprintf(ffly_err, "failed to receive leftover.\n");
				return 0;
			} else {
				recved+=res;
				goto _fault;
			}
		}
	}
	return hdr.size;
_fault:
	return recved; 
}
