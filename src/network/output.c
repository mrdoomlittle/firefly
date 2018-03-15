# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
ffly_size_t ff_net_sendto(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t __len, void const *__buf, mdl_uint_t __size, ffly_err_t *__err) {
    FF_NET_HDR *hdr;
    mdl_u8_t seg[FF_MTU];
    hdr = (FF_NET_HDR*)seg;
    mdl_u8_t *bufp = (mdl_u8_t*)__buf;
    mdl_u8_t *bufend = bufp+__size;
    mdl_u8_t *p = seg+sizeof(FF_NET_HDR);
    mdl_u8_t *end = seg+FF_MTU;
    mdl_uint_t seg_no = 0, left = 0;
    _next:
    left = __size-(bufp-(mdl_u8_t*)__buf);

    hdr->seg_no = seg_no++;
    hdr->size = left>=(end-p)?(end-p):left;
    ffly_mem_cpy(p, bufp, hdr->size);
    bufp+= hdr->size;

    mdl_uint_t size = hdr->size+sizeof(FF_NET_HDR);

    ffly_sock_sendto(__sock, seg, size, 0, __addr, __len, __err);
    if (_err(*__err)) {
        ffly_fprintf(ffly_err, "failed to send segment.\n");
        return 0;
    }

    if (bufp != bufend)
        goto _next;
}

ffly_size_t ff_net_send(FF_SOCKET *__sock, void const *__buf, mdl_uint_t __size, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    FF_NET_HDR hdr = {
        .size = __size
    };

    if (_err(*__err = ff_net_sndhdr(__sock, &hdr))) {
        ffly_fprintf(ffly_err, "failed to send header.\n");
        return 0; 
    }

    mdl_uint_t seq = 0, sent = 0;
    ffly_size_t res;
    mdl_uint_t left = 0;
    mdl_u8_t *p = (mdl_u8_t*)__buf;
    goto _sk;
    _send:
    ffly_printf("seq: %u\n", seq++);
    if ((res = ffly_sock_send(__sock, p, FF_MTU, 0, __err))<FF_MTU) {
        sent+=res;
        if (_err(*__err)) {
            ffly_fprintf(ffly_err, "failed to send segment.\n");
            return 0;
        } else
            goto _shard;
    }
    p+= FF_MTU;
    _sk:
    if (__size-(p-(mdl_u8_t*)__buf) >= FF_MTU) {
        goto _send;
    }

    left = __size-(p-(mdl_u8_t*)__buf);
    if (left>0) {
        if ((res = ffly_sock_send(__sock, p, left, 0, __err))<left) {
            sent+=res;
            if (_err(*__err)) {
                ffly_fprintf(ffly_err, "failed to send leftover.\n");
                return 0;
            } else
                goto _shard;
        }
    }
    return __size;
    _shard:
    return sent;
}
