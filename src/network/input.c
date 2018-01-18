# include "../net.h"
# include "../system/io.h"
# include "../system/err.h"
ffly_size_t ff_net_recv(FF_SOCKET *__sock, void *__buf, mdl_uint_t __size, ffly_err_t *__err) {
    *__err = FFLY_SUCCESS;
    FF_NET_HDR hdr = {

    };
    ffly_err_t err;
    if (_err(err = ff_net_rcv_hdr(__sock, &hdr))) {
        *__err = FFLY_FAILURE;
        return 0;
    }

    mdl_uint_t seq = 0;
    ffly_size_t res;
    mdl_u8_t *p = (mdl_u8_t*)__buf;
    goto _sk;
    _recv:
    ffly_printf("seq: %u\n", seq++);
    if ((res = ffly_sock_recv(__sock, p, FF_MTU, 0, &err))<FF_MTU) {
        *__err = FFLY_FAILURE;
        return 0;
    } 
    p+= FF_MTU;
    _sk:
    if (__size-(p-(mdl_u8_t*)__buf) >= FF_MTU) {
        goto _recv;
    }

    mdl_uint_t left = __size-(p-(mdl_u8_t*)__buf);
    if (left>0) {
        if ((res = ffly_sock_recv(__sock, p, left, 0, &err))<left) {
            *__err = FFLY_FAILURE;
            return 0;
        }
    }
    return 0;
}
