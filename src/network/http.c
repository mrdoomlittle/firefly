# include "http.h"
# include "sock.h"
# include "resolve.h"
# include "../data/str_cpy.h"
# include "../data/str_len.h"
# include "../system/io.h"
# include "../data/mem_cpy.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../data/str_cmp.h"
# include "../system/errno.h"
# include "../data/bzero.h"
# include <sys/ioctl.h>
# include "../system/nanosleep.h"
# define SLICE_SIZE 80
# define MAX_TRYS 10
struct http_hdr {
    char *date;
    char *server;
    char *end; 
};

mdl_u8_t is_end(char *__s) {
    if (*__s == '\r' && *(__s+1) == '\n')
        if (*(__s+2) == '\r' && *(__s+3) == '\n')
            return 1;
    return 0;
}

char *extfield(char *__s, char *__buf) {
    char *itr = __s;
    while(*itr != '\r' && *(itr+1) != '\n')
        *(__buf+(itr-__s)) = *(itr++);
    *(__buf+(itr-__s)) = '\0';
    mdl_uint_t len = (itr-__s)+1;
    char *field = (char*)__ffly_mem_alloc(len);
    ffly_mem_cpy(field, __buf, len);
    return field;
}

mdl_u8_t is_field(char *__field, char *__name, char *__buf) {
    char *itr = __field;
    while(*itr != ':') {
        if (*(__name+(itr-__field)) == '\0') {
            return 0;
        }
        if (*itr != *(__name+(itr-__field))) {
            return 0;
        }
        itr++;
    }

    if ((itr-__field) != ffly_str_len(__name))
        return 0;
    return 1;
}

ffly_err_t read_http_hdr(struct http_hdr *__hdr, char *__s) {
    char *itr = __s;
    // skip status
    while(*itr != '\r' && *(itr+1) != '\n') itr++;
    itr+=2;
    char buf[200];
    _next:
    {
        char *field = extfield(itr, buf);
        itr+= ffly_str_len(field);
    
        if (is_field(field, "Date", buf)) {
            __hdr->date = field;
        } else if (is_field(field, "Server", buf)) {
            __hdr->server = field;
        } else {
            __ffly_mem_free(field);
        }

        if (is_end(itr)) {
            __hdr->end = itr+4;  
            goto _end;
        }

        itr+= 2; 
    }

    goto _next;
    _end:
    return FFLY_SUCCESS;
}

void free_http_hdr(struct http_hdr *__hdr) {
    if (__hdr->date != NULL)
        __ffly_mem_free(__hdr->date);
    if (__hdr->server != NULL)
        __ffly_mem_free(__hdr->server);
}
ffly_err_t ffly_http_get(char *__host, char *__path, mdl_u16_t __port, void **__buf, ffly_size_t *__size) {
    struct ffly_socket sock;
    ffly_socket(&sock, AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = ffly_resolve(__host);
    addr.sin_port = htons(__port);

    printf("ip-addr: %u.%u.%u.%u\n", addr.sin_addr.s_addr&0xFF, addr.sin_addr.s_addr>>8&0xFF, addr.sin_addr.s_addr>>16&0xFF, addr.sin_addr.s_addr>>24&0xFF);

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

    char *p = (char*)__ffly_mem_alloc(SLICE_SIZE);
    mdl_uint_t off = 0, slice_c = 1, try_c = 0;
    ffly_size_t recved;
    int available;

    _again:
    if (off >= slice_c*SLICE_SIZE)
        p = (char*)__ffly_mem_realloc(p, (++slice_c)*SLICE_SIZE);
    ffly_printf("%u\n", off);

    ffly_nanosleep(0, 1000000);
    _try_again:
    if (!ioctl(sock.fd, FIONREAD, &available)) {
        if (available>0) {
            if ((recved = ffly_sock_recv(&sock, p+off, SLICE_SIZE, 0, &err))>0) {
                ffly_printf("recved: %u\n", recved);
                off+= recved;
                goto _again;
            }     
        } else {
            if (try_c < MAX_TRYS) {
                try_c++;
                goto _try_again;
            }
            // err
        }
    }

    *(p+(off++)) = '\0';

    struct http_hdr h;
    ffly_bzero(&h, sizeof(struct http_hdr));
    read_http_hdr(&h, p);
    ffly_printf("date: %s\n", h.date);
    ffly_printf("server: %s\n", h.server);
    ffly_printf("data: %s\n", h.end);
    

    ffly_size_t size  = (p+off)-h.end;
    void *buf = __ffly_mem_alloc(size);
    ffly_mem_cpy(buf, h.end, size);
    *__buf = buf;
    *__size = size;
    __ffly_mem_free(p);
    free_http_hdr(&h);  
    ffly_sock_close(&sock);
}
