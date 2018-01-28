# include "../db.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../types/err_t.h"
# include "../data/str_cmp.h"
# include "../system/string.h"

void ff_db_shutdown(FF_SOCKET *__sock) {
    struct ff_db_msg msg = {
        .kind = _ff_db_msg_shutdown
    };
    
    ff_db_sndmsg(__sock, &msg);
}

ff_db_errno static get_errno(FF_SOCKET *__sock, ffly_err_t *__err) {
    struct ff_db_msg msg = {
        .kind = _ff_db_msg_req_errno
    };

    ff_db_sndmsg(__sock, &msg);

    ff_db_errno ern;
    ff_db_rcv_errno(__sock, &ern);
    return ern;
}

ffly_err_t ff_db_login(FF_SOCKET *__sock, mdl_u8_t const *__id, mdl_uint_t __il, mdl_u32_t __passkey, mdl_u8_t *__key) {
    struct ff_db_msg msg = {
        .kind = _ff_db_msg_login
    };

    ff_db_sndmsg(__sock, &msg);
    ffly_printf("sent message.\n");
    ffly_err_t err;
    ff_net_send(__sock, &__il, sizeof(mdl_uint_t), &err);
    ffly_printf("sent id length.\n");

    ff_net_send(__sock, __id, __il, &err);
    ffly_printf("sent id.\n");
    ff_net_send(__sock, &__passkey, sizeof(mdl_u32_t), &err);
    ffly_printf("sent passkey.\n");

    ff_db_rcv_err(__sock, &err);
    if (_err(err)) {
        ff_db_errno ern = get_errno(__sock, &err);
        ffly_printf("errstr: %s\n", ff_db_errno_str(ern));
        return err;
    }

    ff_db_rcv_key(__sock, __key);
    retok;
}

ffly_err_t ff_db_logout(FF_SOCKET *__sock, mdl_u8_t *__key) {
    struct ff_db_msg msg = {
        .kind = _ff_db_msg_logout
    };
    ff_db_sndmsg(__sock, &msg);

    ffly_err_t err;
    ff_db_snd_key(__sock, __key);

    ff_db_rcv_err(__sock, &err);
    if (_err(err)) {
        ff_db_errno ern = get_errno(__sock, &err);
        ffly_printf("errstr: %s\n", ff_db_errno_str(ern));
        return err;
    }
}

int main(int __argc, char const *__argv[]) {
    ffly_io_init();
    char const **arg = __argv+1;
    char const *port = NULL;
    char const *ip = NULL;
    while(arg != __argv+__argc) {
        if (!ffly_str_cmp(*arg, "-port"))
            port = *(++arg);
        else if (!ffly_str_cmp(*arg, "-ip"))
            ip = *(++arg);
        arg++;
    }

    if (!port){
        ffly_printf("missing -port.\n");
        return -1;
    }

    if (!ip) {
        ffly_printf("missing -ip.\n");
        return -1;
    }

    ffly_printf("port: %s, address: %s\n", port, ip);

    ffly_err_t err;
    FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    ffly_bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ffly_stno(port));
    ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr));
    mdl_u8_t key[KEY_SIZE];

    char const *uname = "root";
    ff_db_login(sock, uname, ffly_str_len(uname), 21299, key);
    ff_db_logout(sock, key);

    ff_db_shutdown(sock);

    ff_net_close(sock);
    ffly_io_closeup();
    return 0;  
}
