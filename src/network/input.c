# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/mem_cpy.h"
ffly_size_t ff_net_recvfrom(FF_SOCKET *__sock, struct sockaddr *__addr, socklen_t *__len, void *__buf, mdl_uint_t __size, ffly_err_t *__err) {
    FF_NET_HDR *hdr;
    mdl_u8_t seg[FF_MTU];
    hdr = (FF_NET_HDR*)seg;
    mdl_u8_t *p = seg+sizeof(FF_NET_HDR);
    mdl_u8_t *bufp = (mdl_u8_t*)__buf;
    mdl_uint_t togo = __size;
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

ffly_size_t ff_net_recv(FF_SOCKET *__sock, void *__buf, mdl_uint_t __size, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    FF_NET_HDR hdr;

    if (_err(*__err = ff_net_rcvhdr(__sock, &hdr))) {
        ffly_fprintf(ffly_err, "failed to receive header.\n");
        return 0;
    }

    mdl_uint_t seq = 0, recved = 0;
    ffly_size_t res;
    mdl_u8_t *p = (mdl_u8_t*)__buf;
    mdl_uint_t left = 0;
    goto _sk;
    _recv:
    ffly_printf("seq: %u\n", seq++);
    if ((res = ffly_sock_recv(__sock, p, FF_MTU, 0, __err))<FF_MTU) {
        recved+=res;
        if (_err(*__err)) {
            ffly_fprintf(ffly_err, "failed to receive segment.\n");
            return 0;
        } else
            goto _shard;
    } 
    p+= FF_MTU;
    _sk:
    if (__size-(p-(mdl_u8_t*)__buf) >= FF_MTU) {
        goto _recv;
    }

    left = __size-(p-(mdl_u8_t*)__buf);
    if (left>0) {
        if ((res = ffly_sock_recv(__sock, p, left, 0, __err))<left) {
            recved+=res;
            if (_err(*__err)) {
                ffly_fprintf(ffly_err, "failed to receive leftover.\n");
                return 0;
            } else  
                goto _shard;
        }
    }
    return hdr.size;
    _shard:
    return recved; 
}
