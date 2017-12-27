# include "http.h"
# include "sock.h"
# include "resolve.h"
# include "../data/str_cpy.h"
# include "../data/str_len.h"
# include "../system/io.h"
ffly_err_t ffly_http_get(char *__host, char *__path, mdl_u16_t __port, void *__buf, ffly_size_t __size) {
    struct ffly_socket sock;
    ffly_socket(&sock, AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = ffly_resolve(__host);
    addr.sin_port = htons(__port);

    printf("%u.%u.%u.%u\n", addr.sin_addr.s_addr&0xFF, addr.sin_addr.s_addr>>8&0xFF, addr.sin_addr.s_addr>>16&0xFF, addr.sin_addr.s_addr>>24&0xFF);

    ffly_sock_connect(&sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));

    char hdr[200];
    char *s = hdr;
    ffly_str_cpy(s, "GET ");
    s+= 4;
    ffly_str_cpy(s, "http://");
    s+= 7;
    ffly_str_cpy(s, __host);
    s+= ffly_str_len(__host);
    *(s++) = '/';
    ffly_str_cpy(s, __path);
    s+= ffly_str_len(__path);
    *(s++) = ' ';
    ffly_str_cpy(s, "HTTP/1.1\r\nHost: ");
    s+= 16;
    ffly_str_cpy(s, __host);
    s+= ffly_str_len(__host);
    ffly_str_cpy(s, "\r\n\r\n");
    s+= 4;
    ffly_err_t err; 
    ffly_size_t sent = ffly_sock_send(&sock, hdr, s-hdr, 0, &err);
    ffly_printf("sent: %u\n", sent);
    ffly_printf("header: '%s'\n", hdr);

    void *buf[2000];

    ffly_size_t recved = ffly_sock_recv(&sock, buf, sizeof(buf), 0, &err);    
    ffly_printf("recved: %u\n", recved);

    char *begin = NULL;
    char *itr = buf;
    while(itr != buf+sizeof(buf)) {
        if (!begin) {
            if (*itr == '\r' && *(itr+1) == '\n') {
                if (*(itr+2) == '\r' && *(itr+3) == '\n') {
                    begin = itr+4;
                }
            }
        }
        itr++;
    }

    ffly_str_cpy(__buf, begin);

/*
    mdl_u8_t *itr = (mdl_u8_t*)__buf;
    while(1) {
        ffly_sock_recv(&sock, itr, 1, 0, &err);
        ffly_printf("recved: %c\n", *itr);
        if (itr > (mdl_u8_t*)__buf) {
            if (*(itr-1) == '\r' && *itr == '\n') break;
        }
    }
*/
    ffly_sock_close(&sock);
}
