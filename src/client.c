# include "net.h"
# include "system/io.h"
# include "data/bzero.h"
int main() {
    ffly_io_init();
    ffly_err_t err;
    FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    ffly_bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(21299);
    ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

    char s[50];
    ff_net_recv(sock, s, 50, &err); 
    ffly_printf("%s\n", s);
    ff_net_close(sock);
    ffly_io_closeup();
}
