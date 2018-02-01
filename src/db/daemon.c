# include "../db.h"
# include "../types/err_t.h"
# include "../system/io.h"
# include "../types/err_t.h"
# include "../data/bzero.h"
# include "../system/string.h"
# include "../system/err.h"
# include "../data/mem_cmp.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../data/str_len.h"
void static *ptrs[20];
void static **next = ptrs;

void static *vacant[20];
void static **end = vacant;

ffly_err_t static ff_db_login(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp *__user, ff_db_errno *__err, mdl_u8_t *__key) {
    ffly_printf("logging in.\n");
    ffly_err_t err;
    char id[40];
    mdl_uint_t il;
 
    ff_net_recv(__sock, &il, sizeof(mdl_uint_t), &err);
    if (il >= 40) {
        ffly_printf("id length too long.\n");
        reterr;
    }
    ffly_printf("recved id length.\n");

    ff_net_recv(__sock, id, il, &err);
    ffly_printf("recved id.\n");
    mdl_u32_t passkey;
    ff_net_recv(__sock, &passkey, sizeof(mdl_u32_t), &err);
    ffly_printf("idlen: %u\n", il);

    mdl_u8_t i = 0;
    while(i != il) {
        ffly_printf("id: %c", id[i]);
        i++;
    }
    ffly_printf("\n");

    ff_db_userp user = (*__user = ff_db_get_user(__d, id, il, &err)); 
    if (_err(err) || !user) {
        ffly_printf("user doesn't exist.\n");
        *__err = _ff_err_nsu;
        goto _fail;
    }

    if (passkey != user->passkey) {
        ffly_printf("passkey doesn't match.\n");
        *__err = _ff_err_lci;
        goto _fail;
    }

    if (user->loggedin) {
        ffly_printf("already logged in.\n");
        *__err = _ff_err_ali;
        goto _fail;
    }
    goto _succ;

    _fail:
    ff_db_snd_err(__sock, FFLY_FAILURE); 
    reterr;
    _succ:
    ff_db_snd_err(__sock, FFLY_SUCCESS);
    *__err = _ff_err_null;
    ff_db_keygen(__key);
    ff_db_add_key(__d, __key);
    ff_db_snd_key(__sock, __key, user->enckey);
    user->loggedin = 1;
    retok; 
}

ffly_err_t static ff_db_logout(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_errno *__err, mdl_u8_t *__key) {
    ffly_printf("logging out.\n");
    ffly_err_t err;
    mdl_u8_t key[KEY_SIZE];
    ff_db_rcv_key(__sock, key, __user->enckey);
    if (!ffly_mem_cmp(key, __key, KEY_SIZE)) {
        ff_db_rm_key(__d, __key); 
        __user->loggedin = 0;
        goto _succ;  
    }

    _fail:
    ff_db_snd_err(__sock, FFLY_FAILURE);
    reterr;
    _succ:
    ff_db_snd_err(__sock, FFLY_SUCCESS);
    *__err = _ff_err_null;
    retok;
}

# include "../system/util/hash.h"
mdl_u8_t cmdauth[] = {
    _ff_db_auth_null, //login
    _ff_db_auth_user, //logout
    _ff_db_auth_null, //pulse
    _ff_db_auth_root, //shutdown
    _ff_db_auth_null, //disconnect
    _ff_db_auth_null  //req_errno
};

mdl_u8_t has_auth(ff_db_userp __user, mdl_u8_t __cmd) {
    if (!__user)
        return (cmdauth[__cmd] == _ff_db_auth_null);
    return (__user->auth_level <= cmdauth[__cmd]);
}

void _ff_login();
void _ff_logout();
void _ff_pulse();
void _ff_shutdown();
void _ff_disconnect();
void _ff_req_errno();

void *jmp[] = {
    &_ff_login,
    &_ff_logout,
    &_ff_pulse,
    &_ff_shutdown,
    &_ff_disconnect,
    &_ff_req_errno
};

char const *msgstr(mdl_u8_t __kind) {
    switch(__kind) {
        case _ff_db_msg_login: return "login";
        case _ff_db_msg_logout: return "logout";
        case _ff_db_msg_pulse: return "pulse";
        case _ff_db_msg_shutdown: return "shutdown";
        case _ff_db_msg_disconnect: return "disconnect";
        case _ff_db_msg_req_errno: return "req_errno";
    }
    return "unknown";
}

# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
# define jmpend __asm__("jmp _ff_end")
# define jmpexit __asm__("jmp _ff_exit");
# include <linux/sockios.h>
# include <sys/ioctl.h>
# include <sys/types.h>
void ff_db_daemon(mdl_u16_t __port) {
    struct ff_dbd daemon;
    daemon.list = (void**)__ffly_mem_alloc((KEY_SIZE*0xff)*sizeof(void*));
    ffly_map_init(&daemon.users, _ffly_map_127);

    char const *rootid = "root";
    ff_db_userp root = ff_db_add_user(&daemon, rootid, ffly_str_len(rootid), ffly_hash("21299", 5));
    root->auth_level = _ff_db_auth_root; 
    root->enckey = 9331413;

    ffly_err_t err;
    struct sockaddr_in addr, cl;
    ffly_bzero(&addr, sizeof(struct sockaddr_in));
    ffly_bzero(&cl, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(__port);
    FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
    
    ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));

    socklen_t len;
    mdl_i8_t alive;
    ff_db_errno ern;
    mdl_u8_t key[KEY_SIZE];
    ff_db_userp user = NULL;
    struct ff_db_msg msg;
    FF_SOCKET *peer;

    __asm__("_ff_bk:\nnop");
    ff_net_listen(sock);
    peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
    alive = 0;

    while(alive != -1) {
        if (_err(err = ff_db_rcvmsg(peer, &msg))) {
            jmpexit;
        } 

        if (!has_auth(user, msg.kind)) {
            ffly_printf("user doesn't have authorization, for %s, user auth: %u\n", msgstr(msg.kind), !user?0:user->auth_level);
            ern = _ff_err_prd;
            ff_db_snd_err(peer, FFLY_FAILURE);
            continue;
        }

        if (_err(err = ff_db_snd_err(peer, FFLY_SUCCESS))) {
            jmpexit;
        }

        if (msg.kind > _ff_db_msg_req_errno) {
            jmpexit;
        } 
        jmpto(jmp[msg.kind]);

        __asm__("_ff_shutdown:\nnop"); {
            ffly_printf("goodbye.\n");
            ff_net_close(peer);    
            jmpexit;
        }
        __asm__("_ff_pulse:\nnop"); {

            jmpend; 
        }
        __asm__("_ff_login:\nnop"); {
            ff_db_login(&daemon, peer, &user, &ern, key);
            jmpend;
        }
        __asm__("_ff_logout:\nnop"); {
            ff_db_logout(&daemon, peer, &user, &ern, key);
            user = NULL;
            jmpend;
        }
        __asm__("_ff_disconnect:\nnop"); {
            ff_net_close(peer);
            __asm__("jmp _ff_bk");
        }
        __asm__("_ff_req_errno:\nnop"); {
            ff_db_snd_errno(peer, ern);
            jmpend;
        }
        __asm__("_ff_end:\nnop");
        // do somthing
    }

    ffly_printf("somthing went wrong.\n");
    __asm__("_ff_exit:\nnop");
    ff_net_close(sock);
    ffly_map_de_init(&daemon.users);
    __ffly_mem_free(daemon.list);
}

int main(int __argc, char const *__argv[]) {
    ffly_io_init();
    if (__argc != 2) {
        ffly_printf("usage: deamon {port number}.\n");
        return -1;
    }

    char const *port = __argv[1];
    ff_db_daemon(ffly_stno(port));  
    ffly_io_closeup();
    return 0;
}
